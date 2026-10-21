cmake_minimum_required(VERSION 3.21)

project(TcpServer
    VERSION 1.0.0
    DESCRIPTION "Учебный TCP-сервер и клиент на Qt Network"
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
# Core достаточно для сервера: без QtQuick и графики.
# Network даёт QTcpServer / QTcpSocket.
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

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Network)

# ---------------------------------------------------------------------------
# Цель
# ---------------------------------------------------------------------------
# Всё в одном файле: никаких классов, только QTcpServer и QTcpSocket.
# Консольное приложение: WIN32_EXECUTABLE не ставим, чтобы был виден вывод.
qt_add_executable(TcpServer
    src/main.cpp
)

target_link_libraries(TcpServer PRIVATE
    Qt6::Core
    Qt6::Network
)

target_include_directories(TcpServer PRIVATE src)

# Общие предупреждения и санитайзеры, см. cmake/CompilerOptions.cmake
target_link_libraries(TcpServer PRIVATE project_warnings)

set_target_properties(TcpServer PROPERTIES
    WIN32_EXECUTABLE FALSE
    MACOSX_BUNDLE FALSE
)

# ---------------------------------------------------------------------------
# Деплой: копируем Qt DLL рядом с .exe
# ---------------------------------------------------------------------------
# Функция объявлена в cmake/qt.cmake: POST_BUILD windeployqt плюс цель
# deploy_TcpServer для ручного запуска.
cpp_demo_deploy_qt(TcpServer)
