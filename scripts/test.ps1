$env:TESTING = "1"
& (Join-Path $PSScriptRoot "build.ps1") @args
exit $LASTEXITCODE
