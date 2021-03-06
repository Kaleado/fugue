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
#include <iostream>
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
        std::chrono::duration<double> _frequency;
        AbstractKeyValueStore<Key>& _store;
        std::thread _workerThread;
        std::mutex _listMutex;
        typename CacheItem::Ptr _expirations = nullptr;

        void _cleanExpiredItems();

    public:

        std::thread& start();

        void stop();

        void addExpiringKey(Key k, std::chrono::time_point<std::chrono::system_clock> expirationTime);

        void removeExpiringKey(Key k);

        std::chrono::time_point<std::chrono::system_clock> expirationTime(Key k);

        ExpirationManager(std::chrono::duration<double> frequency,
                AbstractKeyValueStore<Key>& store) : _frequency{frequency}, _store{store} {}
    };

    template<class Key>
    void ExpirationManager<Key>::_cleanExpiredItems() {
        if(_frequency.count() == 0)
            return;
        while(!_shouldStop.load()){
#ifdef DEBUG
            std::cout << "Starting cache clean...\n";
#endif
            std::lock_guard<std::mutex> lck{_listMutex};
            typename CacheItem::Ptr itm;
            for(itm = _expirations;
                itm != nullptr && itm->expirationTime <= std::chrono::system_clock::now(); itm = itm->next){
#ifdef DEBUG
                std::cout << "Purging key " << itm->key << "\n";
#endif
                auto storeLck = _store.getUniqueLock();
                _store.remove(itm->key);
            }
            _expirations = itm;
#ifdef DEBUG
            if(_expirations)
                std::cout << "New head key of expiration list: " << _expirations->key << "\n";
            else
                std::cout << "No more keys to purge due to expiration.\n";
#endif
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
            for(it = _expirations; it->next != nullptr && it->expirationTime < expirationTime; it = it->next){}
            itm->next = it->next;
            it->next = itm;
        }
    }

    template <typename Key>
    void ExpirationManager<Key>::removeExpiringKey(Key k){
        typename CacheItem::Ptr it = _expirations;
        if(!it)
            throw std::invalid_argument("Error removing expiration from key: expiration list is empty.");
        // If we removed the head of the list, update it to point to the next item.
        if(it->key == k){
            _expirations = it->next;
        }
        else{
            // Iterate until the next item is the one we want to remove, then update the current item's next pointer.
            for(it = _expirations; it->next != nullptr && it->next->key != k; it = it->next){}
            if(it->next != nullptr)
                it->next = it->next->next;
            else
                throw std::invalid_argument("Error removing expiration from key: key does not have an expiry.");
        }
    }

    template <typename Key>
    std::chrono::time_point<std::chrono::system_clock> ExpirationManager<Key>::expirationTime(Key k){
        for(auto it = _expirations; it != nullptr; it = it->next){
            if(it->key == k)
                return it->expirationTime;
        }
        throw std::runtime_error("Error getting expiration time: the key did not have an expiry set.");
    }

}

#endif //FUGUE_CACHEMANAGER_HPP
