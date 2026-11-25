#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

// ============================================================
// ЧАСТЬ 1: АЛГОРИТМ КМП (Кнута-Морриса-Пратта)
// ============================================================

// Вычисление префикс-функции
vector<int> computePrefix(const string& p) {
    vector<int> pi(p.length(), 0);
    for (int i = 1; i < p.length(); i++) {
        int j = pi[i - 1];
        while (j > 0 && p[i] != p[j]) j = pi[j - 1];
        if (p[i] == p[j]) j++;
        pi[i] = j;
    }
    return pi;
}

// Поиск всех вхождений
vector<int> kmpSearch(const string& t, const string& p) {
    vector<int> res;
    if (p.empty()) return res;
    vector<int> pi = computePrefix(p);
    int j = 0;
    
    for (int i = 0; i < t.length(); i++) {
        while (j > 0 && t[i] != p[j]) j = pi[j - 1];
        if (t[i] == p[j]) j++;
        if (j == p.length()) {
            res.push_back(i - p.length() + 1);
            j = pi[j - 1]; // Откат для поиска следующих вхождений
        }
    }
    return res;
}

// ВАРИАНТ 11: Замена всех вхождений
string replaceAll(string t, const string& p, const string& rep) {
    vector<int> pos = kmpSearch(t, p);
    if (pos.empty()) return t;
    
    string result = "";
    int last = 0;
    
    for (int idx : pos) {
        result += t.substr(last, idx - last) + rep; // Копируем текст до вхождения + замена
        last = idx + p.length(); // Сдвигаем указатель за пределы найденного слова
    }
    result += t.substr(last); // Добавляем остаток строки
    return result;
}

// ============================================================
// ЧАСТЬ 2: STL (Подсчёт частоты символов через std::map)
// ============================================================
void printCharFrequency(const string& text) {
    map<char, int> freq;
    for (char c : text) freq[c]++; // Автоматическая сортировка по ключу и подсчёт
    
    cout << "Char frequency:\n";
    for (const auto& pair : freq) {
        cout << " '" << pair.first << "': " << pair.second << "\n";
    }
}

int main() {
    string text = "ababaababcabab";
    string pattern = "abab";
    string replacement = "X";

    cout << "--- 1. KMP Algorithm ---\n";
    cout << "Original text: " << text << "\n";
    
    vector<int> positions = kmpSearch(text, pattern);
    cout << "Positions: ";
    for (int p : positions) cout << p << " ";
    cout << "\n";

    cout << "\n--- 2. Replace All (Variant 11) ---\n";
    string newText = replaceAll(text, pattern, replacement);
    cout << "Replaced text: " << newText << "\n\n";

    cout << "--- 3. STL (Map) ---\n";
    printCharFrequency(newText);

    return 0;
}
