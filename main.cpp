#define DEBUG

#include <iostream>

#include <boost/program_options.hpp>

#include "BPlusTree.hpp"
#include "ConnectionManager.hpp"
#include "CommandParser.hpp"
#include "DataItem.hpp"

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
    auto* data = new Fugue::DataItem();
    auto* str = new std::string("hello");
    data->raw = str;
    data->length = sizeof(str);
//    t.insert(2, data);
//    t.insert(7, data);
//    t.insert(9, data);
//    t.insert(5, data);
//    t.insert(10, data);
//    t.insert(6, data);
//    t.insert(8, data);
//    t.insert(0, data);
//    t.insert(11, data);
    t.insert("birthday", data);
    t.dbgPrint();
//    std::cout << "Value is " << *(int*)t.get(8) << "\n";

    Fugue::CommandParser<std::string> parser;
    Fugue::DataItem item;
    Fugue::ConnectionManager cm{Fugue::ServerConfiguration{}, t};
    auto cmd = parser.parse("get birthday");
    Fugue::ServerState fake;
    cmd->execute(t, fake, item);
    std::cout << item.raw << " " << item.length << "\n";
    std::cout << item.get<std::string>() << "\n";
    cm.startListening();
    item.free<std::string>();

    return 0;
}