# Лабораторная работа №9  
## Бинарное дерево. Обходы. BST

**Вариант 11**  
**Дисциплина:** Алгоритмы и структуры данных  
**Выполнила:** Тарасенко Г. В., группа ПИЖ-6-о-25-1(2)  
**Преподаватель:** ассистент Белик А. И.  
**Ставрополь 2026**

---

## Цель работы
1. Реализовать структуру данных «бинарное дерево» и изучить способы обхода (прямой, симметричный, обратный, уровневый).
2. Реализовать бинарное дерево поиска (BST) с операциями вставки, удаления, поиска.
3. Освоить рекурсию для работы с деревьями.
4. Проанализировать сложность операций в BST.
5. Научиться визуализировать структуру дерева.

## Задание (Вариант 11)
**Часть 1:** Построить зеркальное отображение бинарного дерева (swap левого и правого потомков).  
**Часть 2:** Восстановление BST из двух переставленных узлов.  
**Часть 3:** Применение BST — построение дерева из случайных чисел, вывод, поиск min/max, удаление узла.

---

## Теоретическая часть

| Вид обхода | Порядок посещения |
|---|---|
| Прямой (pre-order) | Корень → Левый → Правый |
| Симметричный (in-order) | Левый → Корень → Правый |
| Обратный (post-order) | Левый → Правый → Корень |
| Уровневый (level-order) | По уровням слева направо (очередь) |

**BST:** для каждого узла все ключи в левом поддереве < ключ узла < все ключи в правом поддереве. Симметричный обход даёт отсортированную последовательность.

---

## Полный код решения

```cpp
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
```

---

## Результаты работы

```
--- Part 1: Binary Tree Mirror ---
Before mirror (in-order): 5 10 15
After mirror  (in-order): 15 10 5

--- Part 2: BST Recovery (swapped nodes) ---
Broken BST: 70 30 40 50 60 20 80
Fixed BST:  20 30 40 50 60 70 80

--- Part 3: BST Application (N=20) ---
Sorted BST: 0 5 24 27 34 36 41 42 45 58 61 62 64 67 69 78 81 91 95
Min: 0
Max: 95
Enter number to remove: 5
After removal: 0 24 27 34 36 41 42 45 58 61 62 64 67 69 78 81 91 95
```

---

## Анализ сложности

| Операция | Средний случай | Худший случай |
|---|---|---|
| Поиск в BST | O(log n) | O(n) |
| Вставка в BST | O(log n) | O(n) |
| Удаление в BST | O(log n) | O(n) |
| Обходы (все) | O(n) | O(n) |
| Зеркало | O(n) | O(n) |
| Восстановление BST | O(n) | O(n) |

Худший случай O(n) достигается на вырожденном дереве (вставка в отсортированном порядке).

---

## Ответы на контрольные вопросы

**1. Что такое бинарное дерево? Из каких элементов оно состоит?**  
Иерархическая структура, где каждый узел имеет не более двух потомков (левый и правый). Состоит из узлов с полями данных и указателями на потомков.

**2. В чём отличие бинарного дерева от бинарного дерева поиска (BST)?**  
В BST выполняется свойство: все ключи левого поддерева < ключ узла < все ключи правого поддерева. В обычном бинарном дереве порядок не важен.

**3. Какие существуют виды обхода бинарного дерева? В чём их особенности?**  
Pre-order (корень→левый→правый), in-order (левый→корень→правый), post-order (левый→правый→корень), level-order (по уровням через очередь). Различаются порядком посещения узлов.

**4. Какой обход даёт отсортированную последовательность для BST?**  
Симметричный (in-order) обход.

**5. Какова сложность операций поиска, вставки и удаления в BST? От чего она зависит?**  
O(log n) в среднем, O(n) в худшем. Зависит от высоты дерева: чем более вырождено дерево, тем медленнее операции.

**6. Что происходит с BST, если вставлять элементы в отсортированном порядке?**  
Дерево становится вырожденным (списком), высота = n, сложность операций деградирует до O(n).

**7. Какие три случая нужно рассмотреть при удалении узла из BST?**  
1) Узел без детей — просто удалить.  
2) Узел с одним ребёнком — заменить узлом-ребёнком.  
3) Узел с двумя детьми — заменить минимальным элементом правого поддерева и удалить его.

**8. Как найти наименьший элемент в BST? Наибольший?**  
Наименьший — идти по левым указателям от корня до конца. Наибольший — по правым.

**9. Как вычислить высоту бинарного дерева рекурсивно?**  
`height(n) = 1 + max(height(n->left), height(n->right))`, для `nullptr` — 0.

**10. Что такое наименьший общий предок (LCA) двух узлов в дереве?**  
Самый нижний узел, который является предком обоих заданных узлов.

**11. Как проверить, является ли бинарное дерево BST?**  
Выполнить in-order обход: если полученная последовательность строго возрастает, то дерево является BST.

**12. Что такое сбалансированное дерево? Почему это важно для производительности?**  
Дерево, где высоты левого и правого поддеревьев каждого узла отличаются не более чем на 1 (AVL) или поддерживается логарифмическая высота. Важно, потому что гарантирует O(log n) для операций.

**13. Как преобразовать несбалансированное BST в сбалансированное?**  
Выполнить in-order обход в массив, затем рекурсивно строить дерево из середины массива.

**14. Как восстановить дерево по массиву обхода (in-order + pre-order)?**  
Первый элемент pre-order — корень. Находим его в in-order: слева — левое поддерево, справа — правое. Рекурсивно повторяем для подмассивов.

**15. Как реализовать итеративный (не рекурсивный) обход бинарного дерева?**  
Использовать явный стек: для in-order — идти влево, складывая узлы в стек, затем извлекать и переходить вправо. Для level-order — очередь.

---

## Выводы
Реализованы структуры «бинарное дерево» и «BST». Освоены все виды обходов, операции вставки, поиска и удаления (все три случая). Решена задача варианта 11 — восстановление BST из двух переставленных узлов и построение зеркального отображения. Проведён анализ сложности, показаны преимущества сбалансированных деревьев. Получены навыки рекурсивной обработки деревьев и управления памятью.

---

**Ссылка на репозиторий:**  
[https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)
