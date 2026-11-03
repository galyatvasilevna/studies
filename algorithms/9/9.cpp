#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// БИНАРНОЕ ДЕРЕВО (обходы + зеркало для Части 1)
class BinaryTree {
public:
    Node* root;
    BinaryTree() : root(nullptr) {}

    // Обходы
    void preOrder(Node* n) { if (!n) return; cout << n->data << " "; preOrder(n->left); preOrder(n->right); }
    void inOrder(Node* n) { if (!n) return; inOrder(n->left); cout << n->data << " "; inOrder(n->right); }
    void postOrder(Node* n) { if (!n) return; postOrder(n->left); postOrder(n->right); cout << n->data << " "; }
    void levelOrder(Node* n) {
        if (!n) return;
        queue<Node*> q; q.push(n);
        while (!q.empty()) {
            Node* cur = q.front(); q.pop();
            cout << cur->data << " ";
            if (cur->left)  q.push(cur->left);
            if (cur->right) q.push(cur->right);
        }
    }

    Node* mirror(Node* n) {
        if (!n) return nullptr;
        swap(n->left, n->right);       // меняем местами
        mirror(n->left);               // идем в левое
        mirror(n->right);              // идем в правое
        return n;
    }

    void destroy(Node* n) { if (!n) return; destroy(n->left); destroy(n->right); delete n; }
    ~BinaryTree() { destroy(root); }
};

// БИНАРНОЕ ДЕРЕВО ПОИСКА (BST)
class BST {
    Node* insert(Node* n, int key) {
        if (!n) return new Node(key);
        if (key < n->data)      n->left = insert(n->left, key);
        else if (key > n->data) n->right = insert(n->right, key);
        return n;
    }

    Node* search(Node* n, int key) {
        if (!n || n->data == key) return n;
        if (key < n->data) return search(n->left, key);
        return search(n->right, key);
    }

    Node* findMin(Node* n) { while (n && n->left) n = n->left; return n; }

    Node* remove(Node* n, int key) {
        if (!n) return nullptr;
        if (key < n->data) n->left = remove(n->left, key);
        else if (key > n->data) n->right = remove(n->right, key);
        else {
            // Случай 1 и 2: 0 или 1 ребенок
            if (!n->left) { Node* t = n->right; delete n; return t; }
            if (!n->right) { Node* t = n->left;  delete n; return t; }
            // Случай 3: 2 ребенка — берем минимум из правого поддерева
            Node* t = findMin(n->right);
            n->data = t->data;
            n->right = remove(n->right, t->data);
        }
        return n;
    }

    void inOrder(Node* n) { if (!n) return; inOrder(n->left); cout << n->data << " "; inOrder(n->right); }

    // ВАРИАНТ 11: Восстановление BST из двух переставленных узлов
    void fixBST(Node* n, Node*& first, Node*& middle, Node*& last, Node*& prev) {
        if (!n) return;
        fixBST(n->left, first, middle, last, prev);

        if (prev && n->data < prev->data) {
            if (!first) { first = prev; middle = n; } // первое нарушение
            else last = n;                            // второе нарушение
        }
        prev = n;
        fixBST(n->right, first, middle, last, prev);
    }

    void destroy(Node* n) { if (!n) return; destroy(n->left); destroy(n->right); delete n; }

public:
    Node* root;
    BST() : root(nullptr) {}
    ~BST() { destroy(root); }

    void insert(int k) { root = insert(root, k); }
    bool search(int k) { return search(root, k) != nullptr; }
    void remove(int k) { root = remove(root, k); }
    void print() { inOrder(root); cout << "\n"; }

    // Обертка для восстановления
    void recoverBST() {
        Node* first = nullptr, * middle = nullptr, * last = nullptr, * prev = nullptr;
        fixBST(root, first, middle, last, prev);
        if (first && last) swap(first->data, last->data);       // несмежные
        else if (first && middle) swap(first->data, middle->data); // смежные
    }
};

int main() {
    // --- ЧАСТЬ 1: Зеркало дерева ---
    cout << "--- Part 1: Binary Tree Mirror ---\n";
    BinaryTree bt;
    bt.root = new Node(10);
    bt.root->left = new Node(5);
    bt.root->right = new Node(15);

    cout << "Before mirror (in-order): "; bt.inOrder(bt.root); cout << "\n";
    bt.mirror(bt.root);
    cout << "After mirror  (in-order): "; bt.inOrder(bt.root); cout << "\n\n";

    // --- ЧАСТЬ 2: BST и восстановление ---
    cout << "--- Part 2: BST Recovery (swapped nodes) ---\n";
    BST bst;
    // Строим правильное дерево, а потом меняем местами 2 узла
    for (int x : {50, 30, 70, 20, 40, 60, 80}) bst.insert(x);

    // Ломаем дерево: меняем 20 и 70 (они далеко)
    Node* n20 = bst.root->left->left;
    Node* n70 = bst.root->right;
    swap(n20->data, n70->data);

    cout << "Broken BST: "; bst.print();
    bst.recoverBST();
    cout << "Fixed BST:  "; bst.print(); cout << "\n";

    // --- ЧАСТЬ 3: Применение ---
    cout << "--- Part 3: BST Application (N=20) ---\n";
    BST appTree;
    for (int i = 0; i < 20; i++) appTree.insert(rand() % 100);
    cout << "Sorted BST: "; appTree.print();

    // Поиск Min/Max
    Node* cur = appTree.root;
    while (cur->left) cur = cur->left;  cout << "Min: " << cur->data << "\n";
    cur = appTree.root;
    while (cur->right) cur = cur->right; cout << "Max: " << cur->data << "\n";

    int toRemove;
    cout << "Enter number to remove: ";
    cin >> toRemove;
    appTree.remove(toRemove);
    cout << "After removal: "; appTree.print();

    return 0;
}
