cmake_minimum_required(VERSION 3.12)
project(fugue)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost COMPONENTS program_options)


if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(fugue main.cpp BPlusTree.cpp BPlusTree.hpp)
    target_link_libraries(fugue ${Boost_LIBRARIES})
endif()

