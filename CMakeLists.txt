cmake_minimum_required(VERSION 3.12)
project(fugue)

set(CMAKE_CXX_STANDARD 14)

add_executable(fugue main.cpp BPlusTree.cpp BPlusTree.hpp)