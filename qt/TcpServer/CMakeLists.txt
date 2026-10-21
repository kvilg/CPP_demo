# Конфигурируется только из корня репозитория: общие настройки приходят
# из ../CMakeLists.txt, а функции и флаги — из cmake/.
if(NOT COMMAND cpp_demo_qt_app)
    message(FATAL_ERROR "Конфигурируй из корня репозитория: cmake --preset debug")
endif()

project(TcpServer
    VERSION 1.0.0
    DESCRIPTION "Учебный TCP-сервер и клиент на Qt Network"
    LANGUAGES CXX
)

# Core достаточно для сервера: без QtQuick и графики.
# Network даёт QTcpServer / QTcpSocket.
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Network)

# Всё в одном файле: никаких классов, только QTcpServer и QTcpSocket.
qt_add_executable(TcpServer
    src/main.cpp
)

target_link_libraries(TcpServer PRIVATE
    Qt6::Core
    Qt6::Network
)

# src в путях включения, общие предупреждения, консольное приложение
# и windeployqt — всё в cpp_demo_qt_app, см. cmake/qt.cmake
cpp_demo_qt_app(TcpServer)

set_target_properties(TcpServer PROPERTIES MACOSX_BUNDLE FALSE)
