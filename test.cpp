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
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get(2))->get<int>(), 111);

    ASSERT_NE(tree.get(9), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get(2))->get<int>(), 111);

    ASSERT_NE(tree.get(5), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get(2))->get<int>(), 111);

    ASSERT_NE(tree.get(10), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get(2))->get<int>(), 111);

    ASSERT_NE(tree.get(6), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get(2))->get<int>(), 111);

    ASSERT_NE(tree.get(8), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get(2))->get<int>(), 111);

    ASSERT_NE(tree.get(0), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get(2))->get<int>(), 111);

    ASSERT_NE(tree.get(11), nullptr);
    ASSERT_EQ(static_cast<Fugue::DataItem*>(tree.get(2))->get<int>(), 111);
}


int main(int argc, char** argv){
    auto* data = new Fugue::DataItem();
    auto* str = new std::string("hello");
    data->raw = str;
    data->length = sizeof(str);
//    std::cout << "Value is " << *(int*)t.get(8) << "\n";

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}
