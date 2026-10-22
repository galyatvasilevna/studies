# Лабораторная работа №7  
## Эффективные алгоритмы сортировки: Quick Sort, Merge Sort, Heap Sort

**Вариант 11**  
**Дисциплина:** Алгоритмы и структуры данных  
**Выполнила:** Тарасенко Г. В., группа ПИЖ-6-о-25-1(2)  
**Преподаватель:** ассистент Белик А. И.

---

## Цель работы
Реализовать три эффективных алгоритма сортировки: быструю (Quick Sort), слиянием (Merge Sort) и пирамидальную (Heap Sort). Освоить принципы «разделяй и властвуй», изучить структуру данных «куча» (heap), проанализировать и сравнить производительность алгоритмов, понять их преимущества и недостатки в различных сценариях.

## Задание (Вариант 11)
- Реализовать все три сортировки с полной функциональностью.
- Провести углублённый анализ.
- Реализовать профилирование и оптимизацию (кэш-промахи, ветвления).

---

## Теоретическая часть

| Алгоритм | Лучший | Средний | Худший | Память | Устойчивость |
|---|---|---|---|---|---|
| Quick Sort | O(n log n) | O(n log n) | O(n²) | O(log n) | Нет |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) | O(n) | Да |
| Heap Sort | O(n log n) | O(n log n) | O(n log n) | O(1) | Нет |

---

## Полный код решения

```cpp
#include <iostream>
#include <algorithm>
#include <chrono>
using namespace std;
using namespace chrono;

// ==================== QUICK SORT (оптимизация: медиана из трёх) ====================
void quickSort(int* arr, int low, int high) {
    if (low >= high) return;

    // Медиана из трёх (защита от O(n²) и оптимизация ветвлений)
    int mid = low + (high - low) / 2;
    if (arr[mid] < arr[low]) swap(arr[mid], arr[low]);
    if (arr[high] < arr[low]) swap(arr[high], arr[low]);
    if (arr[high] < arr[mid]) swap(arr[high], arr[mid]);
    swap(arr[mid], arr[high]);

    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    int pi = i + 1;

    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
}

// ==================== MERGE SORT (оптимизация кэш-промахов: один буфер) ====================
void merge(int* arr, int* temp, int left, int mid, int right) {
    int i = left, j = mid + 1, k = left;

    while (i <= mid && j <= right) {
        temp[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    for (int i = left; i <= right; i++) arr[i] = temp[i];
}

void mergeSortRec(int* arr, int* temp, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortRec(arr, temp, left, mid);
    mergeSortRec(arr, temp, mid + 1, right);
    merge(arr, temp, left, mid, right);
}

void mergeSort(int* arr, int n) {
    int* temp = new int[n];   // вспомогательный буфер ОДИН раз
    mergeSortRec(arr, temp, 0, n - 1);
    delete[] temp;
}

// ==================== HEAP SORT ====================
void heapify(int* arr, int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int* arr, int n) {
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// ==================== ИЗМЕРЕНИЕ ====================
template <typename Func>
long long measure(Func sortFn, int src[], int n) {
    int* copy = new int[n];
    for (int i = 0; i < n; i++) copy[i] = src[i];

    auto start = high_resolution_clock::now();
    sortFn(copy, n);
    auto end = high_resolution_clock::now();

    delete[] copy;
    return duration_cast<microseconds>(end - start).count();
}

int main() {
    const int N = 50000;
    int* arr = new int[N];
    for (int i = 0; i < N; i++) arr[i] = rand() % N;

    cout << "=== Profiling sorts (N = " << N << ") ===\n";

    auto qs = [](int* a, int n) { quickSort(a, 0, n - 1); };
    cout << "Quick Sort: " << measure(qs, arr, N) << " mks\n";
    cout << "Merge Sort: " << measure(mergeSort, arr, N) << " mks\n";
    cout << "Heap Sort:  " << measure(heapSort, arr, N) << " mks\n";

    delete[] arr;
    return 0;
}
```

---

## Анализ сложности

