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
