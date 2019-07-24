#include <iostream>

using std::cout;
using std::endl;

namespace haizei{

struct TreeNodeBase {
    TreeNodeBase *lchild, *rchild, *parent;
    TreeNodeBase(TreeNodeBase *parent_ = nullptr) :lchild(nullptr), rchild(nullptr),  parent(parent_) {}
    ~TreeNodeBase() {}
};

class BinarySearchTreeHelper {
    using TreeNodePtr = TreeNodeBase* ;

public:
    static TreeNodePtr right_most(TreeNodePtr node) {
        while (node->rchild != nullptr) node = node->rchild;
        return node;
    }
    static TreeNodePtr left_most(TreeNodePtr node) {
        while (node->lchild != nullptr) node = node->lchild;
        return node;
    }
    static TreeNodePtr decreament(TreeNodePtr node) {
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
struct TreeNode : public TreeNodeBase {
    Value val;
    TreeNode(const Value& val, TreeNode* parent_) : TreeNodeBase(parent_), val(val) {}
};

template<typename Value>
class IteratorImpl {
    using ref = Value&;
    using ptr =  Value*;
    using TreeNodePtr = TreeNode<Value>* ;
    using helper = BinarySearchTreeHelper;

private:
    TreeNodePtr node;

public:
    IteratorImpl(TreeNodePtr node) : node(node) {}

    ref operator *() {
        return node->val;
    }

    ptr operator ->() {
        return &(this->node->val);
    }

    IteratorImpl& operator ++() {
        node = static_cast<TreeNode<Value>*>(helper::increament(node));
        return *this;
    }

    IteratorImpl operator ++(int) {
        IteratorImpl old = *this;
        ++(*this);
        return old;
    }

    IteratorImpl& operator --() {
        node = helper::decreament(node);
        return *this;
    }

    IteratorImpl& operator --(int) {
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

template<typename Key, typename KeyValue >
class BinarySearchTree {

    using Value = std::pair<Key, KeyValue>;

    using Node = TreeNode<Value>;
    using Nodeptr = Node*;

    using BaseNode = TreeNodeBase;
    using BaseNodeptr = BaseNode*;

    using parentAndNodeptrs = std::pair<Nodeptr, Nodeptr>;

    using Helper = BinarySearchTreeHelper;
    using iterator = IteratorImpl<Value>;

private:
    Nodeptr vHead;

    static inline Key& getKey(Value &v) { return v.first; }

    static int compareKeys(Key& a, Key& b) {
        if (b < a) return 1;
        else if (a < b) return -1;
        else return 0;
    }

    inline Nodeptr root() const {return static_cast<Nodeptr>(vHead->lchild);}

    inline bool insert_node(Nodeptr root, Value val) {
        Nodeptr new_node = new Node(val, root);
        if (compareKeys(getKey(val), getKey(root->val)) == -1) root->lchild = new_node;
        else root->rchild = new_node;
        return true;
    }

    inline bool delete_node(BaseNodeptr node) {
        BaseNodeptr child = (node->lchild != nullptr ? 
               node->lchild : node->rchild);
        (node->parent->lchild == node ? 
         node->parent->lchild: node->parent->rchild) = child;
        if (child) child->parent = node->parent;
        delete node;
        return true;
    }

    parentAndNodeptrs findParentAndNode(Key key) {
        if (root() == nullptr) 
            return std::make_pair(vHead, nullptr);
        int op;
        Nodeptr p = root(), parent = vHead;
        while(p != nullptr && (op = compareKeys(key, getKey(p->val))) != 0) {
            parent = p;
            if (op == -1) p = static_cast<Nodeptr>(p->lchild);
            else if (op == 1) p = static_cast<Nodeptr>(p->rchild);
        }
        return std::make_pair(parent,p);
    }

public:
    BinarySearchTree() {
        vHead = (Nodeptr)malloc(sizeof(Node));
        vHead->lchild = vHead->rchild = nullptr;
        vHead->parent = vHead;
    }

    ~BinarySearchTree() {
        free(vHead);
    }

    inline const iterator begin() const {
        if (root() == nullptr) return end();
        else return iterator(static_cast<Nodeptr>(Helper::left_most(root())));
    }

    inline const iterator end() const { return iterator(vHead); }

    bool insert(Value val) {
        if (root() == nullptr) {
            vHead->lchild = new Node(val, vHead);
            return true;
        }
        parentAndNodeptrs p = findParentAndNode(getKey(val));
        if (p.second != nullptr) return false;
        return insert_node(p.first, val);
    }

    bool erase(Key key) {
        Nodeptr node = findParentAndNode(key).second;
        if (node == nullptr) return false;
        if (node->lchild == nullptr || node->rchild == nullptr) {
            return delete_node(node);
        } else {
            Nodeptr tmp = static_cast<Nodeptr>(Helper::decreament(node));
            node->val = tmp->val;
            delete_node(tmp);
            return true;
        }
    }

    bool erase(const iterator& it) { return erase(*(it)); }

    Value& find(Key key) {
        Nodeptr tmp = findParentAndNode(key).second;
        if (tmp == nullptr) return -1;
        else return tmp->val;
    }

};

}

// 迭代器设计是ｓｔｌ的一个重点
// 结构与数值分离, 类型转换了
// vHead 设计用来方便插入与删除, 同时设置为左孩子, 左闭合右开
// 只重载小于号的设计

/*      测试                */

#include <map>
using namespace std;
using namespace haizei;

struct Node {
    int value, key;

    Node (int val, int key):value(val), key(key) {}

    bool operator < (const Node & node) const {
        return value < node.value;
    }

    friend ostream& operator<< (ostream& out, const Node & node) {
        return out << node.value << "-" << node.key;
    }
};

template<typename T>
void outputIterator(const T & tree) {
    cout << "Start:";
    for (auto it = tree.begin(); it != tree.end(); it++) {
        cout << "( " << it->first << "," << it->second.key  << " ) ";
    }
    cout << endl;
}

void output() {}
template<typename T, typename ...Args>
void output(const T &a, Args... args) {
    outputIterator(a);
    output(args...);
}

#define doThameThing(a, b, things) {a.things;b.things;}

int main() {
    srand(time(0));

    BinarySearchTree<int, Node> tree;
    map<int, Node> m;

    for (int i = 0; i < 10; i++) {
        int val = rand() % 10;
        doThameThing(tree, m, insert(make_pair(val, Node(i, i))));
    }

    output(tree, m);

    for (int i = 0; i < 10; i++) {
        doThameThing(tree, m, erase(i));
        output(tree, m);
    }
    return 0;
}
