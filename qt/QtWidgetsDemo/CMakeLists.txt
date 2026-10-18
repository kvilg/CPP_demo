cmake_minimum_required(VERSION 3.21)

# ---------------------------------------------------------------------------
# АВТОНАСТРОЙКА Qt И КОМПИЛЯТОРА
# ---------------------------------------------------------------------------
# Тот же блок, что в QtDemo и TcpServer: проект должен собираться из любой IDE,
# даже если она не знает, где лежит Qt (CLion игнорирует CMakePresets.json).
#
# Комплект Qt обязан соответствовать компилятору:
#   Visual Studio 2022 -> .../Qt/<версия>/msvc2022_64
#   MinGW (g++)         -> .../Qt/<версия>/mingw_64
# Смешивать нельзя, будет ошибка линковки.
#
# Ручное задание (перебивает автопоиск):
#   list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.12.0/mingw_64")
#   set(QTDIR "C:/Qt/6.12.0/mingw_64")

# 1) Комплект Qt: явный путь -> QTDIR -> автопоиск в типовых местах.
set(QT_DEMO_QT_PREFIX "" CACHE PATH "Явный путь к комплекту Qt (C:/Qt/6.12.0/mingw_64)")

set(_qt_prefix "")
set(_qt_from_autosearch FALSE)

if(QT_DEMO_QT_PREFIX)
    set(_qt_prefix "${QT_DEMO_QT_PREFIX}")
elseif(DEFINED ENV{QTDIR})
    set(_qt_prefix "$ENV{QTDIR}")
else()
    file(GLOB _qt_mingw "C:/Qt/*/mingw_64" "D:/Qt/*/mingw_64")
    file(GLOB _qt_msvc  "C:/Qt/*/msvc2022_64" "D:/Qt/*/msvc2022_64")

    if(_qt_mingw)
        list(SORT _qt_mingw)
        list(GET _qt_mingw 0 _qt_prefix)
        set(_qt_from_autosearch TRUE)
    elseif(_qt_msvc)
        list(SORT _qt_msvc)
        list(GET _qt_msvc 0 _qt_prefix)
        set(_qt_from_autosearch TRUE)
    endif()
endif()

if(_qt_prefix)
    if(CMAKE_PREFIX_PATH)
        message(STATUS "Qt: CMAKE_PREFIX_PATH уже задан -> ${CMAKE_PREFIX_PATH}")
        message(STATUS "Qt: наш путь (${_qt_prefix}) НЕ подмешан, приоритет за внешним")
    else()
        list(APPEND CMAKE_PREFIX_PATH "${_qt_prefix}")
        if(_qt_from_autosearch)
            message(STATUS "Qt: комплект найден автоматически -> ${_qt_prefix}")
        else()
            message(STATUS "Qt: комплект задан явно -> ${_qt_prefix}")
        endif()
    endif()
else()
    message(STATUS "Qt: автопоиск ничего не нашёл, укажи QT_DEMO_QT_PREFIX вручную")
endif()

# 2) Qt под MinGW -> берём ровно тот g++, которым он собран,
#    и добавляем его bin в PATH (иначе g++.exe не найдёт свои DLL).
if(_qt_prefix MATCHES "mingw_64$")
    file(GLOB _mingw_tools "C:/Qt/Tools/mingw*_64" "D:/Qt/Tools/mingw*_64")

    if(_mingw_tools)
        list(SORT _mingw_tools)
        list(GET _mingw_tools 0 _mingw_root)

        if(NOT DEFINED CMAKE_CXX_COMPILER)
            set(CMAKE_CXX_COMPILER "${_mingw_root}/bin/g++.exe"
                CACHE FILEPATH "Компилятор C++ (Qt-MinGW)" FORCE)
        endif()

        set(ENV{PATH} "${_mingw_root}/bin;$ENV{PATH}")
        message(STATUS "MinGW: ${_mingw_root}")
    endif()
endif()

project(QtWidgetsDemo
    VERSION 1.0.0
    DESCRIPTION "Тот же интерфейс, что QtDemo, но на Qt Widgets"
    LANGUAGES CXX
)

# ---------------------------------------------------------------------------
# Общие настройки
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)   # moc_*.cpp из Q_OBJECT
set(CMAKE_AUTORCC ON)   # qrc_*.cpp из .qrc

if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif()

# ---------------------------------------------------------------------------
# Qt
# ---------------------------------------------------------------------------
# Widgets вместо Quick: QMainWindow, QVBoxLayout, QPushButton.
# Network даёт QTcpSocket (в src/main.cpp клиент подключается к серверу).
# Charts даёт QChart/QLineSeries — готовую отрисовку графика сигнала.
find_package(Qt6 6.5 QUIET COMPONENTS Core Gui Widgets Network Charts)

