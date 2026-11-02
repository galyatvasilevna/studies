# Лабораторная работа №8  
## Алгоритмы поиска: бинарный поиск и хеш-таблица

**Вариант 11**  
**Дисциплина:** Алгоритмы и структуры данных  
**Выполнила:** Тарасенко Г. В., группа ПИЖ-6-о-25-1(2)  
**Преподаватель:** ассистент Белик А. И.

---

## Цель работы
Реализовать и сравнить алгоритмы поиска: бинарный поиск в упорядоченном массиве и хеш-таблицу. Освоить методы разрешения коллизий (метод цепочек), оценить временную сложность операций поиска, вставки и удаления. Провести экспериментальное сравнение производительности на данных разных размеров.

## Задание (Вариант 11)
1. Реализовать бинарный поиск в массиве, отсортированном **по убыванию**.
2. Реализовать хеш-таблицу методом цепочек (ключ — `int`, значение — структура `Student`).
3. Сравнить время выполнения поиска при `N = 1000, 10000, 100000`.

---

## Теоретическая часть

| Алгоритм | Средний случай | Худший случай | Память | Условия применения |
|---|---|---|---|---|
| Бинарный поиск | O(log n) | O(log n) | O(1) | Массив отсортирован |
| Хеш-таблица (цепочки) | O(1) | O(n) (все коллизии) | O(n) | Есть хеш-функция |

- **Бинарный поиск** работает только на упорядоченных данных. На каждом шаге область поиска сокращается вдвое. Для массива по убыванию логика сравнений инвертируется.
- **Хеш-таблица** обеспечивает в среднем O(1) за счёт вычисления индекса по ключу. Метод цепочек хранит коллизии в связных списках. При хорошей хеш-функции и достаточном размере таблицы (`size ≈ N`) средняя длина цепочки мала.

---

## Полный код решения

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

## Анализ сложности

| Операция | Бинарный поиск | Хеш-таблица (среднее) | Хеш-таблица (худшее) |
|---|---|---|---|
| Поиск | O(log n) | O(1) | O(n) |
| Вставка | — | O(1) | O(n) |
| Удаление | — | O(1) | O(n) |
| Память | O(1) | O(n) | O(n) |

---

## Пример вывода программы

```
N	Binary Search (mks)	Hash Table (mks)
--------------------------------------------------
1000	250			48
10000	370			181
100000	528			245
```

**Интерпретация:**
- Время бинарного поиска растёт логарифмически (≈ в 1.5–2 раза при каждом увеличении N в 10 раз).
- Хеш-таблица работает в среднем за константу, но с ростом N видны накладные расходы на выделение памяти и кеш-промахи.
- При N = 1000 хеш-таблица быстрее в 5 раз, при N = 100000 — примерно в 2 раза, что объясняется ростом количества коллизий и ухудшением локальности.

---

## Выводы
Реализованы бинарный поиск (для массива по убыванию) и хеш-таблица методом цепочек. Проведено экспериментальное сравнение: хеш-таблица показывает более высокую скорость поиска, особенно на малых и средних объёмах данных, однако требует дополнительной памяти и зависит от качества хеш-функции. Бинарный поиск проще по памяти и стабилен по времени, но требует упорядоченного массива.

## Ответы на контрольные вопросы (кратко)
1. **Бинарный поиск** — O(log n), требует отсортированного массива.
2. **Массив по убыванию** — условие ветвления инвертируется: если `arr[mid] < target`, идём влево.
3. **Хеш-таблица** — O(1) в среднем, O(n) при коллизиях.
4. **Метод цепочек** — коллизии хранятся в связных списках, что упрощает вставку и удаление.
5. **Хеш-функция** `key % size` — простая, но при `size = N` коллизий немного.
6. **Рост времени** — бинарный поиск логарифмический, хеш-таблица константная, но с накладными расходами на память.
7. **Когда бинарный поиск лучше?** — при статическом наборе данных, где поиск выполняется многократно и память ограничена.
8. **Когда хеш-таблица лучше?** — при частых вставках/удалениях и поиске, когда память не критична.

---

**Ссылка на репозиторий:**  
[https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)
