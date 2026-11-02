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
