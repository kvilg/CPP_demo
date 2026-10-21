cmake_minimum_required(VERSION 4.0)

project(CPP_demo
        DESCRIPTION "Учебные проекты: двусвязный список и два демо на Qt"
        LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Флаги компилятора. ПЕРВЫМ: санитайзеры задаются глобально и
# действуют только на цели, созданные после этого include —
# включая doctest, который приедет ниже через CPM.
include(CompilerOptions)

# CPM и doctest — один раз на все подпроекты
include(dependencies)

# Поиск Qt и функция раскладки его DLL
include(qt)

enable_testing()

add_subdirectory(Linked)

# qt/ собираются, если Qt нашёлся (поиск в cmake/qt.cmake).
# Отдельного флага нет: есть Qt — есть сборка.
if(Qt6_FOUND)
    add_subdirectory(qt/QtWidgetsDemo)
    add_subdirectory(qt/TcpServer)
endif()
