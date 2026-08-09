param(
    [string]$BuildType = "Debug"
)

if ($PSBoundParameters.ContainsKey("BuildType")) {
    Write-Host "Building $BuildType build"
}

$RootDir = Resolve-Path (Join-Path $PSScriptRoot "..")
$BuildDir = Join-Path $RootDir "build"

if (-not (Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}
elseif ($env:REBUILD) {
    Remove-Item -Recurse -Force $BuildDir
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

Push-Location $BuildDir
try {
    Write-Host "Building cmake target"
    cmake $RootDir -DCMAKE_BUILD_TYPE=$BuildType -DMST_RUN_TESTS=True -DMST_UTILS=True
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

    Write-Host "Building mst"
    cmake --build . --config $BuildType --parallel 10
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

    if ($env:TESTING) {
        ctest -C $BuildType --output-on-failure
        if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
    }
}
finally {
    Pop-Location
}
