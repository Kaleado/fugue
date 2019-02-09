//
// Created by kaleado on 6/02/19.
//

#define DEBUG

#include <iostream>

#include "gtest/gtest.h"
#include <boost/program_options.hpp>

#include "BPlusTree.hpp"
#include "ConnectionManager.hpp"
#include "CommandParser.hpp"
#include "DataItem.hpp"

TEST(BPlusTreeTest, IntKeyInsertionSuccess) {
    Fugue::BPlusTree<int, 3> tree;
    auto* data = new Fugue::DataItem(new int(111), sizeof(int));
    tree.insert(2, data);
    tree.insert(7, data);
    tree.insert(9, data);
    tree.insert(5, data);
    tree.insert(10, data);
    tree.insert(6, data);
    tree.insert(8, data);
    tree.insert(0, data);
    tree.insert(11, data);
    tree.dbgPrint();
    //Assert we can get the item we just inserted.
    ASSERT_NE(tree.get(2), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get(2))->get<int>(), 111);

    ASSERT_NE(tree.get(7), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get(7))->get<int>(), 111);

    ASSERT_NE(tree.get(9), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get(9))->get<int>(), 111);

    ASSERT_NE(tree.get(5), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get(5))->get<int>(), 111);

    ASSERT_NE(tree.get(10), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get(10))->get<int>(), 111);

    ASSERT_NE(tree.get(6), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get(6))->get<int>(), 111);

    ASSERT_NE(tree.get(8), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get(8))->get<int>(), 111);

    ASSERT_NE(tree.get(0), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get(0))->get<int>(), 111);

    ASSERT_NE(tree.get(11), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get(11))->get<int>(), 111);
}

TEST(BPlusTreeTest, StringKeyInsertionSuccess) {
    Fugue::BPlusTree<std::string, 3> tree;
    auto* data1 = new Fugue::DataItem(new int(111), sizeof(int));
    auto* data2 = new Fugue::DataItem(new int(222), sizeof(int));
    auto* data3 = new Fugue::DataItem(new int(333), sizeof(int));
    tree.insert("two", data1);
    tree.insert("seven", data2);
    tree.insert("nine", data3);
    tree.insert("five", data1);
    tree.insert("ten", data2);
    tree.insert("six", data3);
    tree.insert("eight", data1);
    tree.insert("zero", data2);
    tree.insert("eleven", data3);
    tree.dbgPrint();
    //Assert we can get the item we just inserted.
    ASSERT_NE(tree.get("two"), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get("two"))->get<int>(), 111);

    ASSERT_NE(tree.get("seven"), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get("seven"))->get<int>(), 222);

    ASSERT_NE(tree.get("nine"), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get("nine"))->get<int>(), 333);

    ASSERT_NE(tree.get("five"), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get("five"))->get<int>(), 111);

    ASSERT_NE(tree.get("ten"), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get("ten"))->get<int>(), 222);

    ASSERT_NE(tree.get("six"), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get("six"))->get<int>(), 333);

    ASSERT_NE(tree.get("eight"), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get("eight"))->get<int>(), 111);

    ASSERT_NE(tree.get("zero"), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get("zero"))->get<int>(), 222);

    ASSERT_NE(tree.get("eleven"), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get("eleven"))->get<int>(), 333);
}

TEST(BPlusTreeTest, StringKeyGetMissingShouldBeNull) {
    Fugue::BPlusTree<std::string, 3> tree;
    auto* data1 = new Fugue::DataItem(new int(111), sizeof(int));
    auto* data2 = new Fugue::DataItem(new int(222), sizeof(int));
    auto* data3 = new Fugue::DataItem(new int(333), sizeof(int));
    tree.insert("two", data1);
    tree.insert("seven", data2);
    tree.insert("nine", data3);
    tree.insert("five", data1);
    tree.insert("ten", data2);
    tree.insert("six", data3);
    tree.insert("eight", data1);
    tree.insert("zero", data2);
    tree.insert("eleven", data3);
    tree.dbgPrint();

    //Assert we can't get items with invalid keys.
    ASSERT_EQ(tree.get("tw"), nullptr);
    ASSERT_EQ(tree.get("sevensevenseven"), nullptr);
    ASSERT_EQ(tree.get("nine99"), nullptr);
    ASSERT_EQ(tree.get("five00"), nullptr);
    ASSERT_EQ(tree.get("tenaa"), nullptr);
    ASSERT_EQ(tree.get("s"), nullptr);
    ASSERT_EQ(tree.get("eght"), nullptr);
    ASSERT_EQ(tree.get("zer"), nullptr);
    ASSERT_EQ(tree.get("elevenaa"), nullptr);
    ASSERT_EQ(tree.get("twoseven"), nullptr);
}

