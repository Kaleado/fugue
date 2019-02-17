#define DEBUG

#include <iostream>

#include "gtest/gtest.h"
#include <boost/program_options.hpp>

#include "BPlusTree.hpp"
#include "ConnectionManager.hpp"
#include "CommandParser.hpp"
#include "DataItem.hpp"
#include "ExpirationManager.hpp"

int main(int argc, char** argv) {
    // Set up program options.
    boost::program_options::options_description description("Allowed options");
    description.add_options()
    ("help", "display this message")
    ("port", boost::program_options::value<int>(), "set TCP port to listen on");
    boost::program_options::variables_map vars;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, description), vars);
    boost::program_options::notify(vars);

    if(vars.count("help")) {
        std::cout << description << "\n";
        return 1;
    }

    Fugue::BPlusTree<std::string, 3> t;

    Fugue::DataItem item;
    Fugue::ExpirationManager<std::string> expirationManager{std::chrono::seconds(10), t};
    expirationManager.start();
    Fugue::ConnectionManager cm{Fugue::ServerConfiguration{}, t};
    t.insert("hello", new Fugue::DataItem(new std::string("goodbye"), sizeof(std::string)));
    expirationManager.addExpiringKey("hello", std::chrono::system_clock::now() + std::chrono::seconds(30));

    cm.startListening();

    item.free<std::string>();

    return 0;
}