#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;

// --- Задача 1: факториал ---
long long factorial(int n) {
    if (n <= 1) return 1;              // базовый случай
    return n * factorial(n - 1);       // шаг рекурсии
}

// --- Задача 2: числа Фибоначчи ---
long long fib(int n) {
    if (n <= 1) return n;              // базовый случай
    return fib(n - 1) + fib(n - 2);    // шаг рекурсии
}

// --- Задача 3: обход лабиринта 6x6 ---
const int N = 6;
char maze[N][N] = {
    {'S', '.', '#', '.', '.', '.'},
    {'.', '#', '.', '#', '.', '#'},
    {'.', '.', '.', '#', '.', '.'},
    {'#', '#', '.', '.', '.', '#'},
    {'.', '.', '#', '#', '.', '.'},
    {'.', '#', '.', '.', '.', 'F'}
};

bool findPath(int x, int y, bool visited[N][N]) {
    if (x < 0 || x >= N || y < 0 || y >= N) return false;   // за границей
    if (maze[x][y] == '#' || visited[x][y])  return false;   // стена или уже были
    if (maze[x][y] == 'F') return true;                     // финиш — базовый случай

    visited[x][y] = true;
    if (maze[x][y] != 'S') maze[x][y] = '*';                // помечаем путь

    // шаг рекурсии — 4 направления (backtracking)
    if (findPath(x + 1, y, visited) || findPath(x - 1, y, visited) ||
        findPath(x, y + 1, visited) || findPath(x, y - 1, visited)) return true;

    if (maze[x][y] != 'S') maze[x][y] = '.';                 // откат
    return false;
}

// --- Задача 4: задача о ферзях (5x5) ---
int  queenBoard[5];       // queenBoard[i] = столбец ферзя в строке i
bool queenCols[5];        // занят ли столбец
bool queenD1[10];         // занята ли главная диагональ
bool queenD2[10];         // занята ли побочная диагональ

void placeQueens(int row) {
    if (row == 5) {                                   // базовый случай — все 5 расставлены
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) cout << (queenBoard[i] == j ? "Q " : ". ");
            cout << "\n";
        }
        cout << "\n";
        return;
    }
    for (int col = 0; col < 5; col++) {               // шаг рекурсии — пробуем 5 столбцов
        if (queenCols[col] || queenD1[row - col + 4] || queenD2[row + col]) continue;
        queenBoard[row] = col;
        queenCols[col] = queenD1[row - col + 4] = queenD2[row + col] = true;
        placeQueens(row + 1);
        queenCols[col] = queenD1[row - col + 4] = queenD2[row + col] = false;  // откат
    }
}

// --- Пузырьковая (итеративная) ---
void bubbleSort(int a[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++)
            if (a[j] > a[j + 1]) { swap(a[j], a[j + 1]); swapped = true; }
        if (!swapped) break;                     // оптимизация
    }
}

// --- Пузырьковая (рекурсивная) ---
void bubbleRec(int a[], int n) {
    if (n <= 1) return;                          // базовый случай
    for (int i = 0; i < n - 1; i++)
        if (a[i] > a[i + 1]) swap(a[i], a[i + 1]);
    bubbleRec(a, n - 1);                         // шаг рекурсии — остаток массива
}

// --- Вставками ---
void insertionSort(int a[], int n) {
    for (int i = 1; i < n; i++) {
        int key = a[i], j = i - 1;
        while (j >= 0 && a[j] > key) { a[j + 1] = a[j]; j--; }
        a[j + 1] = key;
    }
}

// --- Выбором ---
void selectionSort(int a[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int m = i;
        for (int j = i + 1; j < n; j++) if (a[j] < a[m]) m = j;
        if (m != i) swap(a[i], a[m]);
    }
}

// Замер времени сортировки
template <typename Func>
long long timeSort(Func sortFn, int src[], int n) {
    int* copy = new int[n];
    for (int i = 0; i < n; i++) copy[i] = src[i];
    auto t0 = high_resolution_clock::now();
    sortFn(copy, n);
    auto t1 = high_resolution_clock::now();
    delete[] copy;
    return duration_cast<microseconds>(t1 - t0).count();
}

int main() {
    setlocale(LC_ALL, "ru");

    // --- 1. Факториал ---
    cout << "1. factorial(10) = " << factorial(10) << "\n";

    // --- 2. Фибоначчи ---
    cout << "2. fib(15) = " << fib(15) << "\n";

    // --- 3. Лабиринт ---
    bool visited[N][N] = {};   // создаётся перед вызовом
    findPath(0, 0, visited);
    cout << "3. Maze path found: " << (findPath(0, 0, visited) ? "yes" : "no") << "\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) cout << maze[i][j] << " ";
        cout << "\n";
    }

    // --- 4. Ферзи (5x5) ---
    cout << "\n4. One solution for 5 queens:\n";
    placeQueens(0);

    // --- 5. Сортировки ---
    const int SZ = 1000;
    int* arr = new int[SZ];
    for (int i = 0; i < SZ; i++) arr[i] = rand() % SZ;

    cout << "5. Sorting times (N = " << SZ << "):\n";
    cout << "   bubble:    " << timeSort(bubbleSort, arr, SZ) << " mks\n";
    cout << "   bubbleRec: " << timeSort(bubbleRec, arr, SZ) << " mks\n";
    cout << "   insertion: " << timeSort(insertionSort, arr, SZ) << " mks\n";
    cout << "   selection: " << timeSort(selectionSort, arr, SZ) << " mks\n";

    delete[] arr;
    return 0;
}
