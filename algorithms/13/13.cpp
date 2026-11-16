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
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    bool unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return false;
        if (rank[rx] < rank[ry]) swap(rx, ry);
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
