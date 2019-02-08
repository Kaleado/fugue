//
// Created by kaleado on 5/02/19.
//

#ifndef FUGUE_SETCOMMAND_HPP
#define FUGUE_SETCOMMAND_HPP

#include "AbstractCommand.hpp"
#include "AbstractKeyValueStore.hpp"

namespace Fugue {

    template<class Key>
    class SetCommand : public AbstractCommand<Key> {
    private:
        Key _key;
        std::size_t _length;
    public:
        //! Execute the command in the context of the given underlying storage, writing the result to the buffer.
        void execute(AbstractKeyValueStore<Key>& store, ServerState& state, DataItem& buffer);

        explicit SetCommand(Key key, std::size_t& length) : _key{key}, _length{length} {}
    };

    template<class Key>
    void SetCommand<Key>::execute(AbstractKeyValueStore<Key> &store, ServerState& state, DataItem& buffer) {
        if(_length > state.maxValueSize) {
            return;
        }
        buffer.raw = new std::string("success");
        buffer.length = sizeof(std::string);
        state.status = ServerState::READY_BINARY;
        state.settingKey = _key;
        state.incomingBinaryLength = _length;
    }

}

#endif //FUGUE_SETCOMMAND_HPP
