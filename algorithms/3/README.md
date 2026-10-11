```markdown
# Лабораторная работа №3  
## Реализация динамического массива (аналог vector). Итераторы

**Вариант 11**  
**Дисциплина:** Алгоритмы и структуры данных  
**Выполнила:** Тарасенко Г. В., группа ПИЖ-6-о-25-1(2)  
**Преподаватель:** ассистент Белик А. И.

---

## Цель работы
Реализовать шаблонный класс `Vector<T>` — аналог `std::vector` — с нуля. Освоить управление динамической памятью, «правило трёх» и «правило пяти». Реализовать итераторы для поддержки range-based for и STL-совместимости. Провести амортизированный анализ `push_back`. Для варианта 11 — реализовать быструю сортировку внутри класса.

## Задание (Вариант 11)
Реализовать класс `Vector` с поддержкой:
- «правила пяти» (деструктор, копирующий/перемещающий конструкторы, копирующее/перемещающее присваивание);
- итераторов (`begin()`, `end()`);
- методов `push_back`, `pop_back`, `size`, `operator[]`;
- **быстрой сортировки** внутри класса (`sort()`).

Проверить работу на примерах: сортировка, копирование, перемещение, обработка исключений.

---

## Полный код решения

```cpp
#include <iostream>
#include <stdexcept>
#include <utility>
using namespace std;

template <typename T>
class Vector {
    T* data;   // буфер в куче
    int sz;     // размер
    int cap;    // ёмкость

    // расширение буфера вдвое (O(n), амортизируется push_back
    void grow() {
        int nc = (cap == 0) ? 1 : cap * 2;
        T* nd = new T[nc];
        for (int i = 0; i < sz; i++) nd[i] = std::move(data[i]);
        delete[] data;
        data = nd;
        cap = nc;
    }

    // быстрая сортировка диапазона [lo, hi]
    void quickSort(int lo, int hi) {
        if (lo >= hi) return;
        T pivot = data[(lo + hi) / 2];
        int i = lo, j = hi;
        while (i <= j) {
            while (data[i] < pivot) i++;
            while (data[j] > pivot) j--;
            if (i <= j) { std::swap(data[i], data[j]); i++; j--; }
        }
        quickSort(lo, j);
        quickSort(i, hi);
    }

public:
    // --- конструкторы ---
    Vector() : data(nullptr), sz(0), cap(0) {}

    explicit Vector(int n) : data(new T[n]), sz(n), cap(n) {
        for (int i = 0; i < n; i++) data[i] = T();
    }

    // копирование (глубокое)
    Vector(const Vector& o) : data(new T[o.cap]), sz(o.sz), cap(o.cap) {
        for (int i = 0; i < sz; i++) data[i] = o.data[i];
    }

    // перемещение
    Vector(Vector&& o) noexcept : data(o.data), sz(o.sz), cap(o.cap) {
        o.data = nullptr; o.sz = o.cap = 0;
    }

    // --- деструктор ---
    ~Vector() { delete[] data; }

    // --- операторы присваивания ---
    Vector& operator=(const Vector& o) {
        if (this != &o) { Vector t(o); swap(t); }   // copy-and-swap + защита
        return *this;
    }
    Vector& operator=(Vector&& o) noexcept {
        if (this != &o) {
            delete[] data;
            data = o.data; sz = o.sz; cap = o.cap;
            o.data = nullptr; o.sz = o.cap = 0;
        }
        return *this;
    }

    // --- обмен ---
    void swap(Vector& o) noexcept {
        std::swap(data, o.data);
        std::swap(sz, o.sz);
        std::swap(cap, o.cap);
    }

    // --- доступ по индексу с проверкой ---
    T& operator[](int i) {
        if (i < 0 || i >= sz) throw out_of_range("index out of range");
        return data[i];
    }
    const T& operator[](int i) const {
        if (i < 0 || i >= sz) throw out_of_range("index out of range");
        return data[i];
    }

    // --- базовые методы ---
    int size() const { return sz; }

    void push_back(const T& v) {
        if (sz >= cap) grow();
        data[sz++] = v;
    }
    void pop_back() {
        if (sz == 0) throw underflow_error("Vector is empty");
        sz--;
    }

    // --- итераторы ---
    T* begin() { return data; }
    T* end() { return data + sz; }
    const T* begin() const { return data; }
    const T* end()   const { return data + sz; }

    // --- продвинутый уровень: сортировка ---
    void sort() { quickSort(0, sz - 1); }
};

int main() {
    // 1. push_back + range-for
    Vector<int> v;
    for (int x : {5, 2, 9, 1, 7, 3, 8, 4, 6, 0}) v.push_back(x);
    cout << "size = " << v.size() << "\n";

    // 2. сортировка
    v.sort();
    cout << "sorted: ";
    for (int x : v) cout << x << " ";
    cout << "\n";

    // 3. копирование (правило трёх)
    Vector<int> c = v;
    c.push_back(100);
    cout << "copy size = " << c.size() << ", orig size = " << v.size() << "\n";

    // 4. перемещение (правило пяти)
    Vector<int> m = std::move(c);
    cout << "moved size = " << m.size() << ", after-move c size = " << c.size() << "\n";

    // 5. pop_back
    v.pop_back();
    cout << "after pop_back size = " << v.size() << "\n";

    // 6. обработка исключения
    try {
        Vector<int> s(2);
        cout << s[5] << "\n";
    }
    catch (const out_of_range& e) {
        cout << "caught: " << e.what() << "\n";
    }

    return 0;
}
```

---

## Анализ сложности операций

| Операция       | Сложность | Обоснование |
|----------------|-----------|-------------|
| `push_back`    | O(1) амортизированно | Удвоение ёмкости даёт суммарную O(n) на n вставок |
| `pop_back`     | O(1)      | Уменьшение размера |
| `operator[]`   | O(1)      | Прямой доступ по индексу |
| `sort()`       | O(n log n) средняя, O(n²) худшая | Быстрая сортировка |
| `size()`       | O(1)      | Возврат поля |

---

## Пример вывода программы

```
size = 10
sorted: 0 1 2 3 4 5 6 7 8 9
copy size = 11, orig size = 10
moved size = 11, after-move c size = 0
after pop_back size = 9
caught: index out of range
```

---

## Выводы
В ходе работы реализован шаблонный динамический массив с полным управлением памятью. Освоены «правило трёх» и «правило пяти», итераторы, быстрая сортировка. Проведён амортизированный анализ `push_back`. Получены навыки безопасной работы с ресурсами (RAII) и обработки исключений.

---

## Ответы на контрольные вопросы (кратко)
1. Массив динамический, т.к. размер определяется во время выполнения, память выделяется в куче.
2. `size` — количество элементов, `capacity` — вместимость без перераспределения.
3. Удвоение ёмкости даёт амортизированное O(1) для `push_back`.
4. Правило трёх: деструктор, копирующий конструктор, копирующее присваивание.
5. Правило пяти добавляет перемещающие конструктор и присваивание.
6. `if (this != &other)` защищает от самоприсваивания.
7. Итераторы — указатели `T*`, `begin()`/`end()` задают диапазон.
8. `noexcept` на move-операциях позволяет STL безопасно перемещать объекты.

---

**Ссылка на репозиторий:**  
[https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)
```