| Алгоритм | Лучший | Средний | Худший | Доп. память | Устойчивость |
|---|---|---|---|---|---|
| Quick Sort | O(n log n) | O(n log n) | O(n²) | O(log n) | Нет |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) | O(n) | Да |
| Heap Sort | O(n log n) | O(n log n) | O(n log n) | O(1) | Нет |

---

## Вывод программы

```
=== Profiling sorts (N = 50000) ===
Quick Sort: 12103 mks
Merge Sort: 10166 mks
Heap Sort:  17370 mks
```

---

## Результаты и выводы
- Реализованы три эффективных алгоритма сортировки.
- Освоены принципы «разделяй и властвуй» (Quick Sort, Merge Sort) и структура «куча» (Heap Sort).
- Проведён анализ сложности и сравнительное профилирование.
- Merge Sort показал лучшее время на случайных данных, Heap Sort — худшее из-за накладных расходов на heapify.
- Quick Sort с медианой из трёх устойчив к худшим случаям.

## Преимущества и недостатки
- **Quick Sort:** быстрый на практике, малая память, но неустойчив и имеет худший случай O(n²).
- **Merge Sort:** гарантированное O(n log n), устойчив, но требует O(n) дополнительной памяти.
- **Heap Sort:** O(1) памяти, гарантированное O(n log n), но неустойчив и медленнее на практике.

---

**Ссылка на репозиторий:**  
[https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)

---

**Контрольные вопросы**
1.	Сложность Quick Sort?
Лучшая и средняя — O(n log n). Худшая — O(n²).
2.	Когда возникает худший случай Quick Sort?
Когда опорный элемент всегда оказывается минимальным или максимальным (например, при выборе последнего элемента на уже отсортированном массиве).
3.	Как улучшить Quick Sort?
Случайный выбор опорного элемента, медиана из трёх, или интроспективная сортировка (переход на Heap Sort при глубокой рекурсии).
4.	Сложность Merge Sort? Почему всегда O(n log n)?
Массив всегда делится ровно пополам (глубина рекурсии log n), и на каждом уровне происходит слияние, требующее O(n) операций.
5.	Преимущества и недостатки Merge Sort перед Quick?
Плюсы: Стабильная, гарантирует O(n log n) всегда. Минусы: Требует O(n) дополнительной памяти.
6.	Как работает Heap Sort?
Строится max-куча. Максимальный элемент (корень) меняется с последним, размер кучи уменьшается на 1, и свойство кучи восстанавливается (heapify). Сложность — O(n log n).
7.	Какая сортировка устойчивая?
Только Merge Sort. Quick и Heap нарушают порядок равных элементов из-за обменов на большие расстояния.
8.	Какая требует доп. памяти?
Merge Sort — O(n) для временного массива. Quick Sort — O(log n) для стека рекурсии.
9.	Какая лучше на почти отсортированных данных?
Merge Sort (строго O(n log n)). Quick Sort с медианой из трёх тоже справится хорошо, а вот классический Quick Sort (с последним элементом) деградирует до O(n²).
10.	Зачем в Heap Sort строить кучу?
Чтобы получить структуру, где максимум всегда находится в корне (индекс 0), что позволяет извлекать его за O(log n) и класть в конец массива.
11.	Что такое «разделяй и властвуй»?
Разбиение задачи на подзадачи меньшего размера (рекурсивное решение) с последующим объединением результатов. Quick (разбиение + рекурсия) и Merge (деление пополам + слияние).
12.	Можно ли Quick Sort итеративно?
Да, используя явный стек (массив) для хранения границ подмассивов, которые ещё нужно отсортировать.
13.	Почему Quick Sort быстрее Merge Sort на практике?
Из-за лучшей локальности кэша (работает в рамках одного массива, нет копирования в буфер) и меньшего количества операций с памятью, несмотря на одинаковую O-сложность.
14.	Когда выгодно использовать Heap Sort?
Когда критична память (O(1) доп. памяти) или в системах реального времени, где нельзя допустить худшего случая O(n²).
15.	Выбор для встроенных систем (ограниченная память)?
Heap Sort (не требует лишней памяти) или Quick Sort (при малом N и O(log n) на стек). Merge Sort использовать нельзя из-за выделения O(n) буфера.
