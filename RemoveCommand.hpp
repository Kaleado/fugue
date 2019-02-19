//
// Created by kaleado on 8/02/19.
//

#ifndef FUGUE_REMOVECOMMAND_HPP
#define FUGUE_REMOVECOMMAND_HPP

#include "AbstractCommand.hpp"
#include "AbstractKeyValueStore.hpp"

namespace Fugue {

    template<class Key>
    class RemoveCommand : public AbstractCommand<Key> {
    private:
        Key _key;
    public:
        //! Execute the command in the context of the given underlying storage, writing the result to the buffer.
        void execute(AbstractKeyValueStore<Key>& store, ExpirationManager<Key>& expirationManager, ServerState& state, DataItem& buffer);

        explicit RemoveCommand(Key key) : _key{key} {}

    };

    template<class Key>
    void RemoveCommand<Key>::execute(AbstractKeyValueStore<Key> &store, ExpirationManager<Key>& expirationManager, ServerState& state, DataItem& buffer) {
        store.remove(_key);
        //TODO: do not silently error, this should throw if the key doesn't exist.
        buffer.raw = new std::string("success");
        buffer.length = sizeof(std::string);
    }

}

#endif //FUGUE_REMOVECOMMAND_HPP
