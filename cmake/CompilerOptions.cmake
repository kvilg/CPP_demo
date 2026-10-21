# Флаги компилятора, общие для всех проектов репозитория.
#
# Два разных способа, и разница принципиальная:
#
# Предупреждения и покрытие — INTERFACE-библиотеками, то есть только на
# своих целях. Глобально их ставить нельзя: мы бы чинили предупреждения
# doctest и мерили покрытие чужого кода.
#
#   target_link_libraries(МояЦель PRIVATE project_warnings)
#
# Санитайзеры — глобально, через add_compile_options. Они не про одну
# цель, а про весь бинарник: объектные файлы, собранные с ASan и без,
# не слинкуются. Подробности ниже, у самой опции.

add_library(project_warnings INTERFACE)

# --- предупреждения ---
#
# Сразу как ошибки: предупреждение, которое можно не заметить, рано или
# поздно станет багом. Сейчас репозиторий собирается без единого
# предупреждения, так что планка держится бесплатно.
#
# Если новая версия компилятора начнёт ругаться на старый код и нужно
# срочно собраться, перебить можно снаружи:
#   cmake --build build -- CXXFLAGS=-Wno-error

if(MSVC)
    target_compile_options(project_warnings INTERFACE /W4 /permissive- /WX)
else()
    target_compile_options(project_warnings INTERFACE -Wall -Wextra -Wpedantic -Werror)
endif()

# --- санитайзеры ---
#
# ASan/UBSan ловят то, что в List.cpp сделано руками: выход за границы,
# чтение освобождённой памяти, утечки, разыменование nullptr.
#
# По умолчанию ВЫКЛЮЧЕНЫ, и включать по умолчанию не стоит: на MinGW,
# которым тут собирают, рантаймов ASan вообще нет, так что дефолтная
# сборка просто перестала бы работать. Плюс код с санитайзерами и без —
# это разные бинарники, и гонять надо оба.
#
# Прогон с проверками:
#   cmake -S . -B build-asan -DCPP_DEMO_SANITIZERS=ON -DCMAKE_BUILD_TYPE=Debug

option(CPP_DEMO_SANITIZERS "Собрать с ASan/UBSan" OFF)

# Здесь, в отличие от предупреждений, флаги ставятся ГЛОБАЛЬНО, а не
# INTERFACE-целью. Санитайзер — свойство всего бинарника, а не отдельной
# цели: под /fsanitize=address MSVC включает аннотации в std::string и
# std::vector, и все объектные файлы обязаны о них договориться. Соберёшь
# свой код с ASan, а doctest из CPM без — и линковщик скажет
#   LNK2038: mismatch detected for 'annotate_string'
#
# Поэтому этот модуль подключается в корневом CMakeLists ДО dependencies:
# add_compile_options действует на цели, созданные после него.

if(CPP_DEMO_SANITIZERS)
    if(MSVC)
        # У MSVC есть только ASan, UBSan нет. И он несовместим с /RTC1,
        # который CMake кладёт в флаги Debug по умолчанию.
        add_compile_options(/fsanitize=address)
        # Иначе предупреждение LNK4300: с ASan инкрементальная линковка
        # всё равно отключается, но уже с руганью
        add_link_options(/INCREMENTAL:NO)
        string(REGEX REPLACE "/RTC[1csu]+" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" CACHE STRING "" FORCE)
        message(STATUS "Санитайзеры: MSVC ASan (UBSan у MSVC нет)")
    elseif(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # mingw-w64 не поставляет libasan/libubsan — флаг примется,
        # а линковка упадёт на неразрешённых символах. Лучше сказать сразу.
        message(FATAL_ERROR
            "CPP_DEMO_SANITIZERS=ON не работает с MinGW: в mingw-w64 нет "
            "рантаймов libasan/libubsan.\n"
            "Варианты:\n"
            "  - собрать проект компилятором MSVC (cl.exe) — там есть ASan;\n"
            "  - взять clang из C:/Qt/Tools/llvm-mingw*/bin;\n"
            "  - собрать под Linux/WSL, где ASan и UBSan есть у gcc и clang."
        )
    else()
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -g)
        add_link_options(-fsanitize=address,undefined)
        message(STATUS "Санитайзеры: ASan + UBSan")
    endif()
endif()
