# Консольный лог: общий заголовок для всех проектов репозитория.
#
# Console.h — header-only: переключает консоль Windows на UTF-8 при старте
# программы и печатает сообщения по-русски. Без UTF-8 русские сообщения
# выходят мусором: исходники в UTF-8, а консоль по умолчанию в CP866.
#
# Библиотека INTERFACE: компилировать нечего, нужно только добавить
# common/ в пути включения.

if(TARGET project_console)
    return()
endif()

add_library(project_console INTERFACE)
target_include_directories(project_console INTERFACE "${CMAKE_CURRENT_LIST_DIR}/../common")
