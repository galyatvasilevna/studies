# Лабораторная работа №10  
## AVL-дерево. Префиксное дерево (Trie)

**Дисциплина:** Алгоритмы и структуры данных  
**Вариант:** 11  
**Выполнила:** Тарасенко Галина Васильевна, группа ПИЖ-б-о-25-1(2), направление «Программная инженерия»  
**Проверила:** Ассистент кафедры Белик Ангелина Игоревна  

---

## Цель работы
1. Реализовать самобалансирующееся AVL-дерево с операциями вставки, удаления и поиска.
2. Освоить механизмы балансировки (малые и большие повороты).
3. Изучить структуру данных «префиксное дерево» (Trie) для эффективного хранения и поиска строк.
4. Реализовать операции вставки, поиска и удаления в Trie.
5. Сравнить производительность AVL-дерева, BST и Trie для различных типов данных.

## Задание (Вариант 11)
**Часть 1 (AVL-дерево):** Вставка, балансировка, поиск, удаление. **Доп. задание:** Проверка сбалансированности дерева.
**Часть 2 (Trie):** Вставка, поиск, поиск по префиксу. **Доп. задание:** Поддержка цифр (0-9).
**Часть 3 (Анализ):** Сравнить производительность вставки и поиска AVL и обычного BST для N = 1000, 2000, 3000 на случайных и отсортированных данных.

## Ссылка на репозиторий GitHub
[https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)

---

## Теоретическая часть
- **AVL-дерево** — самобалансирующееся бинарное дерево поиска, в котором разница высот левого и правого поддеревьев (фактор баланса) для каждого узла не превышает 1. Балансировка выполняется с помощью поворотов (LL, RR, LR, RL).
- **Trie (префиксное дерево)** — структура для хранения строк, где каждый узел соответствует символу. Позволяет быстро искать слова и префиксы за O(L), где L — длина строки.

---

## Полный код решения

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
using namespace std;
using namespace chrono;

// ЧАСТЬ 1: AVL-ДЕРЕВО
struct AVLNode {
    int key, h;
    AVLNode* left, * right;
    AVLNode(int k) : key(k), h(1), left(nullptr), right(nullptr) {}
};

class AVL {
    AVLNode* root;
    int height(AVLNode* n) { return n ? n->h : 0; }
    int balance(AVLNode* n) { return n ? height(n->left) - height(n->right) : 0; }
    void update(AVLNode* n) { n->h = 1 + max(height(n->left), height(n->right)); }

    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left; AVLNode* T2 = x->right;
        x->right = y; y->left = T2;
        update(y); update(x); return x;
    }
    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right; AVLNode* T2 = y->left;
        y->left = x; x->right = T2;
        update(x); update(y); return y;
    }

    AVLNode* insert(AVLNode* node, int key) {
        if (!node) return new AVLNode(key);
        if (key < node->key) node->left = insert(node->left, key);
        else if (key > node->key) node->right = insert(node->right, key);
        else return node;
        update(node);
        int bal = balance(node);
        if (bal > 1 && key < node->left->key) return rightRotate(node);
        if (bal < -1 && key > node->right->key) return leftRotate(node);
        if (bal > 1 && key > node->left->key) { node->left = leftRotate(node->left); return rightRotate(node); }
        if (bal < -1 && key < node->right->key) { node->right = rightRotate(node->right); return leftRotate(node); }
        return node;
    }

    AVLNode* minNode(AVLNode* n) { while (n->left) n = n->left; return n; }

    AVLNode* remove(AVLNode* node, int key) {
        if (!node) return node;
        if (key < node->key) node->left = remove(node->left, key);
        else if (key > node->key) node->right = remove(node->right, key);
        else {
            if (!node->left || !node->right) {
                AVLNode* temp = node->left ? node->left : node->right;
                if (!temp) { temp = node; node = nullptr; }
                else *node = *temp;
                delete temp;
            }
            else {
                AVLNode* temp = minNode(node->right);
                node->key = temp->key;
                node->right = remove(node->right, temp->key);
            }
        }
        if (!node) return node;
        update(node);
        int bal = balance(node);
        if (bal > 1 && balance(node->left) >= 0) return rightRotate(node);
        if (bal > 1 && balance(node->left) < 0) { node->left = leftRotate(node->left); return rightRotate(node); }
        if (bal < -1 && balance(node->right) <= 0) return leftRotate(node);
        if (bal < -1 && balance(node->right) > 0) { node->right = rightRotate(node->right); return leftRotate(node); }
        return node;
    }

    // ВАРИАНТ 11: Проверка сбалансированности
    bool isBalanced(AVLNode* n) {
        if (!n) return true;
        if (abs(balance(n)) > 1) return false;
        return isBalanced(n->left) && isBalanced(n->right);
    }

    void destroy(AVLNode* n) { if (n) { destroy(n->left); destroy(n->right); delete n; } }