TEST(BPlusTreeTest, StringKeyGetInvalidShouldBeNull) {
    Fugue::BPlusTree<std::string, 3> tree;
    auto* data1 = new Fugue::DataItem(new int(111), sizeof(int));
    auto* data2 = new Fugue::DataItem(new int(222), sizeof(int));
    auto* data3 = new Fugue::DataItem(new int(333), sizeof(int));
    tree.insert("two", data1);
    tree.insert("seven", data2);
    tree.insert("nine", data3);
    tree.insert("five", data1);
    tree.insert("ten", data2);
    tree.insert("six", data3);
    tree.insert("eight", data1);
    tree.insert("zero", data2);
    tree.insert("eleven", data3);
    tree.dbgPrint();

    //Assert we can't get items with invalid keys.
    ASSERT_EQ(tree.get(""), nullptr);
    ASSERT_EQ(tree.get(" "), nullptr);
    ASSERT_EQ(tree.get("\0"), nullptr);
    ASSERT_EQ(tree.get("\0two\0"), nullptr);

}

TEST(BPlusTreeTest, IntKeyRemovalSuccess) {
    Fugue::BPlusTree<int, 3> tree;
    auto *data1 = new Fugue::DataItem(new int(111), sizeof(int));
    tree.insert(2, data1);
    tree.remove(2);

    ASSERT_EQ(tree.get(2), nullptr);
}

TEST(BPlusTreeTest, IntKeyInsertFewKeys) {
    int num = 5;
    Fugue::BPlusTree<int, 3> tree;
    auto* data1 = new Fugue::DataItem(new int(111), sizeof(int));
    for(int i = 0; i < num; ++i)
        tree.insert(i, data1);

    tree.dbgPrint();

    for(int i = 0; i < num; ++i){
        std::cout << i << " ";
        ASSERT_NE(tree.get(i), nullptr);
    }
}

TEST(BPlusTreeTest, IntKeyInsertMoreKeys) {
    int num = 10;
    Fugue::BPlusTree<int, 3> tree;
    auto* data1 = new Fugue::DataItem(new int(111), sizeof(int));
    for(int i = 0; i < num; ++i)
        tree.insert(i, data1);

    tree.dbgPrint();

    for(int i = 0; i < num; ++i){
        std::cout << i << " ";
        ASSERT_NE(tree.get(i), nullptr);
    }
}


TEST(BPlusTreeTest, IntKeyInsertManyKeys) {
    int num = 400;
    Fugue::BPlusTree<int, 3> tree;
    auto* data1 = new Fugue::DataItem(new int(111), sizeof(int));
    for(int i = 0; i < num; ++i)
        tree.insert(i, data1);

    tree.dbgPrint();

    for(int i = 0; i < num; ++i){
        std::cout << i << " ";
        ASSERT_NE(tree.get(i), nullptr);
    }
}

//! Test fixture class for BPlusNodes with integer keys.
class BPlusNodeIntKeyTest {
private:
    Fugue::BPlusNode<int, 3>& _n;
public:
    std::array<int, 4>& keys;
    int& currentSize;
    template <int size>
    void rightShiftArray(std::array<int, size>& arr, int idx, int shiftBy){_n._rightShiftArray<int, size>(arr, idx, shiftBy);}
    int positionFor(int k){ return _n._positionFor(k); }
    explicit BPlusNodeIntKeyTest(Fugue::BPlusNode<int, 3> n) : _n{n}, keys{n._keys}, currentSize{n._currentSize} {}
};

TEST(BPlusNodeTest, PositionForInnerNode) {
    Fugue::BPlusNode<int, 3> node{nullptr, false, nullptr, nullptr, nullptr};
    BPlusNodeIntKeyTest n{node};
    n.keys = {1,4,7};
    n.currentSize = 3;

    ASSERT_EQ(n.positionFor(2), 1);
    ASSERT_EQ(n.positionFor(0), 0);
    ASSERT_EQ(n.positionFor(1), 1);
    ASSERT_EQ(n.positionFor(4), 2);
    ASSERT_EQ(n.positionFor(7), 3);

    n.keys = {6};
    n.currentSize = 1;

    ASSERT_EQ(n.positionFor(11), 1);
}

TEST(BPlusNodeTest, PositionForLeaf) {
    Fugue::BPlusNode<int, 3> node{nullptr, true, nullptr, nullptr, nullptr};
    BPlusNodeIntKeyTest n{node};
    n.keys = {1,4,7};
    n.currentSize = 3;

    ASSERT_EQ(n.positionFor(2), 1);
    ASSERT_EQ(n.positionFor(0), 0);
    ASSERT_EQ(n.positionFor(1), 0);
    ASSERT_EQ(n.positionFor(4), 1);
    ASSERT_EQ(n.positionFor(7), 2);
}


TEST(BPlusNodeTest, RightShiftArray) {
    Fugue::BPlusNode<int, 3> node{nullptr, true, nullptr, nullptr, nullptr};
    BPlusNodeIntKeyTest n{node};
    n.keys = {1,7,0,0};
    n.currentSize = 2;

    n.rightShiftArray<4>(n.keys, 1, 1);
    std::array<int, 4> res = {1,7,7,0};
    ASSERT_EQ(n.keys, res);

    n.keys = {1,7,0,0};
    n.rightShiftArray<4>(n.keys, 0, 1);
    res = {1,1,7,0};
    ASSERT_EQ(n.keys, res);

    n.keys = {1,7,0,0};
    n.rightShiftArray<4>(n.keys, 0, 1);
    res = {1,1,7,0};
    ASSERT_EQ(n.keys, res);
}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}
