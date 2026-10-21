#include <iostream>

#include "List.h"

int main() {
    List<int> list;
    list.add_last(1);
    list.add_last(2);
    list.add_last(3);
    list.add_last(4);

    List<int> copy(list);

    std::cout << list.getValue(0) << '\n';
    std::cout << copy.getValue(0) << '\n';

    list.setValue(0, 44);
    std::cout << list.getValue(0) << '\n';
    std::cout << copy.getValue(0) << '\n';

    return 0;
}