public:
    AVL() : root(nullptr) {}
    ~AVL() { destroy(root); }
    void insert(int k) { root = insert(root, k); }
    void remove(int k) { root = remove(root, k); }
    bool checkBalance() { return isBalanced(root); }
};

// ЧАСТЬ 2: TRIE (с поддержкой цифр)
class Trie {
    struct Node {
        Node* ch[36]; // 0-9 и a-z
        bool end;
        Node() : end(false) { for (int i = 0; i < 36; i++) ch[i] = nullptr; }
    };
    Node* root;

    // ВАРИАНТ 11: Поддержка цифр и букв
    int idx(char c) {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'z') return c - 'a' + 10;
        return -1;
    }

    void destroy(Node* n) {
        if (!n) return;
        for (int i = 0; i < 36; i++) destroy(n->ch[i]);
        delete n;
    }

public:
    Trie() { root = new Node(); }
    ~Trie() { destroy(root); }

    void insert(const string& s) {
        Node* cur = root;
        for (char c : s) {
            int i = idx(c);
            if (i == -1) continue;
            if (!cur->ch[i]) cur->ch[i] = new Node();
            cur = cur->ch[i];
        }
        cur->end = true;
    }

    bool search(const string& s) {
        Node* cur = root;
        for (char c : s) {
            int i = idx(c);
            if (i == -1) return false;
            if (!cur->ch[i]) return false;
            cur = cur->ch[i];
        }
        return cur->end;
    }

    bool startsWith(const string& prefix) {
        Node* cur = root;
        for (char c : prefix) {
            int i = idx(c);
            if (i == -1) return false;
            if (!cur->ch[i]) return false;
            cur = cur->ch[i];
        }
        return true;
    }
};

// Простой BST для сравнения
class BST {
    struct Node { int key; Node* l, * r; Node(int k) : key(k), l(nullptr), r(nullptr) {} };
    Node* root;
    Node* insert(Node* n, int k) {
        if (!n) return new Node(k);
        if (k < n->key) n->l = insert(n->l, k);
        else n->r = insert(n->r, k);
        return n;
    }
    void destroy(Node* n) { if (n) { destroy(n->l); destroy(n->r); delete n; } }
public:
    BST() : root(nullptr) {}
    ~BST() { destroy(root); }
    void insert(int k) { root = insert(root, k); }
};

template <typename Func> long long measure(Func f) {
    auto t0 = high_resolution_clock::now();
    f();
    auto t1 = high_resolution_clock::now();
    return duration_cast<microseconds>(t1 - t0).count();
}

int main() {
    // --- 1. Демонстрация AVL ---
    AVL avl;
    for (int x : {10, 20, 30, 40, 50, 25}) avl.insert(x);
    avl.remove(30);
    cout << "AVL is balanced: " << (avl.checkBalance() ? "Yes" : "No") << "\n\n";

    // --- 2. Демонстрация Trie ---
    Trie trie;
    trie.insert("abc123");
    trie.insert("xyz789");
    cout << "Trie search abc123: " << (trie.search("abc123") ? "Yes" : "No") << "\n";
    cout << "Trie prefix xyz:    " << (trie.startsWith("xyz") ? "Yes" : "No") << "\n\n";

    // --- 3. Сравнительный анализ (Часть 3) ---
    cout << "N\tBST(rand)\tAVL(rand)\tBST(sort)\tAVL(sort)\n";
    vector<int> sizes = { 1000, 2000, 3000 };

    for (int N : sizes) {
        vector<int> rnd(N), srt(N);
        for (int i = 0; i < N; i++) { rnd[i] = rand() % 100000; srt[i] = i; }

        auto tBstR = measure([&]() { BST b; for (int x : rnd) b.insert(x); });
        auto tAvlR = measure([&]() { AVL a; for (int x : rnd) a.insert(x); });
        auto tBstS = measure([&]() { BST b; for (int x : srt) b.insert(x); });
        auto tAvlS = measure([&]() { AVL a; for (int x : srt) a.insert(x); });

        cout << N << "\t" << tBstR << "\t\t" << tAvlR
            << "\t\t" << tBstS << "\t\t" << tAvlS << "\n";
    }
    return 0;
}
```

---

## Результаты работы

```
AVL is balanced: Yes

