//
// Created by kaleado on 30/01/19.
//

#ifndef FUGUE_BPLUSTREE_HPP
#define FUGUE_BPLUSTREE_HPP

#include <array>

namespace Fugue {

    template<class Key, int size>
    class BPlusTree;

    template<class Key, int size>
    class BPlusNode;

    template<class Key, int size>
    class BPlusTree {
    public:
        void* get(Key& k);
        void remove(Key& k);
        void insert(Key& k, void* value);
    private:
        BPlusNode<Key, size>* _root;
    };

    template<class Key, int size>
    class BPlusNode {
    private:
        bool _isLeaf;
        BPlusTree<Key, size>* _tree;
        void* _value;
        BPlusNode<Key, size>* _parent = nullptr;
        int _currentSize = 0;
        std::array<Key, size> _keys;
        std::array<void*, size+1> _children;

        //! Split this node into two, propagating the middle node upwards.
        void _directInsertKey(Key& k);
    public:
        //! Insert a key into this node.
        void insertKey(Key& k);
        //Move assignment/construction.
        BPlusNode& operator=(const BPlusNode<Key, size>&& rhs);
        BPlusNode(const BPlusNode<Key, size>&& rhs);

        //Copy assignment/construction.
        BPlusNode& operator=(const BPlusNode& rhs);
        BPlusNode(const BPlusNode& rhs);

        ~BPlusNode(BPlusTree<Key, size>* tree);
        ~BPlusNode(BPlusTree<Key, size>* tree, Key& k, void* value);
    };

}

#endif //FUGUE_BPLUSTREE_HPP
