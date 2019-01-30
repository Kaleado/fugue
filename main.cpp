#include <iostream>
#include "BPlusTree.hpp"

int main() {
    Fugue::BPlusTree<int, 3> t;
    int* data = new int(333);
    t.insert(2, data);
    t.insert(7, data);
    t.insert(9, data);
    t.insert(5, data);
    t.insert(10, data);
    t.insert(6, data);
    t.insert(8, data);
    t.insert(0, data);
    t.insert(11, data);
    t.dbgPrint();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}