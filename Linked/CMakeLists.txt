cmake_minimum_required(VERSION 4.0)
project(Linked)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(get_cpm)

CPMAddPackage(
        NAME doctest
        GITHUB_REPOSITORY doctest/doctest
        VERSION 2.5.3
        OPTIONS
        "DOCTEST_WITH_TESTS OFF"
        "DOCTEST_NO_INSTALL ON"
)

# Тесты живут в List.cpp. Здесь они вырезаются препроцессором:
# DOCTEST_CONFIG_DISABLE раскрывает TEST_CASE в ничего, поэтому в
# бинарник не попадает ни тестовый код, ни main() из doctest.
# Заголовок doctest всё равно нужно найти — отсюда doctest::doctest.
add_executable(Linked main.cpp
        List.cpp
        List.h)
target_compile_definitions(Linked PRIVATE DOCTEST_CONFIG_DISABLE)
target_link_libraries(Linked PRIVATE doctest::doctest)

# --- тесты ---

enable_testing()

# Тот же List.cpp, но без DOCTEST_CONFIG_DISABLE, значит с тестами.
# main.cpp не нужен: main() приходит из doctest_with_main.
add_executable(Linked_tests List.cpp)
target_link_libraries(Linked_tests PRIVATE doctest::doctest_with_main)

# doctest_discover_tests регистрирует каждый TEST_CASE отдельным тестом ctest
include("${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake")
doctest_discover_tests(Linked_tests)
