param(
    [string]$BuildType = "Debug"
)

if ($PSBoundParameters.ContainsKey("BuildType")) {
    Write-Host "Building $BuildType build"
}

$RootDir = Resolve-Path (Join-Path $PSScriptRoot "..")
$BuildDir = Join-Path $RootDir "build"
$Preset = "dev-$($BuildType.ToLower())"

if ($env:REBUILD -and (Test-Path $BuildDir)) {
    Remove-Item -Recurse -Force $BuildDir
}

Push-Location $RootDir
try {
    Write-Host "Configuring cmake preset $Preset"
    cmake --preset $Preset
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

    Write-Host "Building mst"
    cmake --build --preset $Preset --parallel 10
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

    if ($env:TESTING) {
        ctest --preset $Preset
        if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
    }
}
finally {
    Pop-Location
}
