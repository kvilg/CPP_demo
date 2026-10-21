<#
.SYNOPSIS
    Сборка, тесты и запуск проектов CPP_demo.

.DESCRIPTION
    Обёртка над CMakePresets.json. Заменила два скрипта, которые лежали
    в qt/QtWidgetsDemo и qt/TcpServer и отличались пятью строками.

.EXAMPLE
    .\build.ps1
    Собрать всё в конфигурации debug.

.EXAMPLE
    .\build.ps1 -Test
    Собрать и прогнать тесты.

.EXAMPLE
    .\build.ps1 -Preset asan -Test
    Тесты под ASan/UBSan.

.EXAMPLE
    .\build.ps1 -Run TcpServer
    Собрать и запустить сервер.

.EXAMPLE
    .\build.ps1 -Preset release -Clean -Run QtWidgetsDemo
#>
[CmdletBinding()]
param(
    [ValidateSet("debug", "release", "asan", "coverage")]
    [string]$Preset = "debug",

    [string]$Run,
    [switch]$Test,
    [switch]$Clean
)

$ErrorActionPreference = "Stop"

# Компилятор и ninja: если их нет в PATH, подставляем из комплекта Qt.
# Версия не зашита — берём новейшую, как это делает cmake/qt.cmake.
function Add-ToolToPath([string]$pattern, [string]$exe) {
    if (Get-Command $exe -ErrorAction SilentlyContinue) { return }

    $dir = Get-ChildItem -Path $pattern -Directory -ErrorAction SilentlyContinue |
           Sort-Object Name -Descending |
           Select-Object -First 1
    if ($dir) {
        $bin = Join-Path $dir.FullName "bin"
        if (-not (Test-Path $bin)) { $bin = $dir.FullName }
        $env:Path = "$bin;$env:Path"
        Write-Host "PATH += $bin" -ForegroundColor DarkGray
    }
}

Add-ToolToPath "C:/Qt/Tools/mingw*_64" "g++"
Add-ToolToPath "C:/Qt/Tools/Ninja*" "ninja"

$buildDir = Join-Path $PSScriptRoot "build/$Preset"

if ($Clean -and (Test-Path $buildDir)) {
    Write-Host "Удаляю build/$Preset ..." -ForegroundColor Yellow
    Remove-Item $buildDir -Recurse -Force
}

Write-Host "`n[1/2] Конфигурация (preset: $Preset)" -ForegroundColor Cyan
cmake --preset $Preset
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Host "`n[2/2] Сборка" -ForegroundColor Cyan
cmake --build --preset $Preset
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

if ($Test) {
    Write-Host "`nТесты" -ForegroundColor Cyan
    ctest --preset $Preset
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}

if ($Run) {
    $exe = Get-ChildItem -Path $buildDir -Filter "$Run.exe" -Recurse -File |
           Select-Object -First 1 -ExpandProperty FullName

    if (-not $exe) {
        Write-Host "$Run.exe не найден в build/$Preset" -ForegroundColor Red
        Write-Host "Доступные цели: Linked, Linked_tests, QtWidgetsDemo, TcpServer" -ForegroundColor DarkGray
        exit 1
    }

    Write-Host "`nЗапускаю $exe" -ForegroundColor Green
    & $exe
}
