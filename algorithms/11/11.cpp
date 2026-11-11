#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
using namespace std;

class Graph {
    int V; // Количество вершин
    vector<vector<int>> adj; // Список смежности

    // Вспомогательная функция для рекурсивного DFS
    void dfs_rec_util(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << v << " ";
        for (int u : adj[v]) if (!visited[u]) dfs_rec_util(u, visited);
    }

public:
    Graph(int vertices) : V(vertices) { adj.resize(V); }

    // ВАРИАНТ 11: Ориентированный граф с петлями
    void addEdge(int u, int v) {
        adj[u].push_back(v); // Только в одну сторону. Если u==v, будет петля.
    }

    void removeEdge(int u, int v) {
        auto it = find(adj[u].begin(), adj[u].end(), v);
        if (it != adj[u].end()) adj[u].erase(it);
    }

    void print() {
        for (int i = 0; i < V; i++) {
            cout << i << ": ";
            for (int u : adj[i]) cout << u << " ";
            cout << "\n";
        }
    }

    // --- 1. DFS (Рекурсивный) ---
    void DFS_Recursive(int start) {
        vector<bool> visited(V, false);
        cout << "DFS (Rec): ";
        dfs_rec_util(start, visited);
        cout << "\n";
    }

    // --- 2. DFS (Итеративный) ---
    void DFS_Iterative(int start) {
        vector<bool> visited(V, false);
        stack<int> st;
        st.push(start);
        cout << "DFS (Iter): ";
        while (!st.empty()) {
            int v = st.top(); st.pop();
            if (!visited[v]) {
                visited[v] = true;
                cout << v << " ";
                // Добавляем в стек соседей (в обратном порядке для корректного вывода)
                for (auto it = adj[v].rbegin(); it != adj[v].rend(); ++it) {
                    if (!visited[*it]) st.push(*it);
                }
            }
        }
        cout << "\n";
    }

    // --- 3. BFS ---
    void BFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);
        cout << "BFS: ";
        while (!q.empty()) {
            int v = q.front(); q.pop();
            cout << v << " ";
            for (int u : adj[v]) {
                if (!visited[u]) { visited[u] = true; q.push(u); }
            }
        }
        cout << "\n";
    }

    // --- ВАРИАНТ 11: Вершины на заданном расстоянии ---
    void nodesAtDistance(int start, int targetDist) {
        vector<int> dist(V, -1);
        queue<int> q;
        dist[start] = 0;
        q.push(start);
        cout << "Nodes at distance " << targetDist << " from " << start << ": ";
        bool found = false;
        while (!q.empty()) {
            int v = q.front(); q.pop();
            if (dist[v] == targetDist) {
                cout << v << " ";
                found = true;
            }
            if (dist[v] < targetDist) { // Дальше не идем, чтобы не тратить время
                for (int u : adj[v]) {
                    if (dist[u] == -1) {
                        dist[u] = dist[v] + 1;
                        q.push(u);
                    }
                }
            }
        }
        if (!found) cout << "none";
        cout << "\n";
    }

    // --- Часть 3: Компоненты связности ---
    int countComponents() {
        vector<bool> visited(V, false);
        int count = 0;
        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                count++;
                dfs_rec_util(i, visited);
            }
        }
        return count;
    }

    // --- Часть 3: Кратчайший путь (BFS) ---
    void shortestPath(int start, int end) {
        vector<int> prev(V, -1), dist(V, -1);
        queue<int> q;
        dist[start] = 0; q.push(start);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            if (v == end) break;
            for (int u : adj[v]) {
                if (dist[u] == -1) { dist[u] = dist[v] + 1; prev[u] = v; q.push(u); }
            }
        }
        if (dist[end] == -1) { cout << "No path from " << start << " to " << end << "\n"; return; }
        
        vector<int> path;
        for (int v = end; v != -1; v = prev[v]) path.push_back(v);
        reverse(path.begin(), path.end());
        
        cout << "Shortest path " << start << "->" << end << ": ";
        for (int i = 0; i < path.size(); i++) {
            cout << path[i] << (i < path.size() - 1 ? " -> " : "");
        }
        cout << " (len " << dist[end] << ")\n";
    }
};

int main() {
    Graph g(6);

    // Создаем граф с петлями (ориентированный)
    g.addEdge(0, 1); g.addEdge(0, 2);
    g.addEdge(1, 3); g.addEdge(2, 3); g.addEdge(2, 4);
    g.addEdge(3, 5); g.addEdge(4, 5);
    g.addEdge(5, 5); // Петля!
    g.addEdge(1, 1); // Еще петля!

    cout << "--- Adjacency List ---\n"; g.print();
    cout << "\n--- Traversals ---\n";
    g.DFS_Recursive(0); g.DFS_Iterative(0); g.BFS(0);
    
    cout << "\n--- Variant 11 Task ---\n";
    g.nodesAtDistance(0, 2); // Найти вершины на расстоянии 2 от 0
    
    cout << "\n--- Part 3 Application ---\n";
    cout << "Connected components: " << g.countComponents() << "\n";
    g.shortestPath(0, 5);

    return 0;
}
