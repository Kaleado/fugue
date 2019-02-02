#define DEBUG

#include <iostream>

#include <boost/program_options.hpp>

#include "BPlusTree.hpp"

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

    Fugue::BPlusTree<int, 3> t;
    int* data = new int(333);
    t.insert(2, data);
    t.insert(7, data);
    t.insert(9, data);
    t.insert(5, data);
    t.insert(10, data);
    t.insert(6, data);
    t.insert(8, data);
    t.insert(0, data);
    t.insert(11, data);
    t.dbgPrint();
    std::cout << "Value is " << *(int*)t.get(8) << "\n";
    std::cout << "Hello, World!" << std::endl;
    return 0;
}