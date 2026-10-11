```markdown
# Лабораторная работа №4  
## Односвязный и двусвязный списки

**Вариант 11**  
**Дисциплина:** Алгоритмы и структуры данных  
**Выполнила:** Тарасенко Г. В., группа ПИЖ-6-о-25-1(2)  
**Преподаватель:** ассистент Белик А. И.

---

## Цель работы
Реализовать шаблонные классы `SinglyLinkedList` и `DoublyLinkedList` с базовыми операциями. Освоить вставку, удаление, поиск и обход. Сравнить производительность и особенности списков. Научиться управлять памятью при удалении узлов.

## Задание (Вариант 11)
1. Реализовать оба класса с полной функциональностью.
2. Сравнить производительность операций односвязного и двусвязного списков.

---

## Полный код решения

```cpp
#include <iostream>
#include <stdexcept>
#include <chrono>
using namespace std;
using namespace chrono;

// ==================== ОДНОСВЯЗНЫЙ СПИСОК ====================
template <typename T>
class SinglyLinkedList {
    struct Node {
        T data;
        Node* next;
        Node(const T& v) : data(v), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    int cnt;

public:
    class Iterator {
        Node* p;
    public:
        Iterator(Node* n = nullptr) : p(n) {}
        T& operator*() { return p->data; }
        Iterator& operator++() { p = p->next; return *this; }
        bool operator!=(const Iterator& o) const { return p != o.p; }
    };

    SinglyLinkedList() : head(nullptr), tail(nullptr), cnt(0) {}
    ~SinglyLinkedList() { clear(); }

    void push_front(const T& v) {
        Node* n = new Node(v);
        n->next = head;
        head = n;
        if (tail == nullptr) tail = n;
        cnt++;
    }

    void push_back(const T& v) {
        Node* n = new Node(v);
        if (head == nullptr) {
            head = tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
        cnt++;
    }

    void pop_front() {
        if (head == nullptr) throw underflow_error("empty");
        Node* t = head;
        head = head->next;
        if (head == nullptr) tail = nullptr;
        delete t;
        cnt--;
    }

    bool find(const T& v) const {
        for (Node* c = head; c; c = c->next)
            if (c->data == v) return true;
        return false;
    }

    void clear() {
        while (head) { Node* t = head; head = head->next; delete t; }
        tail = nullptr; cnt = 0;
    }

    int size() const { return cnt; }
    bool empty() const { return head == nullptr; }

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }

    void print() const {
        cout << "[";
        for (Node* c = head; c; c = c->next) {
            cout << c->data;
            if (c->next) cout << ", ";
        }
        cout << "]\n";
    }
};

// ==================== ДВУСВЯЗНЫЙ СПИСОК ====================
template <typename T>
class DoublyLinkedList {
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& v) : data(v), prev(nullptr), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    int cnt;

public:
    class Iterator {
        Node* p;
    public:
        Iterator(Node* n = nullptr) : p(n) {}
        T& operator*() { return p->data; }
        Iterator& operator++() { p = p->next; return *this; }
        Iterator& operator--() { p = p->prev; return *this; }
        bool operator!=(const Iterator& o) const { return p != o.p; }
    };

    DoublyLinkedList() : head(nullptr), tail(nullptr), cnt(0) {}
    ~DoublyLinkedList() { clear(); }

    void push_front(const T& v) {
        Node* n = new Node(v);
        if (head) { n->next = head; head->prev = n; head = n; }
        else { head = tail = n; }
        cnt++;
    }

    void push_back(const T& v) {
        Node* n = new Node(v);
        if (tail) { tail->next = n; n->prev = tail; tail = n; }
        else { head = tail = n; }
        cnt++;
    }

    void pop_front() {
        if (head == nullptr) throw underflow_error("empty");
        Node* t = head;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        delete t;
        cnt--;
    }

    void pop_back() {
        if (tail == nullptr) throw underflow_error("empty");
        Node* t = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        delete t;
        cnt--;
    }

    bool find(const T& v) const {
        for (Node* c = head; c; c = c->next)
            if (c->data == v) return true;
        return false;
    }

    void printReverse() const {
        cout << "[";
        for (Node* c = tail; c; c = c->prev) {
            cout << c->data;
            if (c->prev) cout << ", ";
        }
        cout << "]\n";
    }

    void clear() {
        while (head) { Node* t = head; head = head->next; delete t; }
        tail = nullptr; cnt = 0;
    }

