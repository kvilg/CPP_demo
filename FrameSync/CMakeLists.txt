cmake_minimum_required(VERSION 4.0)

# Условие задачи, без решения. Собирается из корня и само по себе:
#
#   cmake -S FrameSync -B FrameSync/build
#   cmake --build FrameSync/build
#
# В каталоге только main.cpp.

project(FrameSync LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(FrameSync main.cpp)
target_compile_features(FrameSync PRIVATE cxx_std_20)

find_package(Threads REQUIRED)
target_link_libraries(FrameSync PRIVATE Threads::Threads)

if(MSVC)
    target_compile_options(FrameSync PRIVATE /W4 /permissive- /WX /utf-8)
else()
    target_compile_options(FrameSync PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

set_target_properties(FrameSync PROPERTIES FOLDER "FrameSync")
