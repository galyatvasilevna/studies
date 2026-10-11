```markdown
# Лабораторная работа №2  
## Анализ сложности алгоритмов. Шаблоны классов

**Вариант 11**  
**Дисциплина:** Алгоритмы и структуры данных  
**Выполнила:** Тарасенко Г. В., группа ПИЖ-6-о-25-1(2)  
**Преподаватель:** ассистент Белик А. И.

---

## Цель работы
Научиться оценивать эффективность алгоритмов с помощью O-нотации, освоить классы сложности. Провести экспериментальный анализ производительности. Освоить создание шаблонов функций и классов. Получить навыки сравнения производительности различных реализаций одной задачи.

## Задание (Вариант 11)
Реализовать шаблонный контейнер `RingBuffer<T>` (циклический буфер) с операциями:
- `push` — добавление в хвост,
- `pop` — извлечение из головы,
- `operator[]` — доступ по логическому индексу,
- `isEmpty`, `isFull`, `size`.

Провести анализ сложности каждой операции. Экспериментально сравнить производительность с аналогом из STL (`std::queue`).

---

## Полный код решения

```cpp
#include <iostream>
#include <queue>              // STL-аналог
#include <chrono>
#include <stdexcept>
using namespace std;
using namespace chrono;

// ============================================================
// Шаблонный контейнер RingBuffer<T> фиксированной ёмкости
// ============================================================
template <typename T>
class RingBuffer {
    T* data;          // буфер фиксированного размера
    int capacity;     // ёмкость
    int head;         // индекс первого элемента
    int count;        // текущее число элементов

public:
    // Конструктор: выделяем память под capacity элементов
    RingBuffer(int cap) : capacity(cap), head(0), count(0) {
        if (cap <= 0) throw invalid_argument("capacity must be > 0");
        data = new T[capacity];
    }

    ~RingBuffer() { delete[] data; }

    // push: добавить в хвост. O(1)
    // Если буфер полон — вытесняем самый старый (перезапись).
    void push(const T& v) {
        int tail = (head + count) % capacity;
        data[tail] = v;
        if (isFull()) head = (head + 1) % capacity;  // сдвигаем голову
        else          count++;
    }

    // pop: извлечь из головы. O(1)
    T pop() {
        if (isEmpty()) throw out_of_range("RingBuffer is empty");
        T v = data[head];
        head = (head + 1) % capacity;
        count--;
        return v;
    }

    // operator[]: доступ по логическому индексу. O(1)
    T& operator[](int i) {
        if (i < 0 || i >= count) throw out_of_range("index out of range");
        return data[(head + i) % capacity];
    }
    const T& operator[](int i) const {
        if (i < 0 || i >= count) throw out_of_range("index out of range");
        return data[(head + i) % capacity];
    }

    // isEmpty / isFull: O(1)
    bool isEmpty() const { return count == 0; }
    bool isFull()  const { return count == capacity; }
    int  size()    const { return count; }
};

// ============================================================
// Замер времени (микросекунды)
// ============================================================
template <typename Func>
long long measure(Func f) {
    auto t0 = high_resolution_clock::now();
    f();
    auto t1 = high_resolution_clock::now();
    return duration_cast<microseconds>(t1 - t0).count();
}

int main() {
    setlocale(LC_ALL, "ru");

    // ---------- 1. Демонстрация работы ----------
    cout << "=== RingBuffer<int> (capacity = 4) ===\n";
    RingBuffer<int> rb(4);

    for (int i = 1; i <= 6; i++) {           // переполнение: 5 и 6 вытеснят 1 и 2
        rb.push(i * 10);
        cout << "push(" << i * 10 << ") -> size = " << rb.size()
            << ", isFull = " << rb.isFull() << "\n";
    }

    cout << "operator[]: ";
    for (int i = 0; i < rb.size(); i++) cout << rb[i] << " ";
    cout << "\npop() = " << rb.pop() << "\n";
    cout << "after pop, size = " << rb.size()
        << ", isEmpty = " << rb.isEmpty() << "\n\n";

    // ---------- 2. Сравнение с STL ----------
    const int N = 1'000'000;   // всего операций
    const int CAP = 1024;      // ёмкость буфера

    // Мой RingBuffer
    long long myTime = measure([&]() {
        RingBuffer<int> my(CAP);
        for (int i = 0; i < N; i++) {
            my.push(i);
            if (my.isFull()) my.pop();
        }
        while (!my.isEmpty()) my.pop();
        });

    // STL std::queue (FIFO на deque)
    long long stlTime = measure([&]() {
        queue<int> q;
        for (int i = 0; i < N; i++) {
            q.push(i);
            if ((int)q.size() >= CAP) q.pop();
        }
        while (!q.empty()) q.pop();
        });

    cout << "=== Сравнение при N = " << N << ", capacity = " << CAP << " ===\n";
    cout << "RingBuffer : " << myTime << " мкс\n";
    cout << "std::queue : " << stlTime << " мкс\n";

    return 0;
}
```

---

## Анализ сложности операций

| Операция   | Сложность | Обоснование |
|------------|-----------|-------------|
| `push`     | O(1)      | Хвост = `(head + count) % capacity` — одна арифметическая операция; аллокаций нет |
| `pop`      | O(1)      | Голова = `(head + 1) % capacity` — одна арифметическая операция |
| `operator[]` | O(1)    | Индекс = `(head + i) % capacity` — прямой доступ к `data[index]` |
| `isEmpty`  | O(1)      | Сравнение `count == 0` |
| `isFull`   | O(1)      | Сравнение `count == capacity` |
| `size`     | O(1)      | Возврат поля `count` |

---

## Пример вывода
```
=== RingBuffer<int> (capacity = 4) ===
push(10) -> size = 1, isFull = 0
push(20) -> size = 2, isFull = 0
push(30) -> size = 3, isFull = 0
push(40) -> size = 4, isFull = 1
push(50) -> size = 4, isFull = 1
push(60) -> size = 4, isFull = 1
operator[]: 30 40 50 60
pop() = 30
after pop, size = 3, isEmpty = 0

=== Сравнение при N = 1000000, capacity = 1024 ===
RingBuffer : 56118 мкс
std::queue : 730244 мкс
```

## Выводы
Научилась оценивать эффективность алгоритмов с помощью O-нотации, освоила классы сложности. Провела экспериментальный анализ производительности. Освоила создание шаблонов функций и классов. Получила навыки сравнения производительности различных реализаций одной задачи. Собственный `RingBuffer` на фиксированном массиве оказался быстрее `std::queue` за счёт отсутствия динамического выделения памяти на каждом шаге.

---

**Ссылка на репозиторий:**  
[https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)
```
