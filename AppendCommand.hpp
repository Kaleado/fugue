//
// Created by kaleado on 12/02/19.
//

#ifndef FUGUE_APPENDCOMMAND_HPP
#define FUGUE_APPENDCOMMAND_HPP

#include "AbstractCommand.hpp"

namespace Fugue {

    template<class Key>
    class AppendCommand : public AbstractCommand<Key> {
    private:
        Key _key;
        std::size_t _length;
    public:
        //! Execute the command in the context of the given underlying storage, writing the result to the buffer.
        void execute(AbstractKeyValueStore<Key>& store, ExpirationManager<Key>& expirationManager, ServerState& state, DataItem& buffer);

        explicit AppendCommand(Key key, std::size_t& length) : _key{key}, _length{length} {}

    };

    template<class Key>
    void AppendCommand<Key>::execute(AbstractKeyValueStore<Key> &store, ExpirationManager<Key>& expirationManager, ServerState& state, DataItem& buffer) {
        if(_length > state.maxTransferSize) {
            buffer.raw = nullptr;
            buffer.length = 0;
            state.status = ServerState::READY_TEXT;
            throw std::runtime_error("Value too large");
        }
        buffer.raw = new std::string("success");
        buffer.length = sizeof(std::string);
        state.status = ServerState::READY_APPEND_BINARY;
        state.settingKey = _key;
        state.incomingBinaryLength = _length;
    }

}

#endif //FUGUE_APPENDCOMMAND_HPP
