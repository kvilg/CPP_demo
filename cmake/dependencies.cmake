# Внешние зависимости, общие для всех проектов репозитория.
# Qt живёт отдельно, в cmake/qt.cmake: он нужен только проектам из qt/.

include(get_cpm)

# doctest — модульные тесты.
# DOWNLOAD_ONLY не ставим, в отличие от Meristem: там оно нужно, потому что
# CMakeLists у doctest 2.4.11 требует cmake < 3.5, который CMake 3.30+ уже не
# принимает. В 2.5.3 там 3.14, а его CMake нам нужен — он даёт цель
# doctest_with_main и скрипт doctest_discover_tests.
CPMAddPackage(
        NAME doctest
        GITHUB_REPOSITORY doctest/doctest
        VERSION 2.5.3
        OPTIONS
        "DOCTEST_WITH_TESTS OFF"
        "DOCTEST_NO_INSTALL ON"
)
