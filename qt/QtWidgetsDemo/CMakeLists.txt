# Конфигурируется только из корня репозитория: общие настройки приходят
# из ../CMakeLists.txt, а функции и флаги — из cmake/.
if(NOT COMMAND cpp_demo_qt_app)
    message(FATAL_ERROR "Конфигурируй из корня репозитория: cmake --preset debug")
endif()

project(QtWidgetsDemo
    VERSION 1.0.0
    DESCRIPTION "Тот же интерфейс, что QtDemo, но на Qt Widgets"
    LANGUAGES CXX
)

# Widgets вместо Quick: QMainWindow, QVBoxLayout, QPushButton.
# Network даёт QTcpSocket (в src/main.cpp клиент подключается к серверу).
# Charts даёт QChart/QLineSeries — готовую отрисовку графика сигнала.
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets Network Charts)

# Файлы перечислены явно, а не через GLOB: добавленный .cpp должен попадать
# в сборку сразу, иначе придётся вручную сбрасывать кэш CMake.
#
# main.cpp     — сборка окна и работа с сокетом
# RealTimePlot — виджет графика
qt_add_executable(QtWidgetsDemo
    src/main.cpp
    src/RealTimePlot.cpp
    src/RealTimePlot.h
)

target_link_libraries(QtWidgetsDemo PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Charts
)

# src в путях включения, общие предупреждения, консольное приложение
# и windeployqt — всё в cpp_demo_qt_app, см. cmake/qt.cmake
cpp_demo_qt_app(QtWidgetsDemo)

# Ищем файл от корня проекта, в qrc кладём без папки resources:
# resources/images/app-icon.svg -> ":/images/app-icon.svg"
qt_add_resources(QtWidgetsDemo "resources"
    PREFIX "/"
    BASE "${CMAKE_CURRENT_SOURCE_DIR}/resources"
    FILES
        resources/images/app-icon.svg
)

set_target_properties(QtWidgetsDemo PROPERTIES MACOSX_BUNDLE TRUE)
