#pragma once

// Свой обработчик сообщений Qt: чтобы лог был в консоли и по-русски.
//
// ЗАЧЕМ
//
// 1. На Windows Qt отправляет qInfo/qWarning в отладчик (OutputDebugString),
//    а не в консоль: в окне консоли пусто, сообщения видны только в
//    отладчике Visual Studio. Свой обработчик печатает их сам — в консоль.
//
// 2. Кодировку Qt выбирает по локали системы: qPrintable отдаёт toLocal8Bit,
//    то есть байты в кодировке ANSI (у нас CP1251). Русский текст в консоли
//    Windows от этого превращается в мусор или в вопросительные знаки.
//    Наш обработчик печатает через console: — то есть в UTF-8, а UTF-8 для
//    консоли уже переключил Console.h при старте программы.
//
// ПРИМЕР
//
//     #include "QtConsole.h"
//
//     int main(int argc, char* argv[]) {
//         qtlog::install();
//         ...
//     }
//
// После этого qInfo, qWarning и сообщения самого Qt идут в консоль в том
// же формате, что и лог из common/Console.h: [12:34:56] [ИНФО] сообщение.

#include <QtCore/qlogging.h>
#include <QtCore/qstring.h>

#include <cstdlib>

#include "Console.h"

namespace qtlog {

inline void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& message) {
    // Файл и строка в лог не попадают: сообщение о работе программы важнее
    // места, где оно возникло.
    (void)context;

    // Именно qUtf8Printable, а не qPrintable: первый даёт UTF-8, второй —
    // байты в локали, и русский текст в них не выживает.
    const char* text = qUtf8Printable(message);

    switch (type) {
        case QtDebugMsg:
        case QtInfoMsg:
            console::info(text);
            break;
        case QtWarningMsg:
            console::warning(text);
            break;
        case QtCriticalMsg:
            console::error(text);
            break;
        case QtFatalMsg:
            // Фатальная ошибка: печатаем и сразу останавливаемся, как это
            // делает обработчик Qt по умолчанию.
            console::error(text);
            std::abort();
            break;
        default:
            console::info(text);
            break;
    }
}

// Подменить обработчик сообщений Qt. Звать нужно до первого сообщения,
// то есть в самом начале main.
inline void install() {
    qInstallMessageHandler(messageHandler);
}

}  // namespace qtlog
