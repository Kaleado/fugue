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

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}
