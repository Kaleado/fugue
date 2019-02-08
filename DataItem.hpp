//
// Created by kaleado on 4/02/19.
//

#ifndef FUGUE_DATAITEM_HPP
#define FUGUE_DATAITEM_HPP

#include <cstddef>
#include <new>

namespace Fugue {

    //! Represents a typeless data item from the underlying storage.
    class DataItem {
    private:
    public:
        std::size_t length;
        void* raw;

        template<class T>
        T& get();

        template<class T>
        void free();

        DataItem(void* r=nullptr, std::size_t l=0);

    };

    template<class T>
    T& DataItem::get() {
        return *static_cast<T*>(raw);
    }

    template<class T>
    void DataItem::free() {
        delete static_cast<T*>(raw);
    }

}

#endif //FUGUE_DATAITEM_HPP