if(NOT Qt6_FOUND)
    message(FATAL_ERROR
        "Qt6 не найден (Qt6_DIR-NOTFOUND).\n"
        "Укажи путь к Qt, например:\n"
        "  $env:QTDIR = \"C:/Qt/6.12.0/mingw_64\"\n"
        "или -DCMAKE_PREFIX_PATH=C:/Qt/6.12.0/mingw_64\n"
        "\n"
        "Подсказка: сейчас CMake выбрал компилятор \"${CMAKE_CXX_COMPILER_ID}\".\n"
        "  MSVC        -> бери Qt из папки msvc2022_64\n"
        "  GNU (MinGW) -> бери Qt из папки mingw_64\n"
        "\n"
        "Скачать Qt 6: https://www.qt.io/download"
    )
endif()

message(STATUS "Qt ${Qt6_VERSION} найден: ${Qt6_DIR}")

# ---------------------------------------------------------------------------
# Цель
# ---------------------------------------------------------------------------
# MANUAL_FINALIZATION нужен из-за target_include_directories ниже.
#
# Файлы перечислены явно, а не через GLOB: добавленный .cpp должен попадать
# в сборку сразу, иначе придётся вручную сбрасывать кэш CMake.
#
# main.cpp     — сборка окна и работа с сокетом
# RealTimePlot — виджет графика
set(QT_WIDGETS_DEMO_SOURCES
    src/main.cpp
    src/RealTimePlot.cpp
)

set(QT_WIDGETS_DEMO_HEADERS
    src/RealTimePlot.h
)

qt_add_executable(QtWidgetsDemo
    ${QT_WIDGETS_DEMO_SOURCES}
    ${QT_WIDGETS_DEMO_HEADERS}
)

target_link_libraries(QtWidgetsDemo PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Charts
)

target_include_directories(QtWidgetsDemo PRIVATE src)

# ---------------------------------------------------------------------------
# Ресурсы
# ---------------------------------------------------------------------------
# Ищем файл от корня проекта, в qrc кладём без папки resources:
# resources/images/app-icon.svg -> ":/images/app-icon.svg"
qt_add_resources(QtWidgetsDemo "resources"
    PREFIX "/"
    BASE "${CMAKE_CURRENT_SOURCE_DIR}/resources"
    FILES
        resources/images/app-icon.svg
)

set_target_properties(QtWidgetsDemo PROPERTIES
    # На Windows не ставим WIN32_EXECUTABLE: в консоли видно qDebug/qWarning.
    WIN32_EXECUTABLE FALSE
    MACOSX_BUNDLE TRUE
)

# qt_finalize_executable() здесь НЕ вызывается: он нужен только
# вместе с MANUAL_FINALIZATION в qt_add_executable(). Без того флага
# Qt финализирует цель сам.

# ---------------------------------------------------------------------------
# Деплой: Qt DLL рядом с .exe
# ---------------------------------------------------------------------------
# Зачем: Qt лежит в C:\Qt, а не рядом с программой. Без этого шага запуск
# падает с 0xC0000135 (STATUS_DLL_NOT_FOUND).
#
# POST_BUILD, а не отдельная цель: чтобы обычный Run в любой IDE сразу давал
# рабочую программу. Повторный windeployqt занимает доли секунды.
if(WIN32)
    # Qt6_DIR = <qt>/lib/cmake/Qt6  ->  <qt>/bin
    get_filename_component(QT_BIN_DIR "${Qt6_DIR}/../../../bin" ABSOLUTE)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_BIN_DIR}" REQUIRED)

    add_custom_command(TARGET QtWidgetsDemo POST_BUILD
        COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                --no-translations
                "$<TARGET_FILE:QtWidgetsDemo>"
        COMMENT "windeployqt: Qt-библиотеки рядом с QtWidgetsDemo.exe"
        VERBATIM
    )

    # Цель создаётся только при сборке проекта отдельно: при сборке
    # вместе с QtDemo и TcpServer все трое попытались бы создать "deploy"
    # и CMake упал бы с "another target with the same name already exists".
    if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
        add_custom_target(deploy
            COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                    --no-translations
                    "$<TARGET_FILE:QtWidgetsDemo>"
            DEPENDS QtWidgetsDemo
            COMMENT "windeployqt (явный запуск)"
            VERBATIM
        )
    endif()
endif()