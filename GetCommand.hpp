//
// Created by kaleado on 4/02/19.
//

#ifndef FUGUE_GETCOMMAND_HPP
#define FUGUE_GETCOMMAND_HPP

#include "AbstractKeyValueStore.hpp"
#include "AbstractCommand.hpp"
#include "DataItem.hpp"

namespace Fugue {

    template<class Key>
    class GetCommand : public AbstractCommand<Key> {
    private:
        Key _key;
    public:
        //! Execute the command in the context of the given underlying storage, writing the result to the buffer.
        void execute(AbstractKeyValueStore<Key>& store, ServerState& state, DataItem& buffer);

        explicit GetCommand(Key key) : _key{key} {}
    };

    template<class Key>
    void GetCommand<Key>::execute(AbstractKeyValueStore<Key> &store, ServerState& state, DataItem& buffer) {
        auto* ptr = store.get(_key);
        if(!ptr) {
            return;
        }
        auto* data = static_cast<DataItem*>(ptr);
        buffer.raw = data->raw;
        buffer.length = data->length;
    }


}

#endif //FUGUE_GETCOMMAND_HPP
