//
// Created by kaleado on 30/01/19.
//

#ifndef FUGUE_BPLUSTREE_HPP
#define FUGUE_BPLUSTREE_HPP

#include <array>
#include <iostream>
#include "AbstractKeyValueStore.hpp"

namespace Fugue {

    template<class Key, unsigned int size>
    class BPlusTree;

    template<class Key, unsigned int size>
    class BPlusNode;

    template<class Key, unsigned int size>
    class BPlusTree : public Fugue::AbstractKeyValueStore<Key> {
    private:
        BPlusNode<Key, size>* _root;
    public:

#ifdef DEBUG
        void* dbgPrint();
#endif
        void* get(Key k) const;

        void remove(Key k);

        void insert(Key k, void* value);

        BPlusTree<Key, size>() : _root{new BPlusNode<Key, size>(this, true, nullptr, nullptr, nullptr)} {}

        friend class BPlusNode<Key, size>;
    };

    template<class Key, unsigned int size>
    class BPlusNode {
    private:
        bool _isLeaf;
        BPlusTree<Key, size>* _tree;
        BPlusNode<Key, size>* _parent;
        BPlusNode* _leftSibling;
        BPlusNode* _rightSibling;
        int _currentSize = 0;
        std::array<Key, size> _keys;
        std::array<void*, size+1> _children;

        //! Finds the index of the given child node in _children.
        int _positionOfChild(BPlusNode<Key, size> const* c) const {
            int pos = 0;
            for(pos = 0; pos <= _currentSize; ++pos){
                if (_children[pos] == c)
                    return pos;
            }
            return -1;
        }

        //! Returns the left sibling of this node.
        const BPlusNode<Key, size>* _left() const {
            if(_parent == nullptr) return nullptr;
            int pos = _parent->_positionOfChild(this);
            return pos <= 0 ? nullptr : static_cast<BPlusNode<Key, size>*>(_parent->_children[pos - 1]);
        }

        //! Returns the right sibling of this node.
        const BPlusNode<Key, size>* _right() const {
            if(_parent == nullptr) return nullptr;
            int pos = _parent->_positionOfChild(this);
            return pos + 1 > _parent->_currentSize || pos < 0 ? nullptr : static_cast<BPlusNode<Key, size>*>(_parent->_children[pos + 1]);
        }

        //! Returns the index of the child node where key k lies.
        int _positionFor(Key k) const {
            //TODO: use a binary search for this instead.
            int i;
            for(i = 0; i < _currentSize; ++i){
                if(_keys[i] >= k) break;
            }
            return i;
        }

        //! Shifts all elements of an array to the right by shiftBy indices, in the range idx.._currentSize.
        template<class S, int sz>
        void _rightShiftArray(std::array<S, sz>& arr, int idx, int shiftBy){
            for(int i = sz - shiftBy - 1; i >= idx; --i){
                arr[i + shiftBy] = arr[i];
            }
        }

        //! Shifts all elements of an array to the left by shiftBy indices, in the range 0..idx.
        template<class S, int sz>
        void _leftShiftArray(std::array<S, sz>& arr, int idx, int shiftBy){
            for(int i = shiftBy; i < idx; ++i){
                arr[i - shiftBy] = arr[i];
            }
        }

        //! Split this node and continue trying to insert the given key.
        void _splitAndInsert(Key k, void* data){
            //Create a new node and copy the right half of the children and keys to it.
            unsigned int middleIndex = (size)/2;
            auto* newNode = new BPlusNode(_tree, _isLeaf, _parent, _leftSibling, _rightSibling);
            newNode->_currentSize = _currentSize - middleIndex;
            for(int j = 0; j < newNode->_currentSize; ++j){
                newNode->_keys[j] = _keys[middleIndex+j];
                newNode->_children[j] = _children[middleIndex+j];
                static_cast<BPlusNode<Key, size>*>(newNode->_children[j])->_parent = newNode;
            }
            //Shrink the 'old' node.
            _currentSize = middleIndex;
            //Try to insert the key into the appropriate node.
            auto newKey = _keys[middleIndex];
            if(k < newKey) // If this key should go on the left.
                _leafInsert(k, data);
            else //If this key should go on the right.
                newNode->_leafInsert(k, data);
            //Now we must properly rearrange the parent's children and keys.
            //If we are the root, we need to actually create a new node to contain the middle element.
            if(this == _tree->_root){
                _parent = new BPlusNode(_tree, false, nullptr, nullptr, nullptr);
                newNode->_parent = _parent;
                _tree->_root = _parent;
            }
            //Insert the appropriate node as the left child.
            if(k < newKey) {
                // This node is the left node, and the other node is the right node.
                _parent->_leafInsert(newKey, this);
                _parent->_insertChildAfter(newKey, newNode);
                this->_rightSibling = newNode;
                newNode->_leftSibling = this;
            }
            else{
                //The other node is the left node, and this is the right node.
                _parent->_leafInsert(newKey, newNode);
                _parent->_insertChildAfter(newKey, this);
                this->_leftSibling = newNode;
                newNode->_rightSibling = this;
            }
        }

