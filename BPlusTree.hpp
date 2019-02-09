//
// Created by kaleado on 30/01/19.
//

#ifndef FUGUE_BPLUSTREE_HPP
#define FUGUE_BPLUSTREE_HPP

#include <array>
#include <iostream>
#include "AbstractKeyValueStore.hpp"
#include "DataItem.hpp"

class BPlusNodeIntKeyTest;

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
        void dbgPrint();
#endif
        void* get(Key k) const;

        void remove(Key k);

        void insert(Key k, void* value);

        BPlusTree<Key, size>() : _root{new BPlusNode<Key, size>(this, true, nullptr, nullptr, nullptr)} {}

        ~BPlusTree(){
            delete _root;
        }

        friend class BPlusNode<Key, size>;
    };

    template<class Key, unsigned int size>
    class BPlusNode {
        friend class ::BPlusNodeIntKeyTest;
    private:
        BPlusTree<Key, size>* _tree;
        bool _isLeaf;
        BPlusNode<Key, size>* _parent;
        BPlusNode* _leftSibling;
        BPlusNode* _rightSibling;
        unsigned int _currentSize = 0;
        std::array<Key, size+1> _keys;
        std::array<void*, size+2> _children;

        //! Finds the index of the given child node in _children.
        int _positionOfChild(BPlusNode<Key, size> const* c) const {
            unsigned int pos = 0;
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
            unsigned int pos = _parent->_positionOfChild(this);
            return pos + 1 > _parent->_currentSize || pos < 0 ? nullptr : static_cast<BPlusNode<Key, size>*>(_parent->_children[pos + 1]);
        }

        //! Returns the index of the child node where key k lies.
        unsigned int _positionFor(Key k) const {
            //TODO: use a binary search for this instead.
            unsigned int i;
            if(_isLeaf){
                std::cout << "This is a leaf " <<  _currentSize << "...\n";
                for(i = 0; i < _currentSize; ++i){
                    std::cout << i << ": " << _keys[i] << ", ";
                    if( _keys.at(i) >= k){ std::cout << "\n"; return i; }
                }
            }
            else {
                std::cout << "This is not a leaf " <<  _currentSize << "...\n";
                for(i = 0; i < _currentSize; ++i){
                    std::cout << i << ": " << _keys[i] << ", ";
                    if( _keys.at(i) > k){ std::cout << "\n"; return i; }
                }
            }
            return _currentSize;
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
        void _split(Key k, void *data){
            //Create a new node and copy the right half of the children and keys to it.
#ifdef DEBUG
            std::cout << "Splitting " << this << " to insert " << k << "\n";
#endif
            unsigned int middleIndex = (size + 1)/2;
            auto* newNode = new BPlusNode(_tree, _isLeaf, _parent, _leftSibling, _rightSibling);
            newNode->_currentSize = size - middleIndex + (_isLeaf ? 1 : 0);
            for(unsigned int j = 0; j < newNode->_currentSize; ++j){
                unsigned int srcPos = middleIndex+j+(_isLeaf ? 0 : 1);
                newNode->_keys.at(j) = _keys.at(srcPos);
                newNode->_children.at(j) = _children.at(srcPos);
                if(!_isLeaf)
                    static_cast<BPlusNode<Key, size>*>(_children.at(srcPos))->_parent = newNode;
            }
            //Shrink the 'old' node.
            _currentSize = middleIndex;
            //Try to insert the key into the appropriate node.
            auto middleKey = _keys[_currentSize];
            //Now we must properly rearrange the parent's children and keys.
            //If we are the root, we need to actually create a new node to contain the middle element.
            if(this == _tree->_root){
                _parent = new BPlusNode(_tree, false, nullptr, nullptr, nullptr);
                newNode->_parent = _parent;
                _tree->_root = _parent;
            }
            //Insert the appropriate node as the left child.
            _parent->_leafInsert(middleKey, this);
            _parent->_insertChildAfter(middleKey, newNode);
        }

        //! Insert a child pointer after the given key.
        void _insertChildAfter(Key k, void *data) {
            // Insert the child after this node.
            unsigned int pos = _positionFor(k);
            _children[pos] = data;
        }

        //! Insert an item assuming this node is a leaf node.
        void _leafInsert(Key k, void* data) {
            // Insert into this node.
            unsigned int pos = _positionFor(k);
#ifdef DEBUG
            std::cout << "Inserting " << k << " into leaf " << this << " at position " << pos << "\n";
#endif
            _rightShiftArray<void*, size+2>(_children, pos, 1);
            _children[pos] = data;
            _rightShiftArray<Key, size+1>(_keys, pos, 1);
            _keys[pos] = k;
            _currentSize += 1;
            if(_currentSize > size){
                // This node has grown too large - split it and try to insert it that way.
                _split(k, data);
            }
        }

        //! Move down the tree as we insert this item, assuming this node is an inner node.
        void _innerInsert(Key k, void* data) {
            // Insert into this node.
            unsigned int pos = _positionFor(k);
#ifdef DEBUG
            std::cout << "Inserting " << k << " into " << this << " at position " << pos << "\n";
#endif
            auto child = static_cast<BPlusNode<Key, size>*>(_children[pos]);
            child->_parent = this;
            if(child->_isLeaf)
                child->_leafInsert(k, data);
            else
                child->_innerInsert(k, data);
        }

        void _updateKey(unsigned int keyIndex, Key newK){
#ifdef DEBUG
            assert(keyIndex >= 0 && keyIndex < _currentSize);
#endif
            _keys[keyIndex] = newK;
        }

        void _leafRemove(Key k){
            unsigned int pos = _positionFor(k);
            if(_children[pos]){
                auto* itm = static_cast<DataItem*>(_children[pos]);
                itm->free<void>();
                _children[pos] = nullptr;
                _leftShiftArray<void*, size+2>(_children, pos, 1);
                _leftShiftArray<Key, size+1>(_keys, pos, 1);
                _currentSize--;
            }
        }

        void _innerRemove(Key k){
            //TODO: this method.
        }

    public:

#ifdef DEBUG
        //! For debugging purposes; print the node and its children.
        void dbgPrint();
#endif
        //! Retrieve the value from the tree associated with this key.
        void* getKeyValue(Key k) const {
            unsigned int keyPos = _positionFor(k);
            if(_isLeaf){
                if(keyPos >= 0 && _keys[keyPos] == k)
                    return _children[keyPos];
                else
                    return nullptr;
            }
            auto* ptr = static_cast<BPlusNode<Key, size>*>(_children[keyPos]);
            if (ptr)
                return ptr->getKeyValue(k);
            else
                return nullptr;
        }

        void searchAndRemoveKey(Key k){
            // Traverse down the tree until we find where the item should be.
            // If the key is not in here, do nothing.
            // If we find it, delete it, reshuffle the array back into order.
                // If we have less than size/2 items:
                    // Borrow from the left sibling if it has more than size/2 items.
                    // Merge the left sibling if it does not.
                    // Borrow from the right sibling if there is no left sibling and it has more than size/2 items.
                    // Merge if it does not.
                    // If we have a parent:
                        // If the minimum value changed and this node is not the first child of its parent, adjust the value of the
                        // previous key in the parent.
                        // Move up the hierarchy and repeat this process.
                    //If we do not:
                        // If we have only one child, delete ourselves and make the root the child.

            if(_isLeaf){
                Key minKey = _keys[0];
                _leafRemove(k);
                if(_currentSize < size/2){
                    // TODO: Borrow, merge, etc.
                }
                if(_parent){
                    if(_keys[0] != minKey){
                        // Update the previous key in the parent.
                        unsigned int prevKeyPos = _parent->_positionFor(k);
                        if(prevKeyPos > 0)
                            _parent->_updateKey(prevKeyPos, _keys[0]);
                    }
                    //TODO: Move up the hierarchy.
                }
            }
            else{
                unsigned int keyPos = _positionFor(k);
#ifdef DEBUG
                assert(keyPos >= 0 && keyPos < _currentSize);
#endif
                if(_children[keyPos])
                    static_cast<BPlusNode<Key, size>*>(_children[keyPos])->searchAndRemoveKey(k);
            }
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
            _tree = rhs._tree;
            return *this;
        }

        BPlusNode<Key, size>(const BPlusNode<Key, size>& rhs) {
            _currentSize = rhs._currentSize;
            _parent = rhs._parent;
            _isLeaf = rhs._isLeaf;
            _leftSibling = rhs._leftSibling;
            _rightSibling = rhs._rightSibling;
            _children = rhs._children;
            _keys = rhs._keys;
            _tree = rhs._tree;
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
        std::cout << "Parent: " << _parent << "\n";
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
    void BPlusTree<Key, size>::dbgPrint() {
        _root->dbgPrint();
    }
#endif

    template<class Key, unsigned int size>
    void* BPlusTree<Key, size>::get(Key k) const {
        return _root->getKeyValue(k);
    }

    template<class Key, unsigned int size>
    void BPlusTree<Key, size>::remove(Key k) {
        _root->searchAndRemoveKey(k);
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
