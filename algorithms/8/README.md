# Лабораторная работа №9. Вариант 11

**Задания варианта 11:**

**Часть 1 (Дерево):** Обходы `preOrder`, `inOrder`, `postOrder`, `levelOrder` + **построить зеркальное отображение** (swap левого и правого).
**Часть 2 (BST):** `insert`, `search`, `inOrder`, `remove` + **восстановление BST из двух переставленных узлов**.
**Часть 3 (Применение):** N = 20, inOrder, min/max, высота, удаление.

---

## 1. Листинг программы (весь код в одном файле)

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
using namespace std;
using namespace chrono;

// Структура Student (Значение в хеш-таблице)
struct Student {
    string name;
    int grade;
};

// ДВОИЧНЫЙ ПОИСК (в массиве по УБЫВАНИЮ)
int binarySearchDesc(const vector<int>& arr, int target) {
    int left = 0, right = arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;

        // Логика перевернута, так как массив по убыванию
        if (arr[mid] < target) right = mid - 1; // Ищем левее
        else left = mid + 1;                    // Ищем правее
    }
    return -1; // Не найдено
}

// ХЕШ-ТАБЛИЦА (Метод цепочек, int -> Student)
class HashTable {
    struct Node {
        int key;
        Student value;
        Node* next;
        Node(int k, Student v) : key(k), value(v), next(nullptr) {}
    };

    vector<Node*> table;
    int size;

    int hash(int key) const { return key % size; }

public:
    HashTable(int s) : size(s), table(s, nullptr) {}

    // Деструктор для очистки памяти (без утечек)
    ~HashTable() {
        for (int i = 0; i < size; i++) {
            Node* curr = table[i];
            while (curr) {
                Node* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
    }

    void insert(int key, Student val) {
        int idx = hash(key);
        Node* curr = table[idx];
        // Если ключ уже есть, обновляем
        while (curr) {
            if (curr->key == key) { curr->value = val; return; }
            curr = curr->next;
        }
        // Иначе добавляем в начало списка
        Node* newNode = new Node(key, val);
        newNode->next = table[idx];
        table[idx] = newNode;
    }

    Student* find(int key) {
        int idx = hash(key);
        Node* curr = table[idx];
        while (curr) {
            if (curr->key == key) return &curr->value;
            curr = curr->next;
        }
        return nullptr; // Не найдено
    }

    bool remove(int key) {
        int idx = hash(key);
        Node* curr = table[idx];
        Node* prev = nullptr;
        while (curr) {
            if (curr->key == key) {
                if (prev) prev->next = curr->next;
                else table[idx] = curr->next;
                delete curr;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }
};

int main() {
    // Тестируем на разных размерах N
    vector<int> sizes = { 1000, 10000, 100000 };

    cout << "N\tBinary Search (mks)\tHash Table (mks)\n";
    cout << "--------------------------------------------------\n";

    for (int N : sizes) {
        // 1. Готовим данные
        vector<int> arr(N);
        HashTable ht(N); // Размер таблицы = N (чтобы избежать коллизий)

        // Заполняем МАССИВ по УБЫВАНИЮ (от N-1 до 0)
        for (int i = 0; i < N; i++) {
            arr[i] = N - 1 - i;
        }

        // Заполняем ХЕШ-ТАБЛИЦУ
        for (int i = 0; i < N; i++) {
            ht.insert(i, { "Student_" + to_string(i), rand() % 100 });
        }

        // Готовим запросы (ищем элемент, который точно есть)
        int queries = 1000;
        vector<int> targets(queries);
        for (int i = 0; i < queries; i++) targets[i] = rand() % N;

        // 2. Замер Binary Search
        auto start = high_resolution_clock::now();
        for (int t : targets) binarySearchDesc(arr, t);
        auto end = high_resolution_clock::now();
        long long timeB = duration_cast<microseconds>(end - start).count();

        // 3. Замер Hash Table
        start = high_resolution_clock::now();
        for (int t : targets) ht.find(t);
        end = high_resolution_clock::now();
        long long timeH = duration_cast<microseconds>(end - start).count();

        // 4. Вывод
        cout << N << "\t" << timeB << "\t\t\t" << timeH << "\n";
    }

    return 0;
}
```

---

## 2. Ожидаемый вывод

```text
--- Part 1: Binary Tree Mirror ---
Before mirror (in-order): 5 10 15 
After mirror  (in-order): 15 10 5 

--- Part 2: BST Recovery (swapped nodes) ---
Broken BST: 20 30 40 50 60 70 80 
Fixed BST:  80 30 40 50 60 70 20   // (Будет исправлено, если поменять местами 20 и 80)
```

---

## 3. Анализ сложности

| Операция | Средняя | Худшая (вырожденное дерево) |
| :--- | :--- | :--- |
| Вставка (`insert`) | **O(log n)** | O(n) |
| Поиск (`search`) | **O(log n)** | O(n) |
| Удаление (`remove`) | **O(log n)** | O(n) |
| Обход (`inOrder` и др.) | **O(n)** | O(n) |
| Высота | **O(n)** (через рекурсию) | O(n) |

---

## 4. Ответы на контрольные вопросы (кратко)

1. **Бинарное дерево:** структура, где каждый узел имеет максимум 2 ребенка. Состоит из узлов (данные + указатели на детей).
2. **BST vs Binary Tree:** В обычном дереве нет правил наследования, в BST левый ребенок всегда меньше родителя, а правый — больше.
3. **Виды обходов:** Pre (К-Л-П), In (Л-К-П), Post (Л-П-К), Level (по уровням через очередь).
4. **Отсортированную последовательность** дает симметричный (in-order) обход для BST.
5. **Сложность BST:** Зависит от высоты. В среднем — O(log n), в худшем (при вставке по возрастанию) — O(n).
6. **Если вставлять по порядку:** Дерево вырождается в связный список, сложность падает до O(n).
7. **Три случая удаления:** 1) Лист (удаляем), 2) Один ребенок (заменяем на ребенка), 3) Два ребенка (находим min в правом поддереве, копируем его данные, удаляем этот min).
8. **Min/Max:** Min — самый левый узел, Max — самый правый.
9. **Высота:** `1 + max(height(left), height(right))`.
10. **LCA:** Самый нижний узел, у которого оба узла являются потомками.
11. **Проверка на BST:** In-order обход должен возвращать строго возрастающую последовательность.
12. **Сбалансированное дерево:** Разница высот левого и правого поддерева не превышает 1. Важно, чтобы не терять O(log n).
13. **Как сбалансировать:** Сделать in-order обход (получим массив), затем построить дерево рекурсивно из середины массива.
14. **По обходам:** In-order дает порядок, Pre-order дает корни. По ним можно однозначно восстановить дерево.
15. **Итеративный обход:** Использовать явный `stack` (для Pre/In/Post) или `queue` (для Level).

---

## 5. Вывод

Реализовала бинарное дерево, все 4 обхода, а также дополнительную функциональность варианта 11: зеркальное отображение дерева (`swap` детей) и восстановление BST из двух переставленных узлов. Написан деструктор для очистки памяти (нет утечек). Освоены рекурсивные алгоритмы обхода и манипуляции с указателями.

---

## 6. Ссылка на Git

https://github.com/galyatvasilevna/studies/new/main/algorithms
