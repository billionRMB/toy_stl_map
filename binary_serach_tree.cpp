#include <iostream>

using std::cout;
using std::endl;

namespace haizei{

template<typename Value>
struct TreeNode {
    Value val;
    TreeNode *lchild, *rchild, *parent;
    TreeNode(Value val, TreeNode* parent):val(val), lchild(nullptr), rchild(nullptr), parent(parent){}
};

template<typename Value>
class BinarySearchTreeHelper {
    using TreeNodePtr = TreeNode<Value>* ;
public:
    static TreeNodePtr right_most(TreeNodePtr node) {
        while (node->rchild != nullptr) node = node->rchild;
        return node;
    }
    static TreeNodePtr left_most(TreeNodePtr node) {
        while (node->lchild != nullptr) node = node->lchild;
        return node;
    }
    static TreeNodePtr decreament(TreeNodePtr *node) {
        if (node->lchild != nullptr) return right_most(node->lchild);
        while (node->parent != node && node->parent->lchild == node) node = node->parent;
        return node->parent;
    } 
    static TreeNodePtr increament(TreeNodePtr node) {
        if (node->rchild != nullptr) return left_most(node->rchild);
        while (node->parent != node && node->parent->rchild == node) node = node->parent;
        return node->parent;        
    }
};

template<typename Value>
class IteratorImpl {
    using ref = Value&;
    using ptr = Value*;
    using TreeNodePtr = TreeNode<Value>* ;
    using helper = BinarySearchTreeHelper<Value>;
    TreeNodePtr node;
public:
    IteratorImpl(TreeNodePtr node) : node(node) {}

    ref operator *() {
        return node->val;
    }

    ptr operator ->() {
        return &(this->node->val);
    }

    IteratorImpl & operator ++() {
        node = helper::increament(node);
        return *this;
    }

    IteratorImpl operator ++(int) {
        IteratorImpl old = *this;
        ++(*this);
        return old;
    }

    IteratorImpl & operator --() {
        node = helper::decreament(node);
        return *this;
    }

    IteratorImpl operator --(int) {
        IteratorImpl old = *this;
        --(*this);
        return old;
    }

    bool operator == (const IteratorImpl& b) {
        return this->node == b.node;
    }

    bool operator != (const IteratorImpl& b) {
        return this->node != b.node;
    }
};

template<typename Value>
class BinarySearchTree {
private:
    using Nodeptr = TreeNode<Value>*;
    using Node = TreeNode<Value>;
    using Helper = BinarySearchTreeHelper<Value>;
    using parentAndNodeptrs = std::pair<Nodeptr, Nodeptr>;
    using iterator = IteratorImpl<Value>;

    Node vHead;

    inline Nodeptr& root() {return vHead.lchild;}

    inline bool insert_node(Nodeptr root, Value val) {
        Nodeptr new_node = new Node(val, root);
        if (val < root->val) root->lchild = new_node;
        else root->rchild = new_node;
        return true;
    }

    bool delete_node(Nodeptr node) {
        Nodeptr child = (node->lchild != nullptr ? 
               node->lchild : node->rchild);
        (node->parent->lchild == node ? 
         node->parent->lchild: node->parent->rchild) = child;
        if (child) child->parent = node->parent;
        delete node;
        return true;
    }

    void ouputimpl(Nodeptr root) {
        if (root == nullptr) return;
        ouputimpl(root->lchild);
        cout << root->val << " ";
        ouputimpl(root->rchild);
    }

    parentAndNodeptrs findParentAndNode(Value val) {
        if (root() == nullptr) return std::make_pair(&vHead, nullptr);
        Nodeptr p = root(),parent = root()->parent;
        while(p != nullptr && val != p->val) {
            parent = p;
            if (val < p->val) p = p->lchild;
            else if (val > p->val) p = p->rchild;
        }
        return std::make_pair(parent,p);
    }

public:
    BinarySearchTree (): vHead(-1, nullptr) {vHead.parent = &vHead;}

    bool insert(Value val) {
        if (root() == nullptr) {
            root() = new Node(val, &vHead);
            return true;
        }
        parentAndNodeptrs p = findParentAndNode(val);
        if (p.second != nullptr) return false;
        return insert_node(p.first, val);
    }

    bool erase(Value val) {
        Nodeptr node = findParentAndNode(val).second;
        if (node == nullptr) return false;
        if (node->lchild == nullptr || node->rchild == nullptr) {
            return delete_node(node);
        } else {
            Nodeptr tmp = Helper::decreament(node);
            node->val = tmp->val;
            delete_node(tmp);
            return true;
        }
    }

    int find(Value val) {
        Nodeptr tmp = findParentAndNode(val).second;
        if (tmp == nullptr) return -1;
        else return tmp->val;
    }

    void output() {
        cout << "Tree: ";
        for (iterator it = begin(); it != end(); it++) {
            cout << *(it) << " ";
        }
        cout << endl;
    }

    iterator begin() {
        return iterator(Helper::left_most(root()));
    }

    iterator end() {
        return iterator(&vHead);
    }

};

}

using namespace haizei;

using std::ostream;

struct Node {
    int val;
    Node (int val):val(val) {}
    friend ostream& operator << (ostream& out, const Node&node) {
        return out << node.val;
    }
    bool operator < (const Node & node) {
        return val < node.val;
    }
    bool operator > (const Node & node) {
        return val > node.val;
    }
    bool operator == (const Node & node) {
        return val == node.val;
    }
    bool operator != (const Node & node) {
        return val != node.val;
    }
};

int main() {
    BinarySearchTree<Node> tree;
    srand(time(0));
    for (int i = 0; i < 100; i++) {
        tree.insert(Node(rand() % 100));
    }
    tree.output();
    return 0;
}
