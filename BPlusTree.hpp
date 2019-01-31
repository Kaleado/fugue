//
// Created by kaleado on 30/01/19.
//

#ifndef FUGUE_BPLUSTREE_HPP
#define FUGUE_BPLUSTREE_HPP

#include <array>
#include <iostream>

namespace Fugue {

    template<class Key, unsigned int size>
    class BPlusTree;

    template<class Key, unsigned int size>
    class BPlusNode;

    template<class Key, unsigned int size>
    class BPlusTree {
    private:
        BPlusNode<Key, size>* _root;
    public:

#ifdef DEBUG
        void* dbgPrint();
#endif
        void* get(Key k);

        void remove(Key k);

        void insert(Key k, void* value);

        BPlusTree<Key, size>() : _root{new BPlusNode<Key, size>(this, true, nullptr)} {}

        friend class BPlusNode<Key, size>;
    };

    template<class Key, unsigned int size>
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
            for(int i = sz - shiftBy - 1; i >= idx; --i){
                arr[i + shiftBy] = arr[i];
            }
        }

        void _splitAndInsert(Key k, void* data){
            unsigned int middleIndex = (size)/2;
            auto* newNode = new BPlusNode(_tree, _isLeaf, _parent);
            newNode->_currentSize = _currentSize - middleIndex;
            for(int j = 0; j < newNode->_currentSize; ++j){
                newNode->_keys[j] = _keys[middleIndex+j];
                newNode->_children[j] = _children[middleIndex+j];
                static_cast<BPlusNode<Key, size>*>(newNode->_children[j])->_parent = newNode;
            }
            _currentSize = middleIndex;
            auto newKey = _keys[middleIndex];
            if(k < _keys[middleIndex])
                _leafInsert(k, data);
            else
                newNode->_leafInsert(k, data);
            //Now we must properly rearrange the parent's children and keys.
            if(this == _tree->_root){
                _parent = new BPlusNode(_tree, false, nullptr);
                newNode->_parent = _parent;
                _tree->_root = _parent;
            }
            //Insert the appropriate node as the left child.
            if(k < newKey) {
                _parent->_leafInsert(newKey, this);
                _parent->_leafInsertAfter(newKey, newNode);
            }
            else{
                _parent->_leafInsert(newKey, newNode);
                _parent->_leafInsertAfter(newKey, this);
            }
        }

        //Move down the tree as we insert this item, assuming this node is a leaf node.
        void _leafInsert(Key k, void* data) {
            if(_currentSize + 1 > size){
                _splitAndInsert(k, data);
            }
            else {
                //Insert into this node.
                int pos = _positionFor(k);
                _rightShiftArray<void*, size+1>(_children, pos, 1);
                _children[pos] = data;
                _rightShiftArray<Key, size>(_keys, pos, 1);
                _keys[pos] = k;
                _currentSize += 1;
            }
        }

        //Move down the tree as we insert this item, assuming this node is a leaf node.
        void _leafInsertAfter(Key k, void* data) {
            //Insert into this node.
            int pos = _positionFor(k) + 1;
            //_rightShiftArray<void*, size+1>(_children, pos, 1);
            _children[pos] = data;
        }

        //Move down the tree as we insert this item, assuming this node is an inner node.
        void _innerInsert(Key k, void* data) {
            //Insert into this node.
            int pos = _positionFor(k);
            if(!_children[pos])
                _children[pos] = new BPlusNode(_tree, true, this);
            auto child = static_cast<BPlusNode<Key, size>*>(_children[pos]);
            if(child->_isLeaf)
                child->_leafInsert(k, data);
            else
                child->_innerInsert(k, data);
        }

    public:

#ifdef DEBUG
        //! For debugging purposes; print the node and its children.
        void dbgPrint();
#endif
        void* getKeyValue(Key k){
            if(_isLeaf)
                return _children[_positionFor(k)];
            return static_cast<BPlusNode<Key, size>*>(_children[_positionFor(k)])->getKeyValue(k);
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

#ifdef DEBUG
    template<class Key, unsigned int size>
    void BPlusNode<Key, size>::dbgPrint() {
        std::cout << "This node is " << (_isLeaf ? "" : "not ") << "a leaf.\n";
        for(unsigned int i = 0; i < _currentSize; ++i){
            std::cout << _keys[i] << " ";
        }
        std::cout << "\n";
        for(unsigned int i = 0; i <= _currentSize; ++i){
            std::cout << _children[i] << " ";
        }
        std::cout << "\n\n";
        if(_isLeaf)
            return;
        for(unsigned int i = 0; i <= _currentSize; ++i){
            if(_children[i])
                static_cast<BPlusNode<Key, size>*>(_children[i])->dbgPrint();
            else
                std::cout << "nullptr\n\n";
        }
        std::cout << "\n\n";
    }

    template<class Key, unsigned int size>
    void* BPlusTree<Key, size>::dbgPrint() {
        _root->dbgPrint();
    }
#endif

    template<class Key, unsigned int size>
    void* BPlusTree<Key, size>::get(Key k) {
        return _root->getKeyValue(k);
    }

    template<class Key, unsigned int size>
    void BPlusTree<Key, size>::remove(Key k) {

    }

    template<class Key, unsigned int size>
    void BPlusTree<Key, size>::insert(Key k, void* value) {
        if(_root && _root->_isLeaf)
            _root->_leafInsert(k, value);
        else
            _root->_innerInsert(k, value);
    }

}

#endif //FUGUE_BPLUSTREE_HPP
