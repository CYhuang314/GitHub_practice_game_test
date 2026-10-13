param([Parameter(ValueFromRemainingArguments = $true)][string[]]$GameArguments)
$ErrorActionPreference = 'Stop'
try {
    & (Join-Path $PSScriptRoot 'scripts\build.ps1')
    Push-Location $PSScriptRoot
    try {
        & (Join-Path $PSScriptRoot 'build\git-game.exe') @GameArguments
        $GameExit = $LASTEXITCODE
    } finally { Pop-Location }
    exit $GameExit
} catch {
    [Console]::Error.WriteLine('Unable to start Git Patch Pusher: ' + $_.Exception.Message)
    exit 1
}
