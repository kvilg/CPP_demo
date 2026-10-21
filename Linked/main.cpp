#include <iostream>

#include "List.h"

int main() {
    List<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);
    list.addLast(4);

    List<int> copy(list);

    std::cout << list.getValue(0) << '\n';
    std::cout << copy.getValue(0) << '\n';

    list.setValue(0, 44);
    std::cout << list.getValue(0) << '\n';
    std::cout << copy.getValue(0) << '\n';

    return 0;
}
