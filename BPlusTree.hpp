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
    private:
        BPlusNode<Key, size>* _root;
    public:

        void* get(Key& k) {
            return nullptr;
        }

        void remove(Key& k) {

        }

        void insert(Key& k, void* value) {

        }

        BPlusTree<Key, size>() = default;
    };

    template<class Key, int size>
    class BPlusNode {
    private:
        bool _isLeaf;
        BPlusTree<Key, size>* _tree;
        BPlusNode<Key, size>* _parent;
        int _currentSize = 0;
        std::array<Key, size> _keys;
        std::array<void*, size+1> _children;

        //! Split this node into two, propagating the middle node upwards.
        void _directInsertKey(Key& k) {

        }

    public:
        //! Insert a key into this node.
        void insertKey(Key& k) {

        }

        //Move assignment/construction.
        BPlusNode<Key, size>& operator=(const BPlusNode<Key, size>&& rhs);

        BPlusNode<Key, size>(const BPlusNode<Key, size>&& rhs);

        //Copy assignment/construction.
        BPlusNode<Key, size>& operator=(const BPlusNode<Key, size>& rhs) {
            _currentSize = rhs._currentSize;
            _parent = rhs._parent;
            _isLeaf = rhs._isLeaf;
            _children = rhs._children;
            _keys = rhs._keys;
            return *this;
        }

        BPlusNode<Key, size>(const BPlusNode<Key, size>& rhs) {
            _currentSize = rhs._currentSize;
            _parent = rhs._parent;
            _isLeaf = rhs._isLeaf;
            _children = std::move(rhs._children);
            _keys = std::move(rhs._keys);
        }

        ~BPlusNode<Key, size>() = default;

        BPlusNode<Key, size>(BPlusTree<Key, size>* tree, bool isLeaf, BPlusNode<Key, size>* parent)
                : _tree{tree}, _isLeaf{isLeaf}, _parent{parent} {};

        BPlusNode<Key, size>(BPlusTree<Key, size>* tree, bool isLeaf, BPlusNode<Key, size>* parent, std::array<Key, size> keys, std::array<void*, size+1> children)
                : _tree{tree}, _isLeaf{isLeaf}, _parent{parent}, _keys{keys}, _children{children} {};
    };

}

#endif //FUGUE_BPLUSTREE_HPP
