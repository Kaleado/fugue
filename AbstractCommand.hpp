//
// Created by kaleado on 4/02/19.
//

#ifndef FUGUE_COMMAND_HPP
#define FUGUE_COMMAND_HPP

#include <string>
#include <boost/asio/streambuf.hpp>
#include "ConnectionManager.hpp"
#include "AbstractKeyValueStore.hpp"

namespace Fugue {

    namespace io = boost::asio;

    //! Abstract class representing a command from a client that can be executed on a given key value store.
    template <class KeyValueStore>
    class AbstractCommand {
    private:
    public:
        typedef std::shared_ptr<AbstractCommand> Ptr;

        //! Execute the command in the context of the given underlying storage, writing the result to the buffer.
        virtual void execute(KeyValueStore& store, io::streambuf& buffer)=0;

    };
}

#endif //FUGUE_COMMAND_HPP