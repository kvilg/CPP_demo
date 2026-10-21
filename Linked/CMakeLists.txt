cmake_minimum_required(VERSION 4.0)
project(Linked)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Проект собирается и как часть корневого CPP_demo, и сам по себе.
# В первом случае зависимости и цели с флагами уже готовы, во втором
# подключаем те же модули из ../cmake.
if(NOT TARGET doctest::doctest OR NOT TARGET project_warnings)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
endif()
if(NOT TARGET doctest::doctest)
    include(dependencies)
endif()
if(NOT TARGET project_warnings)
    include(CompilerOptions)
endif()

# Тесты живут в List.cpp. Здесь они вырезаются препроцессором:
# DOCTEST_CONFIG_DISABLE раскрывает TEST_CASE в ничего, поэтому в
# бинарник не попадает ни тестовый код, ни main() из doctest.
# Заголовок doctest всё равно нужно найти — отсюда doctest::doctest.
add_executable(Linked main.cpp
        List.cpp
        List.h)
target_compile_definitions(Linked PRIVATE DOCTEST_CONFIG_DISABLE)
target_link_libraries(Linked PRIVATE
        doctest::doctest
        project_warnings)

# --- демонстрация: цель, которая намеренно не линкуется ---
#
# main_double.cpp берёт List<double>, а в List.cpp инстанцирован только
# List<int>. Компиляция пройдёт, линковка упадёт — это и надо показать.
#
# EXCLUDE_FROM_ALL обязателен: без него обычная сборка всего проекта
# всегда была бы красной. Собирать руками и только её:
#     cmake --build --preset debug --target Linked_double
add_executable(Linked_double EXCLUDE_FROM_ALL main_double.cpp
        List.cpp
        List.h)
target_compile_definitions(Linked_double PRIVATE DOCTEST_CONFIG_DISABLE)
target_link_libraries(Linked_double PRIVATE
        doctest::doctest
        project_warnings)

# --- тесты ---

enable_testing()

# Тот же List.cpp, но без DOCTEST_CONFIG_DISABLE, значит с тестами.
# main.cpp не нужен: main() приходит из doctest_with_main.
add_executable(Linked_tests List.cpp)
target_link_libraries(Linked_tests PRIVATE
        doctest::doctest_with_main
        project_warnings
        project_coverage)

# doctest_discover_tests регистрирует каждый TEST_CASE отдельным тестом ctest
include("${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake")
doctest_discover_tests(Linked_tests)

# Все три цели в одну папку дерева проекта IDE
set_target_properties(Linked Linked_tests Linked_double PROPERTIES FOLDER "Linked")
