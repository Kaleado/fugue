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

        void* get(Key k);

        void remove(Key k);

        void insert(Key k, void* value);

        BPlusTree<Key, size>() = default;

        friend class BPlusNode<Key, size>;
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

        int _positionFor(Key k){
            //TODO: use a binary search for this instead.
            int i;
            for(i = 0; i < _currentSize; ++i){
                if(_keys[i] >= k) break;
            }
            return i;
        }

        //! Shifts all elements of an array to the right by shiftBy indices, starting from idx.
        template<class S, int sz>
        void _rightShiftArray(std::array<S, sz>& arr, int idx, int shiftBy){
            for(int i = idx - shiftBy; i > idx; ++i){
                arr[i + shiftBy] = arr[i];
            }
        }

        //Move down the tree as we insert this item, assuming this node is a leaf node.
        void _leafInsert(Key k, void* data) {
            if(_currentSize + 1 > size){
                //TODO: We split the node.
                //We will need to write another method to handle this case: using _innerInsert will result in an infinite loop.
            }
            else {
                //Insert into this node.
                int pos = _positionFor(k);
                _rightShiftArray<void*, size+1>(_children, pos, 1);
                _children[pos] = data;
                _rightShiftArray<Key, size>(_keys, pos, 1);
                _keys[pos] = k;
            }
        }

        //Move down the tree as we insert this item, assuming this node is an inner node.
        void _innerInsert(Key k, void* data) {
            if(_currentSize + 1 > size){
                //TODO: We split the node.
                //We will need to write another method to handle this case: using _innerInsert will result in an infinite loop.
            }
            else {
                //Insert into this node.
                int pos = _positionFor(k);
                auto child = static_cast<BPlusNode<Key, size>*>(_children[pos]);
                if(child->_isLeaf)
                    _leafInsert(k, data);
                else
                    _innerInsert(k, data);
            }
        }

    public:
        //! Insert a key into this node.
        void insertKey(Key k) {

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

        friend class BPlusTree<Key, size>;
    };

    template<class Key, int size>
    void* BPlusTree<Key, size>::get(Key k) {
        return nullptr;
    }

    template<class Key, int size>
    void BPlusTree<Key, size>::remove(Key k) {

    }

    template<class Key, int size>
    void BPlusTree<Key, size>::insert(Key k, void* value) {
        if(_root->_isLeaf)
            _root->_leafInsert(k, value);
        else
            _root->_innerInsert(k, value);
    }

}

#endif //FUGUE_BPLUSTREE_HPP
