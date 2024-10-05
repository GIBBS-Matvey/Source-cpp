#include <iostream>
#include <vector>
#include <queue>

const int INTMAX = std::numeric_limits<int>::max();

class Graph {
private:
    struct Edge {
        int to;
    };

    std::vector<std::vector<Edge>> adj;
    std::vector<size_t> distances;
    std::vector<int> parent;
    size_t vertex_number;
    std::vector<bool> used;

    void Reset() {
        distances.resize(vertex_number, INTMAX);
        used.resize(vertex_number, false);
        parent.resize(vertex_number, -1);
    }

public:
    explicit Graph(size_t vertex_number) : adj(vertex_number, std::vector<Edge>()), parent(vertex_number, -1),
    used(vertex_number, false), distances(vertex_number, INTMAX), vertex_number(vertex_number) {}

    void AddEdge(const int& from, const int& to) {
        adj[from].push_back({to});
        adj[to].push_back({from});
    }

    std::vector<size_t> Bfs(const int& start) {
        Reset();
        std::queue<int> queue_bfs;
        queue_bfs.push(start);
        distances[start] = 0;
        used[start] = true;
        parent[start] = -1;

        while(!queue_bfs.empty()) {
            int vertex = queue_bfs.front();
            queue_bfs.pop();
            for (const auto& edge : adj[vertex]) {
                if (!used[edge.to]) {
                    used[edge.to] = true;
                    queue_bfs.push(edge.to);
                    distances[edge.to] = distances[vertex] + 1;
                    parent[edge.to] = vertex;
                }
            }
        }
        return distances;
    }

    void PrintPath(const int& vertex) {
        std::vector<int> result;
        result.reserve(distances[vertex] + 1);
        int current = vertex;
        result.push_back(current);
        while(parent[current] != -1) {
            current = parent[current];
            result.push_back(current);
        }
        for (auto it = result.rbegin(); it != result.rend(); ++it) {
            std::cout << *it << ' ';
        }
        std::cout << std::endl;
    }

};



int main() {
    int vertex_number;
    std::cin >> vertex_number;
    Graph graph(vertex_number);

    int edge_number;
    std::cin >> edge_number;
    for (int i = 0; i < edge_number; ++i) {
        int from, to;
        std::cin >> from >> to;
        graph.AddEdge(from, to);
    }

    int start;
    std::cin >> start;

    std::vector<size_t> result = graph.Bfs(start);

    for (int vertex = 0; vertex < vertex_number; ++vertex) {
        if (result[vertex] == INTMAX) {
            std::cout << -1 << std::endl;
            continue;
        }
        std::cout << result[vertex] << std::endl;
        graph.PrintPath(vertex);
    }
    return 0;
}
