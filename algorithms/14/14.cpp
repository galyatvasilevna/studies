#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ============================================================
// ЧАСТЬ 1: ЗАДАЧА О РЮКЗАКЕ (Вес + Объем)
// ============================================================
void knapsackMultiConstraint(int maxW, int maxV, 
                             const vector<int>& weights, 
                             const vector<int>& volumes, 
                             const vector<int>& values) {
    int n = weights.size();
    // 3D таблица: [предметы][вес][объем]
    vector<vector<vector<int>>> dp(n + 1, vector<vector<int>>(maxW + 1, vector<int>(maxV + 1, 0)));

    // Заполнение таблицы
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= maxW; w++) {
            for (int v = 0; v <= maxV; v++) {
                if (weights[i - 1] <= w && volumes[i - 1] <= v) {
                    // Берем максимум из: (не берем предмет) и (берем предмет)
                    dp[i][w][v] = max(dp[i - 1][w][v], 
                                      dp[i - 1][w - weights[i - 1]][v - volumes[i - 1]] + values[i - 1]);
                } else {
                    dp[i][w][v] = dp[i - 1][w][v];
                }
            }
        }
    }

    // Восстановление выбранных предметов
    cout << "Max value: " << dp[n][maxW][maxV] << "\n";
    cout << "Selected items: ";
    int w = maxW, v = maxV;
    for (int i = n; i > 0; i--) {
        // Если значение отличается от верхней строки, значит предмет взят
        if (dp[i][w][v] != dp[i - 1][w][v]) {
            cout << i - 1 << " "; // Выводим индекс предмета
            w -= weights[i - 1];
            v -= volumes[i - 1];
        }
    }
    cout << "\n";
}

// ============================================================
// ЧАСТЬ 2: LCS ДЛЯ МАССИВОВ ЧИСЕЛ (Вариант 11)
// ============================================================
vector<int> lcsArrays(const vector<int>& A, const vector<int>& B) {
    int m = A.size();
    int n = B.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    // Заполнение таблицы
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (A[i - 1] == B[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    // Восстановление LCS
    vector<int> lcs;
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (A[i - 1] == B[j - 1]) {
            lcs.push_back(A[i - 1]);
            i--; j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }
    reverse(lcs.begin(), lcs.end()); // Разворачиваем, так как шли с конца
    return lcs;
}

int main() {
    // --- Тест 1: Рюкзак с 2 ограничениями ---
    cout << "--- 1. Knapsack (Weight + Volume) ---\n";
    vector<int> weights = {2, 3, 4, 5};
    vector<int> volumes = {1, 2, 3, 1};
    vector<int> values  = {3, 4, 5, 8};
    int maxW = 8, maxV = 4;
    knapsackMultiConstraint(maxW, maxV, weights, volumes, values);
    
    // --- Тест 2: LCS для массивов ---
    cout << "\n--- 2. LCS for Arrays ---\n";
    vector<int> arr1 = {1, 2, 3, 4, 5, 6};
    vector<int> arr2 = {1, 3, 5, 7, 6};
    vector<int> res = lcsArrays(arr1, arr2);
    
    cout << "LCS length: " << res.size() << "\n";
    cout << "LCS elements: ";
    for (int x : res) cout << x << " ";
    cout << "\n";

    return 0;
}
