# Общее для QtWidgetsDemo и TcpServer. Переменные уровня каталога
# наследуются подкаталогами, поэтому задаются здесь один раз, а не
# копируются в оба проекта.
#
# Стандарт тут не задаётся: берётся из корневого проекта, то есть C++20.
# Раньше оба проекта ставили себе 17 — Qt 6.5+ прекрасно работает и с 20,
# а два разных стандарта в одном репозитории только путают.

set(CMAKE_AUTOMOC ON)   # moc_*.cpp из Q_OBJECT
set(CMAKE_AUTORCC ON)   # qrc_*.cpp из .qrc

if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif()

add_subdirectory(QtWidgetsDemo)
add_subdirectory(TcpServer)

# Запуск связки целиком. По отдельности эти две программы бесполезны:
# сервер шлёт числа в пустоту, клиент рисует пустой график.
#
#   cmake --build --preset debug --target run_demo
#
# Сервер поднимается в отдельном окне консоли (start), поэтому цель не
# виснет на его бесконечном цикле и успевает запустить клиент. Секунда
# паузы — чтобы сервер занял порт раньше, чем клиент постучится.
#
# Это избавляет от двух копий IDE для запуска, но не для отладки:
# цель запускает программы без отладчика. Под отладкой по-прежнему
# нужны два запуска.
# Запускаем через Start-Process, а не через cmd /c start. У start первый
# аргумент в кавычках считается заголовком окна только пока его не удаётся
# понять иначе: рядом с рабочим каталогом цели лежит папка сборки
# TcpServer, и start открывал в проводнике её вместо запуска программы.
# У Start-Process такой двусмысленности нет, путь всегда путь.
if(WIN32)
    add_custom_target(run_demo
            COMMAND ${CMAKE_COMMAND} -E echo "Поднимаю TcpServer в отдельном окне..."
            COMMAND powershell -NoProfile -Command
                    "Start-Process -FilePath '$<TARGET_FILE:TcpServer>'"
            COMMAND ${CMAKE_COMMAND} -E sleep 1
            COMMAND ${CMAKE_COMMAND} -E echo "Запускаю QtWidgetsDemo..."
            COMMAND "$<TARGET_FILE:QtWidgetsDemo>"
            WORKING_DIRECTORY "$<TARGET_FILE_DIR:TcpServer>"
            USES_TERMINAL
            VERBATIM
    )
    add_dependencies(run_demo TcpServer QtWidgetsDemo)
    set_target_properties(run_demo PROPERTIES FOLDER "qt")
endif()
