//
// Created by kaleado on 4/02/19.
//

#ifndef FUGUE_COMMANDPARSER_HPP
#define FUGUE_COMMANDPARSER_HPP

#include "AbstractCommand.hpp"
#include "GetCommand.hpp"
#include "SetCommand.hpp"
#include "RemoveCommand.hpp"

namespace Fugue {

    //! Parses commands to be executed on a given key value store type.
    template <class Key>
    class CommandParser {
    private:
        typename AbstractCommand<Key>::Ptr _parseGet(std::vector<std::string> tokens);
        typename AbstractCommand<Key>::Ptr _parseSet(std::vector<std::string> tokens);
        typename AbstractCommand<Key>::Ptr _parseRemove(std::vector<std::string> tokens);

    public:
        typename AbstractCommand<Key>::Ptr parse(std::string str);
    };

    //! Parse a command of the format get <key> and return the command object.
    template<class Key>
    typename AbstractCommand<Key>::Ptr CommandParser<Key>::_parseGet(std::vector<std::string> tokens) {
        if(tokens.size() != 2)
            throw std::invalid_argument("Error parsing command: invalid number of arguments.");
        return std::make_shared<GetCommand<Key>>(tokens[1]);
    }

    //! Parse a command of the format set <key> <length> and return the command object.
    template<class Key>
    typename AbstractCommand<Key>::Ptr CommandParser<Key>::_parseSet(std::vector<std::string> tokens) {
        // For this command, we will follow the memcached practice of waiting for the next transmission to contain the
        // raw data.
        // Hence we assume tokens is indeed a vector of strings.
        if(tokens.size() != 3)
            throw std::invalid_argument("Error parsing command: invalid number of arguments.");
        std::size_t sz = static_cast<std::size_t>(std::stoi(tokens[2]));
        std::cout << sz << " sz\n";
        return std::make_shared<SetCommand<Key>>(tokens[1], sz);
    }

    template<class Key>
    typename AbstractCommand<Key>::Ptr CommandParser<Key>::_parseRemove(std::vector<std::string> tokens) {
        if(tokens.size() != 2)
            throw std::invalid_argument("Error parsing command: invalid number of arguments.");
        return std::make_shared<RemoveCommand<Key>>(tokens[1]);
    }

    template<class Key>
    typename AbstractCommand<Key>::Ptr CommandParser<Key>::parse(std::string str) {
        std::stringstream ss{str};
        std::vector<std::string> tokens;
        std::copy(std::istream_iterator<std::string>(ss),
             std::istream_iterator<std::string>(),
             std::back_inserter(tokens));
        if(tokens.empty())
            throw std::invalid_argument("Error parsing command: no tokens transmitted.");

        std::string& command = tokens[0];
        if(command == "get")
            return _parseGet(tokens);
        else if(command == "set")
            return _parseSet(tokens);
        else if(command == "remove")
            return _parseRemove(tokens);
        else
            throw std::invalid_argument("Error parsing command: invalid command.");
    }
}

#endif //FUGUE_COMMANDPARSER_HPP
