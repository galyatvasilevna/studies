```markdown
# Лабораторная работа №13  
## Алгоритмы Прима и Краскала. Минимальное остовное дерево

**Дисциплина:** Алгоритмы и структуры данных  
**Вариант:** 11  
**Выполнила:** Тарасенко Галина Васильевна, группа ПИЖ-б-о-25-1(2), направление 09.03.04 «Программная инженерия»  
**Проверила:** Ассистент кафедры Белик Ангелина Игоревна  

---

## Цель работы
1. Изучить алгоритмы нахождения минимального остовного дерева (MST): Прима и Краскала.
2. Реализовать структуру данных «Система непересекающихся множеств» (DSU) для алгоритма Краскала.
3. Освоить поиск второго по весу минимального остовного дерева (дополнительное задание).
4. Применить алгоритмы для решения практической задачи (прокладка сетей).

## Задание (Вариант 11)
- Реализовать алгоритм Прима (без оптимизации, O(V²)).
- Реализовать алгоритм Краскала с использованием DSU.
- **Дополнительное задание (Вариант 11):** Найти второе минимальное остовное дерево.
- **Применение:** Задача о прокладке сетей (минимальная стоимость соединения всех городов).

## Ссылка на репозиторий GitHub
[https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)

---

## Полный код решения

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

const int INF = 1e9;

// --- Структура для ребра (нужна для Краскала) ---
struct Edge {
    int u, v, w;
    bool operator<(const Edge& other) const { return w < other.w; }
};

// --- Система непересекающихся множеств (DSU) ---
class DSU {
    vector<int> parent, rank;
public:
    DSU(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]); // Сжатие путей
        return parent[x];
    }
    bool unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return false;
        if (rank[rx] < rank[ry]) swap(rx, ry); // Объединение по рангу
        parent[ry] = rx;
        if (rank[rx] == rank[ry]) rank[rx]++;
        return true;
    }
};

// --- Класс графа ---
class Graph {
    int V;
    vector<vector<int>> adj; // Матрица смежности (для Прима)
    vector<Edge> edges;      // Список рёбер (для Краскала)

public:
    Graph(int v) : V(v), adj(v, vector<int>(v, 0)) {}

    void addEdge(int u, int v, int w) {
        adj[u][v] = w;
        adj[v][u] = w; // Неориентированный граф
        edges.push_back({u, v, w});
    }

    // --- 1. АЛГОРИТМ ПРИМА (Вариант 11: O(V^2), без pq) ---
    int primMST() {
        vector<int> minEdge(V, INF);
        vector<bool> inMST(V, false);
        minEdge[0] = 0;
        int totalWeight = 0;

        for (int i = 0; i < V; i++) {
            int u = -1;
            // Ищем вершину с минимальным ребром
            for (int j = 0; j < V; j++) {
                if (!inMST[j] && (u == -1 || minEdge[j] < minEdge[u])) {
                    u = j;
                }
            }
            if (u == -1 || minEdge[u] == INF) return -1; // Граф не связный

            inMST[u] = true;
            totalWeight += minEdge[u];

            // Обновляем расстояния до соседей
            for (int v = 0; v < V; v++) {
                if (adj[u][v] != 0 && !inMST[v] && adj[u][v] < minEdge[v]) {
                    minEdge[v] = adj[u][v];
                }
            }
        }
        return totalWeight;
    }

    // --- 2. АЛГОРИТМ КРАСКАЛА ---
    // Возвращает вес MST и (опционально) сохраняет ребра в mstEdges
    int kruskalMST(vector<Edge>& mstEdges, bool saveEdges = false) {
        vector<Edge> sortedEdges = edges;
        sort(sortedEdges.begin(), sortedEdges.end()); // Простая сортировка

        DSU dsu(V);
        int totalWeight = 0;
        int edgesUsed = 0;
        if (saveEdges) mstEdges.clear();

        for (const auto& e : sortedEdges) {
            if (dsu.unite(e.u, e.v)) {
                totalWeight += e.w;
                edgesUsed++;
                if (saveEdges) mstEdges.push_back(e);
                if (edgesUsed == V - 1) break;
            }
        }
        return (edgesUsed == V - 1) ? totalWeight : -1; // -1, если не связный
    }

    // --- ВАРИАНТ 11: ПОИСК ВТОРОГО MST ---
    int secondMST() {
        vector<Edge> firstMSTEdges;
        int firstMSTWeight = kruskalMST(firstMSTEdges, true);
        if (firstMSTWeight == -1) return -1; // Граф не связный

        int secondMSTWeight = INF;

        // Перебираем каждое ребро из первого MST и "выкидываем" его
        for (const auto& removedEdge : firstMSTEdges) {
            vector<Edge> tempEdges;
            for (const auto& e : edges) {
                // Копируем все ребра, кроме удаленного
                if (!(e.u == removedEdge.u && e.v == removedEdge.v) &&
                    !(e.u == removedEdge.v && e.v == removedEdge.u)) {
                    tempEdges.push_back(e);
                }
            }
            
            // Запускаем Краскала без этого ребра
            vector<Edge> dummy;
            Graph tempGraph(V);
            tempGraph.edges = tempEdges;
            int currentWeight = tempGraph.kruskalMST(dummy);

            // Если MST построилось, оно больше первого, и меньше текущего второго — обновляем
            if (currentWeight != -1 && currentWeight > firstMSTWeight) {
                secondMSTWeight = min(secondMSTWeight, currentWeight);
            }
        }
        return (secondMSTWeight == INF) ? -1 : secondMSTWeight;
    }

    // --- 3. ПРИМЕНЕНИЕ (Задача о прокладке сетей) ---
    void buildNetwork() {
        vector<Edge> mstEdges;
        int cost = kruskalMST(mstEdges, true);
        
        if (cost == -1) {
            cout << "Граф не связный. Невозможно соединить все города.\n";
            return;
        }
        cout << "Минимальная стоимость: " << cost << "\n";
        cout << "Дороги для строительства:\n";
        for (const auto& e : mstEdges) {
            cout << " " << e.u << " - " << e.v << " (" << e.w << ")\n";
        }
    }
};

int main() {
    Graph g(6);

    // Заполняем граф для теста (из примера в методичке)
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 3);
    g.addEdge(1, 2, 2);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 4, 3);
    g.addEdge(3, 4, 5);
    g.addEdge(4, 5, 5);

    cout << "--- 1. Algorithm Prim (O(V^2)) ---\n";
    cout << "Prim MST weight: " << g.primMST() << "\n\n";

    cout << "--- 2. Algorithm Kruskal & 2nd MST (Variant 11) ---\n";
    vector<Edge> mstEdges;
    int kruskalWeight = g.kruskalMST(mstEdges, true);
    cout << "1st MST weight: " << kruskalWeight << "\n";
    cout << "2nd MST weight: " << g.secondMST() << "\n\n";

    cout << "--- 3. Application (Network Planning) ---\n";
    g.buildNetwork();

    return 0;
}
```

