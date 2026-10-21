# TcpServer — эмулятор сигнала

Приложение 1 из [задания](../README.md): генерирует сигнал и отдаёт его
по TCP. Ждёт одного клиента и раз в секунду шлёт ему число, по одному в
строке. Сам ничего не читает. Один файл `src/main.cpp`.

Рисует эти числа [QtWidgetsDemo](../QtWidgetsDemo) — по отдельности
программы бесполезны.

> **Сигнал пока не тот, что требуется.** Задание просит регулярный
> непрерывный сигнал, а здесь `std::rand() % 50` — шум. Из-за этого в
> регистраторе нечем управлять: у случайных чисел нет ни амплитуды, ни
> периода. Подробнее в [qt/README.md](../README.md).

---

## Запуск

Вместе с регистратором, из корня репозитория:

```powershell
.\build.ps1 -Demo
```

Только эмулятор:

```powershell
.\build.ps1 -Run TcpServer
```

---

## Проверка без регистратора

Открой второе окно PowerShell:

```powershell
$c = New-Object System.Net.Sockets.TcpClient
$c.Connect("127.0.0.1", 1234)
$r = New-Object System.IO.StreamReader($c.GetStream())
while ($true) { $r.ReadLine() }
```

Раз в секунду будет появляться число от 0 до 49. `Ctrl+C` — выйти.

---

## Как это работает

```cpp
QTcpServer server;
server.listen(QHostAddress(QHostAddress::Any), 1234);   // 1. занять порт

server.waitForNewConnection(30000);                     // 2. ждать клиента
QTcpSocket* client = server.nextPendingConnection();    //    забрать сокет

while (true) {                                          // 3. слать числа
    const QByteArray out = QByteArray::number(std::rand() % 50) + "\n";
    if (client->state() == QAbstractSocket::ConnectedState) {
        client->write(out);
        client->waitForBytesWritten(100);
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
```

| Что | Зачем |
|---|---|
| `listen(QHostAddress::Any, 1234)` | слушать на всех интерфейсах |
| `waitForNewConnection(30000)` | блокирующее ожидание клиента, 30 секунд |
| `nextPendingConnection()` | забрать сокет подключившегося |
| `QByteArray::number(...) + "\n"` | и есть весь протокол: число текстом, перевод строки — разделитель |
| `waitForBytesWritten(100)` | дождаться отправки, но не дольше 100 мс |
| `Ctrl+C` | остановить сервер |

Цикла событий Qt здесь нет: `QCoreApplication` создаётся, но `exec()` не
вызывается, всё держится на блокирующих `waitFor*`. Для такой маленькой
программы это допустимо, но событийная версия на `QTimer` и сигналах —
то, ради чего обычно и берут Qt.

---

## Что здесь сломано

Это задачи, а не оправдания — всё перечисленное стоит починить.

**Из цикла нельзя выйти.** `while (true)` без `break`, поэтому
`client->close()`, `qInfo("Готово.")` и `return 0` в конце файла
недостижимы. Остановить можно только `Ctrl+C`.

**Разрыв соединения не замечается.** Проверка состояния стоит только
вокруг записи: клиент ушёл, а цикл продолжает крутиться и спать.

**`std::rand()` без `srand`** даёт одну и ту же последовательность при
каждом запуске; на графике это заметно. И это устаревший API —
современный ответ `<random>`.

---

## Замечания

**Клиент один.** `nextPendingConnection()` вызывается ровно раз, до
цикла, так что сокет достаётся первому подключившемуся. Второй клиент
соединится и будет молча ждать. Задание нескольких клиентов не требует,
но в список работ это внесено — см. [qt/README.md](../README.md).

**Кодировка.** `SetConsoleOutputCP(CP_UTF8)` нужен, чтобы русский текст
в консоли не превращался в `╤ыє°р■`. При перенаправлении вывода в файл
Windows всё равно запишет в CP1251 — это её поведение, а не ошибка.

---

## Полезные ссылки

- [QTcpServer](https://doc.qt.io/qt-6/qtcpserver.html)
- [QTcpSocket](https://doc.qt.io/qt-6/qtcpsocket.html)
- [QTimer](https://doc.qt.io/qt-6/qtimer.html) — для событийной версии
