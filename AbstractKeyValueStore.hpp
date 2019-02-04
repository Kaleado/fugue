//
// Created by kaleado on 4/02/19.
//

#ifndef FUGUE_ABSTRACTKEYVALUESTORE_HPP
#define FUGUE_ABSTRACTKEYVALUESTORE_HPP

namespace Fugue {

    //! Abstract class representing any kind of underlying storage for a key-value store.
    template <class Key>
    class AbstractKeyValueStore {
    private:
    public:
        virtual void* get(Key k) const = 0;

        virtual void remove(Key k) = 0;

        virtual void insert(Key k, void* value) = 0;
    };
}

#endif //FUGUE_ABSTRACTKEYVALUESTORE_HPP