---

## Результаты работы

```
--- 1. Algorithm Prim (O(V^2)) ---
Prim MST weight: 15

--- 2. Algorithm Kruskal & 2nd MST (Variant 11) ---
1st MST weight: 15
2nd MST weight: 16

--- 3. Application (Network Planning) ---
Минимальная стоимость: 15
Дороги для строительства:
 1 - 2 (2)
 1 - 3 (2)
 0 - 2 (3)
 2 - 4 (3)
 4 - 5 (5)
```

---

## Анализ сложности

| Алгоритм | Временная сложность | Пространственная сложность | Структура данных |
| :--- | :--- | :--- | :--- |
| **Прим (O(V²))** | O(V²) | O(V²) | Матрица смежности |
| **Краскал** | O(E log E) | O(V + E) | Список рёбер + DSU |
| **Второй MST** | O(V * E log E) | O(V + E) | Многократный запуск Краскала |

- **Прим** лучше использовать для плотных графов (E ≈ V²).
- **Краскал** предпочтительнее для разреженных графов (E << V²).
- Второй MST ищется перебором рёбер первого MST (удаляем по одному и запускаем Краскала заново).

---

## Ответы на контрольные вопросы

**1. Что такое минимальное остовное дерево (MST)?**  
Это подграф, который соединяет все вершины графа без циклов и имеет минимально возможную сумму весов рёбер.

**2. В чём отличие алгоритма Прима от алгоритма Краскала?**  
Прим «выращивает» одно дерево, добавляя к нему ближайшую вершину. Краскал строит лес и объединяет компоненты, перебирая рёбра в порядке возрастания веса.

**3. Зачем нужна система непересекающихся множеств (DSU) в Краскале?**  
Она позволяет быстро проверять, принадлежат ли вершины `u` и `v` к разным компонентам связности (чтобы не образовать цикл) и объединять компоненты.

**4. Какова сложность алгоритма Прима без оптимизации?**  
O(V²), так как на каждом шаге мы линейно ищем минимум в массиве и обновляем расстояния до всех соседей.

**5. Какова сложность алгоритма Краскала?**  
O(E log E) из-за сортировки рёбер. Операции DSU занимают почти O(1) благодаря сжатию путей и объединению по рангу.

**6. Как найти второе минимальное остовное дерево?**  
Нужно найти первое MST, затем по очереди удалять каждое его ребро и запускать алгоритм поиска MST заново. Минимальный вес среди полученных деревьев, который больше веса первого MST, и будет ответом.

**7. Какой алгоритм выбрать для плотного графа?**  
Алгоритм Прима (особенно с матрицей смежности), так как он не требует сортировки всех рёбер и эффективно работает при большом количестве связей.

**8. Какой алгоритм выбрать для разреженного графа?**  
Алгоритм Краскала, так как его сложность зависит от количества рёбер E, а не от V².

**9. Что произойдёт, если граф несвязный?**  
Алгоритм не сможет построить остовное дерево, включающее все вершины. В коде предусмотрена проверка: возвращается -1, если количество использованных рёбер меньше V-1.

**10. Работают ли алгоритмы Прима и Краскала для ориентированных графов?**  
Нет, эти алгоритмы предназначены только для неориентированных графов.

**11. Почему в алгоритме Краскала нужно сортировать рёбра?**  
Чтобы жадно выбирать самые лёгкие рёбра, которые не образуют цикл, что в итоге приводит к минимальному остовному дереву.

**12. Как проверить, что рёбра образуют цикл в Краскале?**  
С помощью DSU: если `find(u) == find(v)`, то вершины уже в одной компоненте, добавление ребра создаст цикл.

**13. Можно ли использовать алгоритм Прима с отрицательными весами?**  
Да, алгоритмы поиска MST работают с любыми весами, так как они ищут минимум суммы, а не кратчайшие пути.

---

**Ссылка на репозиторий:**  
[https://github.com/galyatvasilevna/studies](https://github.com/galyatvasilevna/studies)
```