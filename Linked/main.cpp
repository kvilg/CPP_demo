#include <iostream>
#include <stdexcept>

#include "List.h"

void test(){
    List<int> list;
    list.add_last(1);
    list.add_last(2);
    list.add_last(3);
    list.add_last(4);

    List<int> copy(list);

    if(list.getValue(0) != copy.getValue(0)){
        throw std::runtime_error("boom");;
    }
    list.setValue(0, 44);
    if(list.getValue(0) == copy.getValue(0)){
        throw std::runtime_error("boom");;
    }

}

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

    test();

    return 0;
}