        //! Insert a child pointer after the given key.
        void _insertChildAfter(Key k, void *data) {
            // Insert the child after this node.
            int pos = _positionFor(k) + 1;
            _children[pos] = data;
        }

        //! Insert an item assuming this node is a leaf node.
        void _leafInsert(Key k, void* data) {
            if(_currentSize + 1 > size){
                // This node has grown too large - split it and try to insert it that way.
                _splitAndInsert(k, data);
            }
            else {
                // Insert into this node.
                int pos = _positionFor(k);
                _rightShiftArray<void*, size+1>(_children, pos, 1);
                _children[pos] = data;
                _rightShiftArray<Key, size>(_keys, pos, 1);
                _keys[pos] = k;
                _currentSize += 1;
            }
        }

        //! Move down the tree as we insert this item, assuming this node is an inner node.
        void _innerInsert(Key k, void* data) {
            // Insert into this node.
            int pos = _positionFor(k);
            // If we try to traverse into a node that doesn't exist, we found the right spot and we should make a node to
            // hold the key.
            if(!_children[pos]){
                BPlusNode<Key, size>* newLeftSibling = nullptr;
                BPlusNode<Key, size>* newRightSibling = nullptr;
                if(pos < size - 1)
                    newRightSibling = static_cast<BPlusNode<Key, size>*>(_children[pos + 1]);
                if(pos > 0)
                    newLeftSibling = static_cast<BPlusNode<Key, size>*>(_children[pos - 1]);
                _children[pos] = new BPlusNode(_tree, true, this, newLeftSibling, newRightSibling);
            }
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
        //! Retrieve the value from the tree associated with this key.
        void* getKeyValue(Key k) const {
            if(_isLeaf){
                if(_keys[_positionFor(k) - 1] == k)
                    return _children[_positionFor(k) - 1];
                else
                    return nullptr;
            }
            return static_cast<BPlusNode<Key, size>*>(_children[_positionFor(k)])->getKeyValue(k);
        }

        // Move assignment/construction.
        BPlusNode<Key, size>& operator=(const BPlusNode<Key, size>&& rhs);

        BPlusNode<Key, size>(const BPlusNode<Key, size>&& rhs);

        // Copy assignment/construction.
        BPlusNode<Key, size>& operator=(const BPlusNode<Key, size>& rhs) {
            _currentSize = rhs._currentSize;
            _parent = rhs._parent;
            _isLeaf = rhs._isLeaf;
            _children = rhs._children;
            _leftSibling = rhs._leftSibling;
            _rightSibling = rhs._rightSibling;
            _keys = rhs._keys;
            return *this;
        }

        BPlusNode<Key, size>(const BPlusNode<Key, size>& rhs) {
            _currentSize = rhs._currentSize;
            _parent = rhs._parent;
            _isLeaf = rhs._isLeaf;
            _leftSibling = rhs._leftSibling;
            _rightSibling = rhs._rightSibling;
            _children = std::move(rhs._children);
            _keys = std::move(rhs._keys);
        }

        ~BPlusNode<Key, size>() = default;

        BPlusNode<Key, size>(BPlusTree<Key, size>* tree, bool isLeaf, BPlusNode<Key, size>* parent, BPlusNode<Key, size>* leftSibling, BPlusNode<Key, size>* rightSibling)
                : _tree{tree}, _isLeaf{isLeaf}, _parent{parent}, _leftSibling{leftSibling}, _rightSibling{rightSibling} {}

        BPlusNode<Key, size>(BPlusTree<Key, size>* tree, bool isLeaf, BPlusNode<Key, size>* parent, BPlusNode<Key, size>* leftSibling, BPlusNode<Key, size>* rightSibling, std::array<Key, size> keys, std::array<void*, size+1> children)
                : _tree{tree}, _isLeaf{isLeaf}, _parent{parent}, _leftSibling{leftSibling}, _rightSibling{rightSibling}, _keys{keys}, _children{children} {};

        friend class BPlusTree<Key, size>;
    };

#ifdef DEBUG
    template<class Key, unsigned int size>
    void BPlusNode<Key, size>::dbgPrint() {
        std::cout << "Node " << this << " is " << (_isLeaf ? "" : "not ") << "a leaf.\n";
        std::cout << "Siblings: " << _left() << " " << _right() << "\n";
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
    void* BPlusTree<Key, size>::get(Key k) const {
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
