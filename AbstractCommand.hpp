//
// Created by kaleado on 4/02/19.
//

#ifndef FUGUE_COMMAND_HPP
#define FUGUE_COMMAND_HPP

#include <string>
#include <boost/asio/streambuf.hpp>
#include "ConnectionManager.hpp"
#include "ExpirationManager.hpp"
#include "AbstractKeyValueStore.hpp"
#include "DataItem.hpp"

namespace Fugue {

    namespace io = boost::asio;

    //! Abstract class representing a command from a client that can be executed on a given key value store.
    template <class Key>
    class AbstractCommand {
    private:
    public:
        typedef std::shared_ptr<AbstractCommand<Key>> Ptr;

        //! Execute the command in the context of the given underlying storage, writing the result to the buffer.
        virtual void execute(AbstractKeyValueStore<Key>& store, ExpirationManager<Key>& expirationManager, ServerState& state, DataItem& buffer)=0;

        virtual ~AbstractCommand() = default;
    };
}

#endif //FUGUE_COMMAND_HPP