#include <iostream>

using std::cout;
using std::endl;

namespace haizei{

struct TreeNode {
    int val;
    TreeNode *lchild, *rchild, *parent;
    TreeNode(int val, TreeNode*parent):val(val), lchild(nullptr), rchild(nullptr), parent(parent){}
};

class BinarySearchTreeHelper {
private:
    static TreeNode* right_most(TreeNode *node) {
        while (node->rchild != nullptr) node = node->rchild;
        return node;
    }
    static TreeNode* left_most(TreeNode *node) {
        while (node->lchild != nullptr) node = node->lchild;
        return node;
    }
public:
    static TreeNode* decreament(TreeNode *node) {
        if (node->lchild != nullptr) return right_most(node->lchild);
        while (node->parent != node && node->parent->lchild == node) node = node->parent;
        return node->parent;
    } 
    static TreeNode* increament(TreeNode *node) {
        if (node->rchild != nullptr) return left_most(node->rchild);
        while (node->parent != node && node->parent->rchild == node) node = node->parent;
        return node->parent;        
    }
};

class BinarySearchTree {
private:
    using Nodeptr = TreeNode*;
    using Helper = BinarySearchTreeHelper;
    using parentAndNodeptrs = std::pair<Nodeptr, Nodeptr>;

    TreeNode vHead;

    inline Nodeptr& root() {return vHead.lchild;}

    inline bool insert_node(Nodeptr root, int val) {
        Nodeptr new_node = new TreeNode(val, root);
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

    parentAndNodeptrs findParentAndNode(int val) {
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

    bool insert(int val) {
        if (root() == nullptr) {
            root() = new TreeNode(val, &vHead);
            return true;
        }
        parentAndNodeptrs p = findParentAndNode(val);
        if (p.second != nullptr) return false;
        return insert_node(p.first, val);
    }

    bool erase(int val) {
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

    int find(int val) {
        Nodeptr tmp = findParentAndNode(val).second;
        if (tmp == nullptr) return -1;
        else return tmp->val;
    }

    void output() {
        cout << "Tree: ";
        ouputimpl(root());
        cout << endl;
    }

};

}

using namespace haizei;

int main() {
    BinarySearchTree tree;
    srand(time(0));
    for (int i = 0; i < 100; i++) {
        tree.insert(rand() % 100);
    }
    for (int i = 0; i < 100; i++) {
        int val = rand() % 100;
        cout << "erase " << val << " " << tree.erase(val) << endl;
        tree.output();
    }
    return 0;
}
