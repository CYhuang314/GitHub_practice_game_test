param([switch]$Tests)
$ErrorActionPreference = 'Stop'
$ProjectRoot = Split-Path $PSScriptRoot -Parent
$BuildDirectory = Join-Path $ProjectRoot 'build'
function Find-CompilerCommand([string]$Name) {
    $Command = Get-Command -Name $Name -CommandType Application -ErrorAction SilentlyContinue |
        Select-Object -First 1
    if ($Command) { return $Command.Source }
    return $null
}

function Import-VisualStudioEnvironment {
    $InstallerRoot = ${env:ProgramFiles(x86)}
    if (-not $InstallerRoot) { return $false }
    $Vswhere = Join-Path $InstallerRoot 'Microsoft Visual Studio\Installer\vswhere.exe'
    if (-not (Test-Path -LiteralPath $Vswhere)) { return $false }
    $Vcvars = & $Vswhere -latest -products '*' `
        -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
        -find 'VC\Auxiliary\Build\vcvars64.bat' | Select-Object -First 1
    if (-not $Vcvars -or -not (Test-Path -LiteralPath $Vcvars)) { return $false }
    # Import only compiler-related variables into this PowerShell process.
    $EnvironmentLines = & $env:ComSpec /d /s /c ('call "' + $Vcvars + '" >nul && set')
    if ($LASTEXITCODE -ne 0) { return $false }
    foreach ($Line in $EnvironmentLines) {
        if ($Line -match '^(PATH|INCLUDE|LIB|LIBPATH)=(.*)$') {
            [Environment]::SetEnvironmentVariable($Matches[1], $Matches[2], 'Process')
        }
    }
    return $true
}

$Compiler = $null
if ($env:CC) {
    $Compiler = Find-CompilerCommand $env:CC
    if (-not $Compiler -and $env:CC -in @('cl', 'cl.exe', 'clang-cl', 'clang-cl.exe')) {
        if (Import-VisualStudioEnvironment) { $Compiler = Find-CompilerCommand $env:CC }
    }
    if (-not $Compiler) {
        throw "Configured C compiler not found: $env:CC. Set CC to an executable name or full path."
    }
} else {
    foreach ($Candidate in @('gcc', 'clang', 'cl')) {
        $Compiler = Find-CompilerCommand $Candidate
        if ($Compiler) { break }
    }
    if (-not $Compiler) {
        $SearchRoots = @()
        if ($env:SystemDrive) {
            $SearchRoots += Join-Path $env:SystemDrive 'msys64\ucrt64\bin\gcc.exe'
            $SearchRoots += Join-Path $env:SystemDrive 'msys64\mingw64\bin\gcc.exe'
        }
        foreach ($Candidate in $SearchRoots) {
            if (Test-Path -LiteralPath $Candidate) { $Compiler = $Candidate; break }
        }
    }
    if (-not $Compiler -and (Import-VisualStudioEnvironment)) {
        $Compiler = Find-CompilerCommand 'cl'
    }
    if (-not $Compiler) {
        throw @'
C compiler not found. Install one of these, then run run.cmd again:
- MSYS2 UCRT64 GCC: https://www.msys2.org/
  In its UCRT64 terminal: pacman -S --needed mingw-w64-ucrt-x86_64-gcc
- Visual Studio Build Tools with Desktop development with C++.
You may also set CC to the full path of an installed GCC or Clang executable.
Git alone does not include a C compiler.
'@
    }
}

$CompilerName = [IO.Path]::GetFileNameWithoutExtension($Compiler)
if ($CompilerName -in @('cl', 'clang-cl') -and (-not $env:INCLUDE -or -not $env:LIB)) {
    if (-not (Import-VisualStudioEnvironment)) {
        throw 'MSVC requires the Windows SDK environment. Install Desktop development with C++ or use a Developer Command Prompt.'
    }
}

New-Item -ItemType Directory -Path $BuildDirectory -Force | Out-Null
# GCC subprocesses and freshly built games may need DLLs beside the compiler.
$env:PATH = [IO.Path]::GetDirectoryName($Compiler) + [IO.Path]::PathSeparator + $env:PATH
$Sources = @(Get-ChildItem -LiteralPath (Join-Path $ProjectRoot 'src') -Filter '*.c' -Recurse |
    Where-Object { -not $Tests -or $_.Name -ne 'main.c' } |
    Sort-Object FullName | ForEach-Object FullName)
$OutputName = if ($Tests) { 'test-game.exe' } else { 'git-game.exe' }
$OutputPath = Join-Path $BuildDirectory $OutputName
if ($Tests) { $Sources += Join-Path $ProjectRoot 'tests\test_game.c' }
if ($CompilerName -in @('cl', 'clang-cl')) {
    $Arguments = @('/nologo', '/std:c11', '/W4', '/WX', '/O2',
        '/D_CRT_SECURE_NO_WARNINGS', '/I', (Join-Path $ProjectRoot 'include')) +
        $Sources + @('/Fe:' + $OutputPath)
} else {
    $Arguments = @('-std=c11', '-Wall', '-Wextra', '-Wpedantic', '-Werror', '-O2',
        '-D_CRT_SECURE_NO_WARNINGS',
        '-I', (Join-Path $ProjectRoot 'include')) + $Sources + @('-o', $OutputPath)
}
Write-Host ('Compiler: ' + $Compiler)
# MSVC places object files in the current directory. Keep them inside build/.
Push-Location $BuildDirectory
try {
    & $Compiler @Arguments
    if ($LASTEXITCODE -ne 0) { throw "Compilation failed (exit $LASTEXITCODE)." }
} finally { Pop-Location }
if ($Tests) {
    Push-Location $BuildDirectory
    try {
        & $OutputPath
        if ($LASTEXITCODE -ne 0) { throw 'Game tests failed.' }
    } finally { Pop-Location }
} else { Write-Host "Built: $OutputPath" }
