# CPP_demo

Учебные проекты на C++20 и Qt 6.

| Проект | Что это |
|---|---|
| [Linked](Linked) | Двусвязный список своими руками, с тестами на doctest и списком упражнений |
| [qt](qt) | Мониторинг сигнала: эмулятор и регистратор, работают в паре — реализация тестового задания |
| [qt/TcpServer](qt/TcpServer) | Эмулятор: генерирует сигнал и отдаёт по TCP |
| [qt/QtWidgetsDemo](qt/QtWidgetsDemo) | Регистратор: принимает сигнал и рисует график на Qt Widgets и Charts |

---

## Что нужно

- CMake 4.0+ и Ninja
- Компилятор с C++20: g++ 11+, clang 14+ или MSVC 19.3+
- Qt 6.5+ — только для проектов из `qt/`. Без него они просто
  пропускаются, а `Linked` с тестами собирается

Qt ищется автоматически: берётся новейший комплект под выбранный
компилятор (`mingw_64` для g++, `msvc2022_64` для MSVC). Если он лежит
в нетипичном месте — `-DCMAKE_PREFIX_PATH=путь/к/Qt/6.x/mingw_64`.

---

## Сборка

```powershell
.\build.ps1
```

Скрипт — обёртка над `CMakePresets.json`, он же подставит в `PATH`
компилятор и ninja из комплекта Qt, если их там нет.

```powershell
.\build.ps1 -Test                   # собрать и прогнать тесты
.\build.ps1 -Demo                   # запустить эмулятор и регистратор вместе
.\build.ps1 -Run TcpServer          # собрать и запустить одну цель
.\build.ps1 -Preset release -Clean  # пересобрать начисто
```

Проекты из `qt/` по отдельности бесполезны: эмулятор шлёт числа в
пустоту, регистратор рисует пустой график. `-Demo` поднимает оба — и
избавляет от второй копии IDE.

То же самое напрямую через CMake:

```powershell
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

Пресеты: `debug`, `release`, `asan`, `coverage`. Бинарники попадают
в `build/<пресет>/`.

Проекты из `qt/` конфигурируются только из корня: общие настройки
приходят из `qt/CMakeLists.txt` и `cmake/`.

### Свои пути

Машинные настройки — компилятор, путь к ninja, нестандартный Qt — не
место в репозитории. Для них есть `CMakeUserPresets.json` рядом с
`CMakePresets.json`, он уже в `.gitignore`:

```json
{
    "version": 3,
    "configurePresets": [
        {
            "name": "my-debug",
            "inherits": "debug",
            "cacheVariables": {
                "CMAKE_CXX_COMPILER": "C:/Qt/Tools/mingw1310_64/bin/g++.exe",
                "CMAKE_MAKE_PROGRAM": "C:/Qt/Tools/Ninja/ninja.exe",
                "CMAKE_PREFIX_PATH": "C:/Qt/6.11.1/mingw_64"
            }
        }
    ]
}
```

---

## Проверки

**Предупреждения компилятора включены и считаются ошибками**:
`-Wall -Wextra -Wpedantic -Werror`, на MSVC `/W4 /permissive- /WX`.
Это не настраивается — предупреждение, которое можно не заметить, рано
или поздно становится багом.

**Санитайзеры** ловят то, что в `List` сделано руками: выход за границы,
чтение освобождённой памяти, утечки.

```powershell
.\build.ps1 -Preset asan -Test
```

На MinGW не работают — в mingw-w64 нет `libasan`, сборка откажется
конфигурироваться и скажет, что делать. Нужен MSVC, clang или Linux.

**Покрытие тестами**:

```powershell
.\build.ps1 -Preset coverage -Test
gcov -o build/coverage/Linked/CMakeFiles/Linked_tests.dir Linked/List.cpp
```

Для HTML-отчёта — `pip install gcovr`, затем
`gcovr --root . --html-details build/coverage/coverage.html`.

**Форматирование** задано в [.clang-format](.clang-format):

```powershell
clang-format -i Linked/*.cpp Linked/*.h
clang-format --dry-run -Werror Linked/List.cpp   # только проверить
```

**Проверки идиом** — [.clang-tidy](.clang-tidy). Это не ошибки, а
«так лучше», сборку не ломают:

```powershell
cmake --preset debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
clang-tidy -p build/debug Linked/List.cpp
```

`clang-format` и `clang-tidy` есть в комплекте Visual Studio
(`VC/Tools/Llvm/x64/bin`) и в `C:/Qt/Tools/llvm-mingw*/bin`.

---

## Как разложено

```
CMakeLists.txt         корневой проект, единственная точка входа
CMakePresets.json      пресеты сборки
build.ps1              обёртка над пресетами
cmake/
  get_cpm.cmake        загрузчик CPM фиксированной версии
  dependencies.cmake   внешние зависимости (doctest через CPM)
  qt.cmake             поиск Qt, windeployqt, cpp_demo_qt_app()
  CompilerOptions.cmake предупреждения, санитайзеры, покрытие
Linked/                двусвязный список и его тесты
qt/
  CMakeLists.txt       общее для двух qt-проектов
  README.md            задание и что из него сделано
  QtWidgetsDemo/
  TcpServer/
```

Зависимости тянет [CPM](https://github.com/cpm-cmake/CPM.cmake) при
конфигурации, вручную ставить ничего не нужно. Версии зафиксированы:
CPM 0.43.2 (с проверкой sha256), doctest 2.5.3.
