//
// Created by kaleado on 18/02/19.
//

#ifndef FUGUE_EXPIRYCOMMAND_HPP
#define FUGUE_EXPIRYCOMMAND_HPP

#include "ExpirationManager.hpp"
#include "AbstractCommand.hpp"

namespace Fugue {

    template<class Key>
    class ExpiryCommand : public AbstractCommand<Key> {
    private:
        Key _key;
        std::chrono::duration<long> _expiresAfter;
    public:
        //! Execute the command in the context of the given underlying storage, writing the result to the buffer.
        void execute(AbstractKeyValueStore<Key>& store, ExpirationManager<Key>& expirationManager, ServerState& state, DataItem& buffer);

        explicit ExpiryCommand(Key key, std::chrono::duration<long> expiresAfter)
        : _key{key}, _expiresAfter{expiresAfter} {}
    };

    template<class Key>
    void ExpiryCommand<Key>::execute(AbstractKeyValueStore<Key>& store,
            ExpirationManager<Key>& expirationManager, ServerState& state, DataItem& buffer) {
#ifdef DEBUG
        std::cout << "Setting expiry for key " << _key << " to " << _expiresAfter.count() << "\n";
#endif
        expirationManager.addExpiringKey(_key,std::chrono::system_clock::now() + _expiresAfter);
        buffer.raw = new std::string("success");
        buffer.length = sizeof(std::string);
    }


}

#endif //FUGUE_EXPIRYCOMMAND_HPP
