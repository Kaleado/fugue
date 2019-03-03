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
    ("listen-port", boost::program_options::value<unsigned short int>(), "set TCP port to listen on")
    ("clean-frequency", boost::program_options::value<unsigned int>(),
            "set the amount of time (in seconds) between each cache clean, or 0 to disable value expiration")
    ("max-value-size", boost::program_options::value<unsigned int>(), "set the maximum size (in bytes) for a value");
    boost::program_options::variables_map vars;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, description), vars);
    boost::program_options::notify(vars);

    Fugue::ServerConfiguration config;

    if(vars.count("help")) {
        std::cout << description << "\n";
        return 1;
    }

    if(vars.count("listen-port")){
        config.listenPort = vars["listen-port"].as<unsigned short int>();
    }

    if(vars.count("max-value-size")){
        config.maxValueSize = vars["max-value-size"].as<unsigned int>();
    }

    if(vars.count("clean-frequency")){
        config.cleanFrequency = vars["clean-frequency"].as<unsigned int>();
    }

    Fugue::BPlusTree<std::string, 3> t;

    Fugue::DataItem item;
    Fugue::ExpirationManager<std::string> expirationManager{std::chrono::seconds(config.cleanFrequency), t};
    expirationManager.start();
    Fugue::ConnectionManager cm{config, t, expirationManager};
    cm.startListening();

    item.free<std::string>();
    expirationManager.stop();

    return 0;
}