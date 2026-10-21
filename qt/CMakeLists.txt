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
