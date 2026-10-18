# TcpServer — TCP echo server

Сервер принимает одного клиента, читает от него данные и отправляет то же
самое обратно. Один файл `src/main.cpp`.

---

## Запуск

```powershell
.\build.ps1 -Run
```

---

## Проверка

Открой второе окно PowerShell:

```powershell
$c = New-Object System.Net.Sockets.TcpClient
$c.Connect("127.0.0.1", 1234)
$s = $c.GetStream()

# отправить
$b = [System.Text.Encoding]::UTF8.GetBytes("привет`n")
$s.Write($b, 0, $b.Length)

# получить обратно
$buf = New-Object byte[] 1024
$n = $s.Read($buf, 0, 1024)
[System.Text.Encoding]::UTF8.GetString($buf, 0, $n)
```

Пока клиент не закрыл соединение, можно слать сколько угодно строк —
каждая вернётся. `Ctrl+C` в окне сервера — остановить.

---

## Как это работает

```cpp
QTcpServer server;
server.listen(QHostAddress(QHostAddress::Any), 1234);   // 1. занять порт

server.waitForNewConnection(30000);                       // 2. ждать клиента
QTcpSocket *client = server.nextPendingConnection();      //    забрать сокет

while (true) {                                            // 3. эхо
    if (client->waitForReadyRead(5000)) {
        client->write(client->readAll());
    }
    if (client->state() != QAbstractSocket::ConnectedState)
        break;                                            //    клиент ушёл
}
```

| Что | Зачем |
|---|---|
| `listen(QHostAddress::Any, 1234)` | слушать на всех интерфейсах |
| `waitForNewConnection(30000)` | блокирующее ожидание клиента, 30 секунд |
| `nextPendingConnection()` | забрать сокет подключившегося |
| `waitForReadyRead(5000)` | ждать данные до 5 секунд, потом проверить состояние |
| `readAll()` | забрать всё, что пришло |
| `state() != ConnectedState` | признак, что клиент отключился |
| `Ctrl+C` | в консоли прерывает `waitFor*` и прерывает программу |

---

## Замечания

**Клиент один.** Как только он отключится, сервер завершится.
Для многих клиентов нужен цикл: принимать всех, читать у каждого и
рассылать остальным.

**Таймаут 5000 мс** — это не «ждать данные», а «если за 5 секунд тишина,
проверить, не отключился ли клиент». Если бы блокировка была вечной,
сервер не заметил бы разрыв.

**Кодировка.** `SetConsoleOutputCP(CP_UTF8)` нужен, чтобы русский текст
в консоли не превращался в `╤ыє°р■`. При перенаправлении вывода в файл
Windows всё равно запишет в CP1251 — это её поведение, а не ошибка.

---

## Полезные ссылки

- [QTcpServer](https://doc.qt.io/qt-6/qtcpserver.html)
- [QTcpSocket](https://doc.qt.io/qt-6/qtcpsocket.html)
- [Серверный пример из документации Qt](https://doc.qt.io/qt-6/qtsocketserver.html)