cmake_minimum_required(VERSION 4.0)

# Условие задачи, без решения. Собирается из корня и само по себе:
#
#   cmake -S CallbackScheduler -B CallbackScheduler/build
#   cmake --build CallbackScheduler/build
#
# В каталоге только main.cpp.

project(CallbackScheduler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(CallbackScheduler main.cpp)
target_compile_features(CallbackScheduler PRIVATE cxx_std_20)

if(MSVC)
    target_compile_options(CallbackScheduler PRIVATE /W4 /permissive- /WX /utf-8)
else()
    target_compile_options(CallbackScheduler PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

set_target_properties(CallbackScheduler PROPERTIES FOLDER "CallbackScheduler")
