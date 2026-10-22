#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
using namespace std;

// 1. СТЕК (массив с расширением)
template <typename T>
class Stack {
    T* data; int top_i; int cap;
    void grow() {
        cap = cap ? cap * 2 : 1;
        T* nd = new T[cap];
        for (int i = 0; i <= top_i; i++) nd[i] = data[i];
        delete[] data; data = nd;
    }
public:
    Stack() : data(nullptr), top_i(-1), cap(0) {}
    ~Stack() { delete[] data; }
    void push(const T& v) { if (top_i + 1 >= cap) grow(); data[++top_i] = v; }
    T pop() { if (empty()) throw underflow_error("stack empty"); return data[top_i--]; }
    T top() const { if (empty()) throw underflow_error("stack empty"); return data[top_i]; }
    bool empty() const { return top_i < 0; }
    int  size()  const { return top_i + 1; }
};

// Задача: инфикс -> постфикс (сортировочная станция Дейкстры)
int prec(char op) { return (op == '+' || op == '-') ? 1 : (op == '*' || op == '/') ? 2 : 0; }

string infixToPostfix(const string& s) {
    Stack<char> ops; string out;
    for (char c : s) {
        if (isdigit(c)) out += c;
        else if (c == '(') ops.push(c);
        else if (c == ')') { while (ops.top() != '(') { out += ops.pop(); out += ' '; } ops.pop(); }
        else if (strchr("+-*/", c)) {
            while (!ops.empty() && prec(ops.top()) >= prec(c)) { out += ops.pop(); out += ' '; }
            ops.push(c);
        }
    }
    while (!ops.empty()) { out += ops.pop(); out += ' '; }
    return out;
}

// 2. ОЧЕРЕДЬ (кольцевой буфер)
template <typename T>
class Queue {
    T* data; int front_i, count, cap;
    void grow() {
        cap = cap ? cap * 2 : 1;
        T* nd = new T[cap];
        for (int i = 0; i < count; i++) nd[i] = data[(front_i + i) % (cap / 2 ? cap / 2 : 1)];
        delete[] data; data = nd; front_i = 0;
    }
public:
    Queue() : data(nullptr), front_i(0), count(0), cap(0) {}
    ~Queue() { delete[] data; }
    void enqueue(const T& v) { if (count >= cap) grow(); data[(front_i + count) % cap] = v; count++; }
    T dequeue() { if (empty()) throw underflow_error("queue empty"); T v = data[front_i]; front_i = (front_i + 1) % cap; count--; return v; }
    T front() const { if (empty()) throw underflow_error("queue empty"); return data[front_i]; }
    bool empty() const { return count == 0; }
    int  size()  const { return count; }
};

// Задача: слить две очереди, чередуя элементы
Queue<int> mergeAlternate(Queue<int>& a, Queue<int>& b) {
    Queue<int> r;
    while (!a.empty() || !b.empty()) {
        if (!a.empty()) r.enqueue(a.dequeue());
        if (!b.empty()) r.enqueue(b.dequeue());
    }
    return r;
}

// 3. ДЕК (двусвязный список)
template <typename T>
class Deque {
    struct Node { T data; Node* prev, * next; Node(const T& v) :data(v), prev(nullptr), next(nullptr) {} };
    Node* head, * tail; int cnt;
public:
    Deque() : head(nullptr), tail(nullptr), cnt(0) {}
    ~Deque() { while (head) { Node* t = head; head = head->next; delete t; } }
    void push_front(const T& v) { Node* n = new Node(v); n->next = head; if (head) head->prev = n; else tail = n; head = n; cnt++; }
    void push_back(const T& v) { Node* n = new Node(v); n->prev = tail; if (tail) tail->next = n; else head = n; tail = n; cnt++; }
    T pop_front() { if (empty()) throw underflow_error("deque empty"); T v = head->data; Node* t = head; head = head->next; if (head) head->prev = nullptr; else tail = nullptr; delete t; cnt--; return v; }
    T pop_back() { if (empty()) throw underflow_error("deque empty"); T v = tail->data; Node* t = tail; tail = tail->prev; if (tail) tail->next = nullptr; else head = nullptr; delete t; cnt--; return v; }
    T front() const { if (empty()) throw underflow_error("deque empty"); return head->data; }
    T back()  const { if (empty()) throw underflow_error("deque empty"); return tail->data; }
    bool empty() const { return cnt == 0; }
    int  size()  const { return cnt; }
};

