#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
using namespace std;
using namespace chrono;

const int INF = 1e9; // Бесконечность

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int v) : V(v), adj(v, vector<int>(v, INF)) {
        for (int i = 0; i < V; i++) adj[i][i] = 0;
    }

    // Неориентированный граф
    void addEdge(int u, int v, int w) {
        adj[u][v] = w;
        adj[v][u] = w;
    }

    // --- 1. Дейкстра O(V^2) с восстановлением пути ---
    // Возвращает массив расстояний и массив предков (prev)
    pair<vector<int>, vector<int>> dijkstra(int start) {
        vector<int> dist(V, INF), prev(V, -1);
        vector<bool> visited(V, false);
        dist[start] = 0;

        for (int i = 0; i < V; i++) {
            int u = -1;
            for (int j = 0; j < V; j++) {
                if (!visited[j] && (u == -1 || dist[j] < dist[u])) u = j;
            }
            if (dist[u] == INF) break;
            visited[u] = true;

            for (int v = 0; v < V; v++) {
                if (adj[u][v] != INF && dist[u] + adj[u][v] < dist[v]) {
                    dist[v] = dist[u] + adj[u][v];
                    prev[v] = u;
                }
            }
        }
        return { dist, prev };
    }

    // Вспомогательная функция для вывода пути
    void printPath(int start, int end, const vector<int>& prev) {
        if (end == -1) return;
        vector<int> path;
        for (int v = end; v != -1; v = prev[v]) path.push_back(v);
        reverse(path.begin(), path.end());
        cout << "path: ";
        for (int v : path) cout << v << " ";
        cout << "\n";
    }

    // ВАРИАНТ 11: Нахождение всех путей от одной вершины
    void printAllPaths(int start) {
        auto [dist, prev] = dijkstra(start);
        cout << "Dijkstra from " << start << ":\n";
        for (int i = 0; i < V; i++) {
            if (i == start) continue;
            cout << " to " << i << " (dist " << dist[i] << ") ";
            if (dist[i] == INF) cout << "unreachable\n";
            else printPath(start, i, prev);
        }
    }

    // --- 2. Флойд-Уоршелл ---
    vector<vector<int>> floydWarshall() {
        vector<vector<int>> dist = adj;
        for (int k = 0; k < V; k++) {
            for (int i = 0; i < V; i++) {
                for (int j = 0; j < V; j++) {
                    if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
        return dist;
    }
};

// --- 3. Сравнительный анализ ---
void runBenchmark(int N) {
    Graph g(N);
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            if (rand() % 100 < 30) { // Вероятность ребра 30%
                g.addEdge(i, j, rand() % 50 + 1);
            }
        }
    }

    // Замер Флойда
    auto t1 = high_resolution_clock::now();
    g.floydWarshall();
    auto t2 = high_resolution_clock::now();
    auto timeFloyd = duration_cast<microseconds>(t2 - t1).count();

    // Замер Дейкстры (запускаем для всех вершин, чтобы найти все пути)
    t1 = high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        g.dijkstra(i); // Вызов O(V^2) N раз дает O(V^3), как и Флойд
    }
    t2 = high_resolution_clock::now();
    auto timeDijkstra = duration_cast<microseconds>(t2 - t1).count();

    cout << N << "\t\t" << timeDijkstra << "\t\t" << timeFloyd << "\n";
}

int main() {
    // 1. Демонстрация на маленьком графе (V=6)
    Graph g(6);
    g.addEdge(0, 1, 4); g.addEdge(0, 2, 2);
    g.addEdge(1, 2, 1); g.addEdge(1, 3, 5);
    g.addEdge(2, 3, 8); g.addEdge(2, 4, 10);
    g.addEdge(3, 4, 2); g.addEdge(3, 5, 6);
    g.addEdge(4, 5, 3);

    cout << "--- Paths from 0 (Dijkstra) ---\n";
    g.printAllPaths(0);

    cout << "\n--- Floyd-Warshall Matrix ---\n";
    auto dist = g.floydWarshall();
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (dist[i][j] == INF) cout << "INF\t";
            else cout << dist[i][j] << "\t";
        }
        cout << "\n";
    }

    // 2. Сравнение (Вариант 11 требует V=50, и часть 3 требует 10, 20, 50, 100)
    cout << "\n--- Performance Benchmark (Dijkstra vs Floyd) ---\n";
    cout << "V\t\tDijkstra(All)\tFloyd-Warshall\n";
    cout << "--------------------------------------------------\n";

    vector<int> sizes = { 10, 20, 50, 100 }; // V=50 - требование варианта
    for (int N : sizes) {
        runBenchmark(N);
    }

    return 0;
}
