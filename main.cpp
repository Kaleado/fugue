#include <iostream>
#include "BPlusTree.hpp"

int main() {
    Fugue::BPlusTree<int, 10> t;
    int* data = new int(1);
    t.insert(1, data);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}