// Сравнение double даёт partial_ordering, потому что NaN не равен ничему,
// даже себе. Поэтому возвращаемый тип operator<=> выводится через
// std::compare_three_way_result_t<T>, а не прибит к strong_ordering.

#include <iostream>

#include "List.h"

int main() {
    List<double> list;
    list.addLast(1.5);
    list.addLast(2.5);
    list.addStart(0.5);

    std::cout << "размер: " << list.getSize() << '\n';
    for (std::size_t i = 0; i < list.getSize(); ++i) {
        std::cout << list.getValue(i) << ' ';
    }
    std::cout << '\n';

    return 0;
}
