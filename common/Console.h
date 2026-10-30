#pragma once

// Консольный лог: русские сообщения в правильной кодировке.
//
// ЗАЧЕМ ЭТОТ ФАЙЛ
//
// Исходники репозитория в UTF-8, а консоль Windows по умолчанию в CP866.
// Байты UTF-8, прочитанные как CP866, превращаются в мусор: вместо «не»
// на экране «р®µ». Лечится одной строкой — SetConsoleOutputCP(CP_UTF8) —
// и делать её надо один раз при старте программы, до первого вывода.
//
// Файл header-only: подключил в любой .cpp — и кодировка уже настроена,
// отдельно ничего вызывать не надо. Подключать в нескольких .cpp одного
// проекта безопасно: переменная inline, конструктор у неё один на всю
// программу, поэтому SetConsoleOutputCP вызовется ровно один раз.
//
// ПРИМЕР
//
//     #include "Console.h"
//
//     console::info("Список создан, размер: ", list.getSize());
//     console::warning("Не удалось подключиться: ", error.what());
//     console::error("Файл не найден: ", path);
//
// Формат строки: [12:34:56] [ИНФО] сообщение.
// Ошибки пишутся в cerr, остальное — в cout.

#ifdef _WIN32
// Функции консоли объявляем сами, а не включаем <windows.h>: в заголовке
// он тянет за собой макросы (min, max, ERROR, small), которые ломают чужой
// код. Объявления ниже дословно повторяют kernel32 — см. wincon.h:
//
//     WINBASEAPI WINBOOL WINAPI SetConsoleOutputCP(UINT wCodePageID);
//     WINBASEAPI WINBOOL WINAPI SetConsoleCP(UINT wCodePageID);
//
// Если <windows.h> подключится в этом же файле позже, объявления
// совпадут и компилятор не сочтёт их разными.
extern "C" __declspec(dllimport) int __stdcall SetConsoleOutputCP(unsigned int codePage);
extern "C" __declspec(dllimport) int __stdcall SetConsoleCP(unsigned int codePage);
#endif

#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

namespace console {

// Уровни сообщений. Названия по-русски: весь лог по-русски.
enum class Level {
    Info,
    Warning,
    Error,
};

namespace detail {

// 65001 — это CP_UTF8.
inline constexpr unsigned int kUtf8CodePage = 65001;

// Переключает консоль на UTF-8.
//
// Вывод — главное: именно он виден в консоли. Ввод переключаем тоже:
// программа, которая читает русский текст, иначе споткнётся о кодировку.
// На других системах делать нечего: там кодировка UTF-8 и так.
inline void setUtf8CodePage() noexcept {
#ifdef _WIN32
    SetConsoleOutputCP(kUtf8CodePage);
    SetConsoleCP(kUtf8CodePage);
#endif
}

// Текущее время для префикса строки лога.
inline std::string currentTime() {
    const std::time_t now = std::time(nullptr);
    std::tm parts{};

#ifdef _WIN32
    // В Windows аргументы у localtime_s идут в обратном порядке, чем в POSIX.
    if (localtime_s(&parts, &now) != 0) {
        return "--:--:--";
    }
#else
    if (localtime_r(&now, &parts) != 0) {
        return "--:--:--";
    }
#endif

    char buffer[16]{};
    if (std::strftime(buffer, sizeof(buffer), "%H:%M:%S", &parts) == 0) {
        return "--:--:--";
    }
    return buffer;
}

// Русское название уровня для печати.
inline const char* levelName(Level level) noexcept {
    switch (level) {
        case Level::Info:
            return "ИНФО";
        case Level::Warning:
            return "ВНИМАНИЕ";
        case Level::Error:
            return "ОШИБКА";
    }
    return "СЛУЖЕБНО";
}

// Склеивает аргументы в строку: stream << первый << второй << ...
template <typename... Args>
std::string concat(Args&&... args) {
    std::ostringstream stream;
    (stream << ... << std::forward<Args>(args));
    return stream.str();
}

// Печатает готовую строку лога.
inline void write(Level level, const std::string& message) {
    // Ошибки — в cerr, остальное — в cout. Так stderr можно отделить от
    // stdout перенаправлением, а обычный лог — нет.
    std::ostream& out = (level == Level::Error) ? std::cerr : std::cout;
    out << '[' << currentTime() << "] [" << levelName(level) << "] " << message << '\n';
    out.flush();
}

// Переключает кодировку при старте программы.
struct Utf8OnStartup {
    Utf8OnStartup() {
        setUtf8CodePage();
    }
};

inline Utf8OnStartup utf8OnStartup;

}  // namespace detail

// Обычное сообщение.
template <typename... Args>
void info(Args&&... args) {
    detail::write(Level::Info, detail::concat(std::forward<Args>(args)...));
}

// Что-то пошло не так, но работа продолжается.
template <typename... Args>
void warning(Args&&... args) {
    detail::write(Level::Warning, detail::concat(std::forward<Args>(args)...));
}

// Ошибка: пишем в cerr.
template <typename... Args>
void error(Args&&... args) {
    detail::write(Level::Error, detail::concat(std::forward<Args>(args)...));
}

}  // namespace console
