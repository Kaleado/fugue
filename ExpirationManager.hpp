//
// Created by kaleado on 16/02/19.
//

#ifndef FUGUE_CACHEMANAGER_HPP
#define FUGUE_CACHEMANAGER_HPP

#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <forward_list>
#include "AbstractKeyValueStore.hpp"

namespace Fugue {

    //! Passively frees expired store entries.
    //! Assumes all items are held in Fugue::DataItems.
    template <class Key>
    class ExpirationManager {
    private:

        //! Represents a cache item that will eventually expire.
        class CacheItem {
        public:
            using Ptr = std::shared_ptr<CacheItem>;

            Ptr next = nullptr;
            std::chrono::time_point<std::chrono::system_clock> expirationTime;
            Key key;

            CacheItem(std::chrono::time_point<std::chrono::system_clock> e, Key k) : expirationTime{e}, key{k} {}
        };

        std::atomic<bool> _shouldStop{false};
        AbstractKeyValueStore<Key>& _store;
        std::chrono::duration<double> _frequency;
        std::thread _workerThread;
        std::mutex _listMutex;
        typename CacheItem::Ptr _expirations = nullptr;

        void _cleanExpiredItems();

    public:

        std::thread& start();

        void stop();

        void addExpiringKey(Key k, std::chrono::time_point<std::chrono::system_clock> expirationTime);

        ExpirationManager(std::chrono::duration<double> frequency,
                AbstractKeyValueStore<Key>& store) : _frequency{frequency}, _store{store} {}
    };

    template<class Key>
    void ExpirationManager<Key>::_cleanExpiredItems() {
        while(!_shouldStop.load()){
#ifdef DEBUG
            std::cout << "Starting cache clean...\n";
#endif
            std::lock_guard<std::mutex> lck{_listMutex};
            for(typename CacheItem::Ptr itm = _expirations;
                itm != nullptr && itm->expirationTime >= std::chrono::system_clock::now(); itm = itm->next){
                    _store.remove(itm->key);
                    // TODO: ensure dangling pointer errors will not occur when the removed data item is about to be
                    // transmitted across the wire!
            }
            std::this_thread::sleep_for(_frequency);
        }
    }

    template<class Key>
    std::thread &ExpirationManager<Key>::start() {
        _shouldStop = false;
        _workerThread = std::thread{&ExpirationManager::_cleanExpiredItems, this};
        return _workerThread;
    }

    template<class Key>
    void ExpirationManager<Key>::stop() {
        _shouldStop = true;
        _workerThread.join();
    }

    template<class Key>
    void ExpirationManager<Key>::addExpiringKey(Key k,
            std::chrono::time_point<std::chrono::system_clock> expirationTime) {
        auto itm = std::make_shared<CacheItem>(expirationTime, k);
        typename CacheItem::Ptr it = nullptr;
        if(!_expirations)
            _expirations = itm;
        else{
            for(it = _expirations;
                it->next != nullptr && it->expirationTime < expirationTime; it = it->next){}
            itm->next = it->next;
            it->next = itm;
        }
    }

}

#endif //FUGUE_CACHEMANAGER_HPP
