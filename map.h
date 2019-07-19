#pragma once

#include "bst.h"

#include <functional>
namespace toy {

template <typename Key, typename Value, 
         typename Comare = std::less<Key>, 
         class TreeType = 
             BinarySearchTree<Key, Comare, std::pair<Key, Value>>>
class map
{
public:
    using ValuePair = std::pair<Key, Value>;
    using Tree = TreeType;
    using iterator = typename Tree::iterator;
    using const_iterator = typename Tree::const_iterator;

    map() : tree() {}

    std::pair<iterator, bool> insert(const ValuePair & value_pair) {
        return tree.insert_unique(value_pair);
    }

    size_t erase(const Key & key) {
        return tree.erase(key);
    }

    iterator find(const Key & key) {
        return tree.find(key);
    }

    const_iterator find(const Key & key) const {
        return tree.find(key);
    }

    iterator begin() {
        return tree.begin();
    }

    const_iterator begin() const {
        return tree.begin();
    }

    iterator end() {
        return tree.end();
    }

    const_iterator end() const {
        return tree.end();
    }

    ~map() {}

private:
    Tree tree;
};

}
