# QtWidgetsDemo — тот же интерфейс, что на QML, но на C++

Весь интерфейс собран **целиком на виджетах** — один файл `src/main.cpp`,
без QML и без своих классов.

Ниже он сравнивается с проектом QtDemo, где то же самое сделано на QML.
Самого QtDemo в этом репозитории нет, он остался отдельно, так что
таблицу соответствий и раздел про отличия читай просто как «как это
выглядело бы на QML».

---

## Запуск

Из корня репозитория:

```powershell
.\build.ps1 -Run QtWidgetsDemo
```

Или напрямую через CMake:

```powershell
cmake --preset debug
cmake --build --preset debug
.\build\debug\bin\QtWidgetsDemo.exe
```

---

## Соответствие QML и Widgets

| QML | Widgets |
|---|---|
| `ApplicationWindow` | `QMainWindow` |
| `header: ToolBar` | `QWidget` в общем `QVBoxLayout` |
| `footer: ToolBar` | `statusBar()` |
| `ColumnLayout` | `QVBoxLayout` |
| `RowLayout` | `QHBoxLayout` |
| `Item { Layout.fillHeight: true }` | `addStretch(1)` |
| `Label { ... }` | `QLabel` + `setStyleSheet()` |
| `Button { onClicked: ... }` | `connect(&QPushButton::clicked, ...)` |
| `value: controller.counter` | лямбда, которая обновляет виджет |

---

## Три отличия от QML

**Layout не занимает место сам.** Виджет создаётся без родителя, а
`addWidget()` сам его вставляет. Забудешь вызвать — окно будет пустым,
но программа не упадёт.

**Привязок нет.** В QML `value: controller.counter` перерисовывалось само.
Здесь обновляем виджеты вручную из лямбды:

```cpp
auto refresh = [&] {
    counterValue->setText(QString::number(counter));
    status->setText("Счётчик: " + QString::number(counter));
};

QObject::connect(plus, &QPushButton::clicked, [&] { ++counter; refresh(); });
```

**Карточки — лямбда.** `makeCard()` собирает виджет с подписью и
значением. Вместо отдельного класса ради двух `QLabel` внутри `QFrame`.

---

## Что можно добавить

Класс `AppController` вместо переменной `int counter` — если захочешь
отделить логику от интерфейса, как в `QtDemo`.

Свой виджет вместо `makeCard()` — чтобы переиспользовать карточку
в нескольких окнах.

`QSplitter` — две панели, которые тянутся мышью.

---

## Полезные ссылки

- [Qt Widgets](https://doc.qt.io/qt-6/widgets-widgetclasses.html)
- [Layouts](https://doc.qt.io/qt-6/layout.html)
- [Стили виджетов](https://doc.qt.io/qt-6/stylesheet.html)
- [Сравнение Widgets и Quick](https://doc.qt.io/qt-6/widgets-vs-quick.html)