    int size() const { return cnt; }
    bool empty() const { return head == nullptr; }

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }

    void print() const {
        cout << "[";
        for (Node* c = head; c; c = c->next) {
            cout << c->data;
            if (c->next) cout << ", ";
        }
        cout << "]\n";
    }
};

// ==================== ЗАМЕР ВРЕМЕНИ ====================
template <typename Func>
long long measure(Func f) {
    auto t0 = high_resolution_clock::now();
    f();
    auto t1 = high_resolution_clock::now();
    return duration_cast<microseconds>(t1 - t0).count();
}

// ==================== MAIN ====================
int main() {
    setlocale(LC_ALL, "ru");

    // 1. Демонстрация односвязного
    cout << "=== SinglyLinkedList ===\n";
    SinglyLinkedList<int> s;
    s.push_back(10); s.push_back(20); s.push_back(30); s.push_front(5);
    cout << "print: "; s.print();
    cout << "size = " << s.size() << ", find(20) = " << s.find(20) << "\n";
    cout << "iterator: ";
    for (auto it = s.begin(); it != s.end(); ++it) cout << *it << " ";
    cout << "\n";
    s.pop_front();
    cout << "after pop_front: "; s.print();

    // 2. Демонстрация двусвязного
    cout << "\n=== DoublyLinkedList ===\n";
    DoublyLinkedList<int> d;
    for (int i = 1; i <= 5; i++) d.push_back(i * 10);
    cout << "print:         "; d.print();
    cout << "printReverse: "; d.printReverse();
    d.pop_back();
    cout << "after pop_back: "; d.print();

    // 3. Сравнение производительности (задание варианта 11)
    cout << "\n=== Сравнение производительности (N = 100000) ===\n";
    const int N = 100000;

    long long tS = measure([&]() {
        SinglyLinkedList<int> sl;
        for (int i = 0; i < N; i++) sl.push_back(1);
        while (!sl.empty()) sl.pop_front();
    });

    long long tD_front = measure([&]() {
        DoublyLinkedList<int> dl;
        for (int i = 0; i < N; i++) dl.push_back(1);
        while (!dl.empty()) dl.pop_front();
    });

    long long tD_back = measure([&]() {
        DoublyLinkedList<int> dl;
        for (int i = 0; i < N; i++) dl.push_back(1);
        while (!dl.empty()) dl.pop_back();
    });

    cout << "Singly  push_back + pop_front: " << tS << " мкс\n";
    cout << "Doubly  push_back + pop_front: " << tD_front << " мкс\n";
    cout << "Doubly  push_back + pop_back : " << tD_back << " мкс\n";
    cout << "Вывод: удаление из конца O(1) возможно только в двусвязном списке.\n";

    return 0;
}
```

---

## Анализ сложности

| Операция | Singly | Doubly | Обоснование |
|----------|--------|--------|-------------|
| `push_front` | O(1) | O(1) | Обновляем только голову |
| `push_back` | O(1) | O(1) | Есть указатель на `tail` |
| `pop_front` | O(1) | O(1) | Удаляем голову |
| `pop_back` | O(n) | O(1) | В singly нужно дойти до предпоследнего, в doubly — `tail->prev` |
| `find` | O(n) | O(n) | Линейный обход |
| `print` | O(n) | O(n) | Обход всех элементов |
| `printReverse` | — | O(n) | Возможен только в doubly |
| `operator++` | O(1) | O(1) | Один шаг по `next` |
| `operator--` | — | O(1) | В singly невозможен |
| `clear` | O(n) | O(n) | Удалить все узлы |

---

## Пример вывода программы

```
=== SinglyLinkedList ===
print: [5, 10, 20, 30]
size = 4, find(20) = 1
iterator: 5 10 20 30 
after pop_front: [10, 20, 30]

=== DoublyLinkedList ===
print:         [10, 20, 30, 40, 50]
printReverse: [50, 40, 30, 20, 10]
after pop_back: [10, 20, 30, 40]

=== Сравнение производительности (N = 100000) ===
Singly  push_back + pop_front: 81901 мкс
Doubly  push_back + pop_front: 122734 мкс
Doubly  push_back + pop_back : 93804 мкс
Вывод: удаление из конца O(1) возможно только в двусвязном списке.
```

---

## Выводы
Реализованы шаблонные классы односвязного и двусвязного списков с полным набором операций. Освоены вставка, удаление, поиск, обход, а также управление памятью. Сравнение производительности показало, что двусвязный список быстрее при удалении с конца, но требует больше памяти на узел.

---

**Ссылка на репозиторий:**  
[https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)
```
