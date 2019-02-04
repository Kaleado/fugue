//
// Created by kaleado on 4/02/19.
//

#ifndef FUGUE_COMMANDPARSER_HPP
#define FUGUE_COMMANDPARSER_HPP

#include "AbstractCommand.hpp"

namespace Fugue {

    //! Parses commands to be executed on a given key value store type.
    template <class KeyValueStore>
    class CommandParser {
    private:
        typename AbstractCommand<KeyValueStore>::Ptr _parseGet(std::vector<std::string> tokens);
        typename AbstractCommand<KeyValueStore>::Ptr _parseSet(std::vector<std::string> tokens);
        typename AbstractCommand<KeyValueStore>::Ptr _parseDelete(std::vector<std::string> tokens);

    public:
        typename AbstractCommand<KeyValueStore>::Ptr parse(std::string str);
    };
}

#endif //FUGUE_COMMANDPARSER_HPP
