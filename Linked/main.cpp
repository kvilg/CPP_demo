// Демонстрация двусвязного списка.
//
// В консоль пишем по-русски и с временем: сообщения собирает console: из
// common/Console.h. Он же переключает консоль на UTF-8 — без этого
// кириллица выводится мусором: исходники в UTF-8, а консоль Windows
// по умолчанию в CP866.
//
// Показываем только то, что уже реализовано. Остальные методы (isEmpty,
// clear, insert, sort и прочие) — это упражнения: они объявлены, но тела
// ещё нет, и вызов бросает std::logic_error. Список задач — в README.md.

#include <cstddef>
#include <exception>
#include <sstream>
#include <string>

#include "Console.h"
#include "List.h"

namespace {

// Содержимое списка одной строкой: "1, 2, 3".
template <typename T>
std::string toRow(List<T>& list) {
    std::ostringstream row;
    for (std::size_t i = 0; i < list.getSize(); ++i) {
        if (i != 0) {
            row << ", ";
        }
        row << list.getValue(i);
    }
    return row.str();
}

// Печать списка с подписью.
template <typename T>
void printList(const char* title, List<T>& list) {
    console::info(title, ": ", toRow(list));
}

}  // namespace

int main() {

    List<int> list123;
    list123.addLast(1);
    list123.addLast(2);
    list123.addLast(3);

    int sum = 0;
    for (int x : list123) {
        sum += x;
        console::info(x);
    }

    console::info(sum);

    console::info("=== Демонстрация двусвязного списка ===");

    List<int> list;
    console::info("Создан пустой список, размер: ", list.getSize());

    list.addLast(1);
    list.addLast(2);
    list.addLast(3);
    list.addLast(4);
    printList("После четырёх addLast", list);

    list.addStart(0);
    printList("После addStart(0)", list);

    console::info("Размер списка: ", list.getSize());

    // Копия глубокая: узлы свои, значения те же. Меняем оригинал — копия
    // от этого не меняется.
    List<int> copy(list);
    console::info("Создана копия, размер копии: ", copy.getSize());

    list.setValue(0, 44);
    printList("Оригинал после setValue(0, 44)", list);
    printList("Копия (не изменилась)", copy);

    // Присваивание: содержимое list переезжает в assigned.
    List<int> assigned;
    assigned = list;
    printList("После присваивания assigned = list", assigned);

    // swap меняет узлы местами, значения не копируются.
    List<int> left;
    left.addLast(10);
    List<int> right;
    right.addLast(20);
    left.swap(right);
    printList("После swap: left", left);
    printList("После swap: right", right);

    // Заглушки бросают std::logic_error — показываем, как это выглядит
    // в логе.
    try {
        list.at(0);
    } catch (const std::exception& error) {
        console::warning("at(0) пока не реализован: ", error.what());
    }

    console::info("=== Готово ===");
    return 0;
}
