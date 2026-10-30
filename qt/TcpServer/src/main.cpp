#include <QAbstractSocket>
#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>

#include <chrono>
#include <cstdlib>
#include <thread>

// Console.h переключает консоль на UTF-8 при старте программы: без этого
// русские сообщения qInfo/qWarning выходят мусором (исходники в UTF-8,
// а консоль Windows по умолчанию в CP866).
#include "Console.h"

// QtConsole.h переносит сообщения Qt из отладчика в консоль и печатает
// их в UTF-8 — иначе на Windows в консоли просто пусто.
#include "QtConsole.h"

int main(int argc, char* argv[]) {
    qtlog::install();

    QCoreApplication app(argc, argv);

    QTcpServer server;

    if (!server.listen(QHostAddress(QHostAddress::Any), 1234)) {
        qWarning("Не удалось занять порт 1234: %s", qPrintable(server.errorString()));
        return 1;
    }

    qInfo("Слушаю порт 1234. Подключись к 127.0.0.1:1234");

    if (!server.waitForNewConnection(30000)) {
        qInfo("Никто не подключился. Выход.");
        return 1;
    }

    QTcpSocket* client = server.nextPendingConnection();
    qInfo("Клиент подключился. Жду сообщения...");

    while (true) {

        const QByteArray out = QByteArray::number(std::rand() % 50) + "\n";
        if (client->state() == QAbstractSocket::ConnectedState) {
            client->write(out);
            client->waitForBytesWritten(100);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    client->close();
    qInfo("Готово.");
    return 0;
}
