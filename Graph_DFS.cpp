#include <iostream>
#include <vector>
#include <string>

class Graph {
private:
    struct Edge {
        int to;
    };


    std::vector<std::vector<Edge>> adj;
    std::vector<std::string> colors;
    std::vector<size_t> start;
    std::vector<size_t> finish;
    size_t vertex_number;
    size_t time = 0;

public:
    explicit Graph(const size_t vertex_number) : adj(vertex_number, std::vector<Edge>()),
    colors(vertex_number, "White"), start(vertex_number), finish(vertex_number), vertex_number(vertex_number) {}

    void AddEdge(int from, int to) {
        adj[from].push_back({to});
    }

    void Dfs() {
        for (int v = 0; v < vertex_number; ++v) {
            if (colors[v] == "White") {
                DfsVisit(v);
            }
        }
    }
    void DfsVisit(const int vertex) {
        colors[vertex] = "Gray";
        start[vertex] = ++time;
        for (const auto& edge : adj[vertex]) {
            if (colors[edge.to] == "White") {
                DfsVisit(edge.to);
            }
        }
        colors[vertex] = "Black";
        finish[vertex] = ++time;
    }

    void PrintVisitOrder() {
        for (const auto& val : start) {
            std::cout << val << ' ';
        }
        std::cout << std::endl;
        for (const auto& val : finish) {
            std::cout << val << ' ';
        }
        std::cout << std::endl;
    }
};


int main() {
    size_t vertex_number;
    std::cin >> vertex_number;

    size_t edge_number;
    std::cin >> edge_number;

    Graph graph(vertex_number);

    for (size_t i = 0; i < edge_number; ++i) {
        int from, to;
        std::cin >> from >> to;
        graph.AddEdge(from, to);
    }

    graph.Dfs();
    graph.PrintVisitOrder();

    return 0;
}
