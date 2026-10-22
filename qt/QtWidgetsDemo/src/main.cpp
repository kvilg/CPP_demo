#include "RealTimePlot.h"

#include <QApplication>
#include <QColor>
#include <QColorDialog>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTcpSocket>

QT_USE_NAMESPACE

// --- чего здесь не хватает по заданию ---
//
// Для графического представления требуется набор стандартных
// инструментов управления просмотром. Из четырёх есть один:
//
//   - масштабирование — нет, ни рамкой (setRubberBand), ни колесом;
//   - толщина кривой — нет, хотя проводка готова: setCurveColor в
//     RealTimePlot берёт текущее перо и сохраняет ширину, не хватает
//     только pen.setWidth и контрола рядом с выбором цвета;
//   - цвет кривой — есть.
//
// Амплитуды и периода в этом списке нет намеренно. Это параметры
// генерации, и задаются они в эмуляторе — там для них и нужен
// интерфейс, см. комментарий в TcpServer/src/main.cpp. Регистратор
// рисует то, что пришло, и даёт разглядывать это мышкой; менять сам
// сигнал он не должен.
//
// QLineEdit("1") с именем test ниже создаётся, кладётся в панель и
// никуда не подключается — похоже, начатая заготовка под один из этих
// контролов.
//
// Полная сверка с заданием — в qt/README.md.

// --- упражнение: вынести окно в отдельный класс ---
//
// Всё, что написано ниже в main, должно переехать в
// MainWindow : public QMainWindow — в новые src/MainWindow.h и
// src/MainWindow.cpp:
//
//   - виджеты host, port, btn, log, plot, colorBtn и сокет s — в поля;
//   - сборку layout'ов — в конструктор;
//   - тела лямбд из QObject::connect — в приватные слоты;
//   - curveColor — в поле;
//   - resize(800, 500) — в конструктор, размер окна это дело окна.
//
// Здесь должно остаться только это:
//
//     int main(int argc, char* argv[]) {
//         QApplication app(argc, argv);
//         MainWindow w;
//         w.show();
//         return app.exec();
//     }
//
// В объявлении класса нужен Q_OBJECT, иначе слотов не будет; AUTOMOC
// для этого уже включён в qt/CMakeLists.txt. Новый .cpp дописать в
// qt_add_executable. Пример такого класса лежит рядом — RealTimePlot.
//
// Пока не вынесено, помни: лямбды ниже захватывают [&], то есть ссылки
// на локальные переменные main. Держится это только на том, что main не
// выходит, пока крутится app.exec(). Вынесешь кусок в функцию, не сделав
// его полем класса, — ссылки станут висячими.

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Путь внутрь ресурсов, а не в файловую систему: в CMakeLists указан
    // PREFIX "/" и BASE resources, поэтому resources/images/app-icon.svg
    // лежит под :/images/app-icon.svg. Иконка попадает в сам бинарник,
    // рядом с программой ничего класть не нужно.
    //
    // Ставим на QApplication, а не на окно: так её получат и диалоги,
    // например выбор цвета.
    //
    // SVG рисует подключаемый модуль qsvgicon, его раскладывает
    // windeployqt. Если он не доедет, QIcon промолчит и иконки просто
    // не будет — ошибки не покажут.
    app.setWindowIcon(QIcon(":/images/app-icon.svg"));

    QMainWindow w;
    auto* central = new QWidget;
    auto* lay = new QVBoxLayout(central);

    auto* host = new QLineEdit("127.0.0.1");
    auto* port = new QLineEdit("1234");
    auto* btn = new QPushButton("Подключиться");
    btn->setCursor(Qt::PointingHandCursor);
    auto* log = new QPlainTextEdit;
    log->setReadOnly(true);
    log->setMaximumHeight(100);

    auto* top = new QHBoxLayout;
    top->addWidget(host);
    top->addWidget(port);
    top->addWidget(btn);
    lay->addLayout(top);

    auto* plot = new RealTimePlot;
    lay->addWidget(plot, 1);

    auto* btnBox = new QHBoxLayout;
    auto* test = new QLineEdit("1");

    lay->addLayout(btnBox);

    auto* colorLabel = new QLabel("Цвет");
    auto* colorBtn = new QPushButton("Выбрать...");
    colorBtn->setToolTip("Цвет кривой — нажми, чтобы выбрать");
    colorBtn->setCursor(Qt::PointingHandCursor);

    btnBox->addWidget(colorLabel);
    btnBox->addWidget(colorBtn);
    btnBox->addWidget(test);

    QColor curveColor = Qt::cyan;
    plot->setCurveColor(curveColor);

    QObject::connect(colorBtn, &QPushButton::clicked, [&] {
        const QColor picked = QColorDialog::getColor(curveColor, colorBtn, "Цвет кривой");
        if (picked.isValid()) {
            curveColor = picked;
            plot->setCurveColor(picked);
        }
    });

    QTcpSocket s;
    QObject::connect(&s, &QTcpSocket::connected, [&] { btn->setText("Отключиться"); });
    QObject::connect(&s, &QTcpSocket::disconnected, [&] { btn->setText("Подключиться"); });
    QObject::connect(&s, &QTcpSocket::errorOccurred, [&] { btn->setText("Подключиться"); });

    QObject::connect(&s, &QTcpSocket::readyRead, [&] {
        const QByteArray data = s.readAll();
        log->appendPlainText(QString::fromUtf8(data));

        for (const QByteArray& line : data.split('\n')) {
            bool ok = false;
            double y = line.trimmed().toDouble(&ok);
            if (ok) {
                plot->addPoint(y);
            }
        }
    });

    QObject::connect(btn, &QPushButton::clicked, [&] {
        if (s.state() == QAbstractSocket::ConnectedState) {
            s.disconnectFromHost();
        } else {
            log->clear();
            plot->reset();
            s.connectToHost(host->text(), port->text().toUShort());
        }
    });

    w.setCentralWidget(central);
    w.resize(800, 500);
    w.show();
    return app.exec();
}
