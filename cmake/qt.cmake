# Всё общее по Qt: где он лежит и как раскладывать его DLL рядом с .exe.
# Подключается корневым проектом и каждым из qt/* при отдельной сборке.

# --- поиск комплекта Qt ---
#
# Комплект обязан соответствовать компилятору, и подобрать его задним числом
# нельзя: CMAKE_CXX_COMPILER задаётся до первого project(). Поэтому ищем
# комплект под тот компилятор, который уже выбран:
#   MSVC        -> msvc2022_64
#   MinGW (g++) -> mingw_64
#
# Сначала обычный find_package: он подхватит CMAKE_PREFIX_PATH или Qt6_DIR,
# если путь задали снаружи. Только если не нашёл — ищем сами.
#
# Здесь запрашивается один Core, чтобы просто понять, есть ли Qt; свой
# список компонентов каждый проект объявляет сам.

if(MSVC)
    set(_qt_kit "msvc2022_64")
else()
    set(_qt_kit "mingw_64")
endif()

find_package(Qt6 6.5 QUIET COMPONENTS Core)

if(NOT Qt6_FOUND AND WIN32)
    file(GLOB _qt_candidates
            "C:/Qt/6*/${_qt_kit}"
            "D:/Qt/6*/${_qt_kit}"
            "$ENV{USERPROFILE}/Qt/6*/${_qt_kit}"
            "$ENV{LOCALAPPDATA}/Qt/6*/${_qt_kit}"
    )
    if(_qt_candidates)
        list(SORT _qt_candidates ORDER DESCENDING)   # новейшая версия первой
        list(GET _qt_candidates 0 _qt_path)
        list(APPEND CMAKE_PREFIX_PATH "${_qt_path}")
        message(STATUS "Qt: найден автоматически -> ${_qt_path}")
        find_package(Qt6 6.5 QUIET COMPONENTS Core)
    endif()
endif()

if(Qt6_FOUND)
    message(STATUS "Qt ${Qt6_VERSION}: ${Qt6_DIR}")
else()
    message(STATUS
            "Qt 6.5+ (${_qt_kit}) не найден, проекты из qt/ собираться не будут.\n"
            "   Указать путь вручную: -DCMAKE_PREFIX_PATH=C:/Qt/<версия>/${_qt_kit}")
endif()

# --- раскладка Qt DLL рядом с .exe ---
#
# Зачем: на Windows Qt лежит в C:\Qt, а не рядом с программой. Без этого шага
# запуск падает с кодом 0xC0000135 (STATUS_DLL_NOT_FOUND), то есть
# "Qt6Core.dll не найден".
#
# Делаем автоматически (POST_BUILD), чтобы обычный Run в любой IDE сразу давал
# рабочую программу. Повторный запуск windeployqt занимает доли секунды:
# файлы уже разложены и обновлять нечего.
#
# Плюс цель для ручного запуска и CI:
#   cmake --build <build-dir> --target deploy_QtWidgetsDemo
# При отдельной сборке проекта она дублируется коротким именем deploy.

function(cpp_demo_deploy_qt target)
    if(NOT WIN32)
        return()
    endif()

    # Qt6_DIR = <qt>/lib/cmake/Qt6  ->  <qt>/bin
    get_filename_component(_qt_bin_dir "${Qt6_DIR}/../../../bin" ABSOLUTE)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}" REQUIRED)

    set(_cmd "${WINDEPLOYQT_EXECUTABLE}" --no-translations "$<TARGET_FILE:${target}>")

    add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${_cmd}
            COMMENT "windeployqt: Qt-библиотеки рядом с ${target}.exe"
            VERBATIM
    )

    add_custom_target(deploy_${target}
            COMMAND ${_cmd}
            DEPENDS ${target}
            COMMENT "windeployqt для ${target} (явный запуск)"
            VERBATIM
    )

    # Короткое имя deploy — только когда проект собирается сам по себе:
    # в общей сборке оба qt-проекта создали бы цель с одним именем, и CMake
    # упал бы с "another target with the same name already exists".
    if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
        add_custom_target(deploy DEPENDS deploy_${target})
    endif()
endfunction()