// Задача: минимумы скользящего окна размера k
void windowMin(const int* a, int n, int k) {
    Deque<int> dq;   // хранит индексы
    cout << "mins: ";
    for (int i = 0; i < n; i++) {
        while (!dq.empty() && dq.front() <= i - k) dq.pop_front();
        while (!dq.empty() && a[dq.back()] >= a[i])  dq.pop_back();
        dq.push_back(i);
        if (i >= k - 1) cout << a[dq.front()] << " ";
    }
    cout << "\n";
}

// 4. ПРИОРИТЕТНАЯ ОЧЕРЕДЬ фиксированной ёмкости (max-heap)
template <typename T>
class PriorityQueue {
    T* heap; int cnt; const int cap;   // cap фиксирован
    void up(int i) { while (i > 0) { int p = (i - 1) / 2; if (heap[i] <= heap[p]) break; swap(heap[i], heap[p]); i = p; } }
    void down(int i) {
        while (true) {
            int l = 2 * i + 1, r = 2 * i + 2, m = i;
            if (l < cnt && heap[l] > heap[m]) m = l;
            if (r < cnt && heap[r] > heap[m]) m = r;
            if (m == i) break; swap(heap[i], heap[m]); i = m;
        }
    }
public:
    explicit PriorityQueue(int c) : heap(new T[c]), cnt(0), cap(c) {}
    ~PriorityQueue() { delete[] heap; }
    void push(const T& v) {
        if (cnt >= cap) throw overflow_error("priority queue full");
        heap[cnt] = v; up(cnt); cnt++;
    }
    T pop() { if (empty()) throw underflow_error("pq empty"); T top = heap[0]; heap[0] = heap[--cnt]; down(0); return top; }
    T top() const { if (empty()) throw underflow_error("pq empty"); return heap[0]; }
    bool empty() const { return cnt == 0; }
    int  size()  const { return cnt; }
    int  capacity() const { return cap; }
};

int main() {
    // 1. Стек: инфикс -> постфикс
    cout << "1. Postfix: " << infixToPostfix("3+4*(2-1)") << "\n";

    // 2. Очередь: объединение двух очередей
    Queue<int> q1, q2;
    for (int x : {1, 3, 5}) q1.enqueue(x);
    for (int x : {2, 4, 6}) q2.enqueue(x);
    Queue<int> merged = mergeAlternate(q1, q2);
    cout << "2. Merged:  ";
    while (!merged.empty()) cout << merged.dequeue() << " ";
    cout << "\n";

    // 3. Дек: минимумы скользящего окна
    int a[] = { 5, 2, 8, 1, 9, 3, 7, 4 };
    cout << "3. "; windowMin(a, 8, 3);

    // 4. Приоритетная очередь фиксированной ёмкости
    cout << "4. PQ pop:  ";
    PriorityQueue<int> pq(4);              // фиксировано 4 элемента
    for (int x : {30, 10, 50, 20}) pq.push(x);
    while (!pq.empty()) cout << pq.pop() << " ";
    cout << "\n";

    // обработка пустых структур
    try { Stack<int> s; s.pop(); }
    catch (const exception& e) { cout << "stack: " << e.what() << "\n"; }
    try { Queue<int> q; q.dequeue(); }
    catch (const exception& e) { cout << "queue: " << e.what() << "\n"; }
    try { Deque<int> d; d.pop_front(); }
    catch (const exception& e) { cout << "deque: " << e.what() << "\n"; }
    try { PriorityQueue<int> p(2); p.pop(); }
    catch (const exception& e) { cout << "pq:    " << e.what() << "\n"; }

    return 0;
}
