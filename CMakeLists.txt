cmake_minimum_required(VERSION 3.8)
project(fugue)

set(CMAKE_CXX_STANDARD 14)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread -Wall -std=c++14")

#Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

# Include Boost as well.
find_package(Boost COMPONENTS system program_options)

if(!Boost_FOUND)
    message(FATAL_ERROR "Unable to locate Boost")
endif()

include_directories(${Boost_INCLUDE_DIRS})
add_executable(fugue main.cpp BPlusTree.hpp ConnectionManager.cpp ConnectionManager.hpp ServerConfiguration.hpp AbstractKeyValueStore.hpp AbstractCommand.hpp CommandParser.hpp GetCommand.hpp DataItem.hpp SetCommand.hpp ServerState.hpp DataItem.cpp RemoveCommand.hpp AppendCommand.hpp ExpirationManager.hpp ExpiryCommand.hpp)
add_executable(test_fugue test.cpp BPlusTree.hpp ConnectionManager.cpp ConnectionManager.hpp ServerConfiguration.hpp AbstractKeyValueStore.hpp AbstractCommand.hpp CommandParser.hpp GetCommand.hpp DataItem.hpp SetCommand.hpp ServerState.hpp DataItem.cpp RemoveCommand.hpp AppendCommand.hpp ExpirationManager.hpp ExpiryCommand.hpp)
target_link_libraries(fugue ${Boost_LIBRARIES} gtest_main)
target_link_libraries(test_fugue ${Boost_LIBRARIES} gtest_main)

