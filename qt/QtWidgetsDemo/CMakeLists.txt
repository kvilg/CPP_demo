cmake_minimum_required(VERSION 3.21)

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
# Общие модули из ../../cmake: поиск Qt и флаги компилятора. В общей сборке
# они уже подключены корневым проектом, при отдельной подключаем сами.
if(NOT COMMAND cpp_demo_deploy_qt OR NOT TARGET project_warnings)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
endif()
if(NOT COMMAND cpp_demo_deploy_qt)
    include(qt)
endif()
if(NOT TARGET project_warnings)
    include(CompilerOptions)
endif()

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets Network Charts)

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

# Общие предупреждения и санитайзеры, см. cmake/CompilerOptions.cmake
target_link_libraries(QtWidgetsDemo PRIVATE project_warnings)

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
# Функция объявлена в cmake/qt.cmake: POST_BUILD windeployqt плюс цель
# deploy_QtWidgetsDemo для ручного запуска.
cpp_demo_deploy_qt(QtWidgetsDemo)
