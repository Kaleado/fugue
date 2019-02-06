cmake_minimum_required(VERSION 3.12)
project(fugue)

set(CMAKE_CXX_STANDARD 14)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

find_package(Boost COMPONENTS system program_options)


if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(fugue main.cpp BPlusTree.hpp ConnectionManager.cpp ConnectionManager.hpp ServerConfiguration.hpp AbstractKeyValueStore.hpp AbstractCommand.hpp CommandParser.hpp GetCommand.hpp DataItem.hpp SetCommand.hpp ServerState.hpp DataItem.cpp)
    target_link_libraries(fugue ${Boost_LIBRARIES})
endif()

