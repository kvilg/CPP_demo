<#
.SYNOPSIS
    Сборка и запуск TcpServer (Qt 6.12 + MinGW + Ninja).

.EXAMPLE
    .\build.ps1 -Run
    Собрать Debug и сразу запустить сервер на порту 1234.

.EXAMPLE
    .\build.ps1 -Configuration Release -Run

.EXAMPLE
    .\build.ps1 -Clean -Run
    Пересобрать начисто.
#>
[CmdletBinding()]
param(
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Debug",

    [switch]$Run,
    [switch]$Clean
)

$ErrorActionPreference = "Stop"

$qtRoot   = "C:\Qt\6.12.0\mingw_64"
$mingwBin = "C:\Qt\Tools\mingw1310_64\bin"
$ninja    = "C:\Qt\Tools\Ninja"
$preset   = "mingw-$($Configuration.ToLower())"
$buildDir = "$PSScriptRoot\build\$preset"

# g++ требует своих DLL из папки bin в PATH
$env:Path = "$mingwBin;$ninja;$env:Path"

if ($Clean -and (Test-Path $buildDir)) {
    Write-Host "Удаляю build\$preset ..." -ForegroundColor Yellow
    Remove-Item $buildDir -Recurse -Force
}

Write-Host "`n[1/2] Конфигурация (preset: $preset)" -ForegroundColor Cyan
cmake --preset $preset
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Host "`n[2/2] Сборка" -ForegroundColor Cyan
cmake --build --preset $preset
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

$exe = Get-ChildItem -Path $buildDir -Filter "TcpServer.exe" -Recurse -File |
       Select-Object -First 1 -ExpandProperty FullName

if (-not $exe) {
    Write-Host "TcpServer.exe не найден в $buildDir" -ForegroundColor Red
    exit 1
}

Write-Host "`nГотово: $exe" -ForegroundColor Green

if ($Run) {
    Write-Host "Запускаю сервер..." -ForegroundColor Cyan
    & $exe
}