Trie search abc123: Yes
Trie prefix xyz:    Yes

N       BST(rand)       AVL(rand)       BST(sort)       AVL(sort)
1000    1842            2523            13290           2374
2000    3066            5206            47847           4797
3000    3933            6759            104960          4970
```

---

## Анализ сложности операций

| Структура | Операция | Средний случай | Худший случай |
|---|---|---|---|
| **BST** | Вставка, поиск, удаление | O(log n) | O(n) — при отсортированных данных |
| **AVL** | Вставка, поиск, удаление | O(log n) | O(log n) — благодаря балансировке |
| **Trie** | Вставка, поиск | O(L) — длина строки | O(L) |

**Вывод из таблицы результатов:** 
- На случайных данных обычный BST работает быстрее AVL за счет меньшего количества накладных расходов на балансировку.
- На **отсортированных данных** BST вырождается в список (время вставки растет квадратично, достигая 104960 мкс), тогда как AVL-дерево сохраняет логарифмическую сложность (всего 4970 мкс) благодаря поворотам.

---

## Ответы на контрольные вопросы

**1. Что такое AVL-дерево? В чём его отличие от обычного BST?**  
Это самобалансирующееся бинарное дерево поиска. Отличие в том, что разница высот поддеревьев (фактор баланса) не превышает 1, что гарантирует высоту O(log n) и защищает от вырождения.

**2. Что такое фактор баланса? Как он вычисляется?**  
Это разница высот левого и правого поддеревьев узла: `BF = height(left) - height(right)`. Допустимые значения: -1, 0, 1.

**3. Какие существуют типы поворотов в AVL-дереве?**  
Малый левый (LL), малый правый (RR), большой левый (LR) и большой правый (RL) повороты.

**4. Когда применяется правый поворот? Левый поворот?**  
Правый: если фактор баланса > 1 и левый потомок сбалансирован (≥ 0). Левый: если фактор баланса < -1 и правый потомок сбалансирован (≤ 0).

**5. Когда применяется левый-правый поворот? Правый-левый?**  
Левый-правый: BF > 1, но левый потомок имеет BF < 0 (перекос "внутрь"). Правый-левый: BF < -1, но правый потомок имеет BF > 0.

**6. Какова сложность операций в AVL-дереве?**  
Поиск, вставка и удаление выполняются за O(log n) всегда, даже в худшем случае.

**7. Почему AVL-дерево называется самобалансирующимся?**  
При каждой вставке или удалении оно автоматически проверяет факторы баланса и выполняет повороты для восстановления равновесия.

**8. Как часто происходит балансировка при вставке?**  
Только когда фактор баланса какого-либо узла выходит за пределы [-1, 1]. В среднем это происходит довольно редко (логарифмически зависит от глубины).

**9. Чем AVL-дерево отличается от красно-чёрного дерева?**  
AVL более строго сбалансировано (быстрее поиск), но требует больше поворотов при вставке/удалении. Красно-чёрное дерево менее строгое (быстрее модификации), часто используется в STL (`std::map`).

**10. В каких случаях AVL-дерево эффективнее BST?**  
Когда данные поступают в отсортированном порядке или когда важна гарантированная производительность O(log n) в худшем случае (в отличие от BST).

**11. Что такое Trie (префиксное дерево)? Для решения каких задач оно используется?**  
Древовидная структура для хранения строк. Используется для автодополнения, поиска слов по префиксу, орфографических проверок и маршрутизации IP.

**12. Какова сложность поиска в Trie? От чего она зависит?**  
O(L), где L — длина искомой строки. Сложность не зависит от количества хранимых слов.

**13. Какая структура данных эффективнее для строк: Trie или хеш-таблица?**  
Хеш-таблица быстрее для точного поиска (O(1)), но Trie эффективнее для поиска по префиксу и автодополнения.

**14. Как реализовать автодополнение с помощью Trie?**  
Найти узел, соответствующий префиксу, а затем выполнить обход (DFS) всех его потомков для сбора полных слов.

**15. Каковы недостатки Trie? Какой объём памяти он требует?**  
Основной недостаток — высокий расход памяти, так как каждый узел хранит массив указателей (алфавит). Объём памяти зависит от количества слов и их длины.

---

**Ссылка на репозиторий:**  
[https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)
```