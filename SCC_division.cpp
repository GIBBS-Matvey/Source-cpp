#include <iostream>
#include <vector>
#include <forward_list>
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

public:
    explicit Graph(const size_t vertex_number) : adj(vertex_number),
    colors(vertex_number, "White"), start(vertex_number), finish(vertex_number), vertex_number(vertex_number) {}

    void Reset() {
        for (size_t i = 0; i < vertex_number; ++i) {
            colors[i] = "White";
            start[i] = 0;
            finish[i] = 0;
        }
        time = 0;
    }

    void Dfs() {
        for (size_t v = 0; v < vertex_number; ++v) {
            if (colors[v] == "White") {
                DfsVisit(v);
            }
        }
    }

    void AddEdge(int from, int to) {
        adj[from].push_back({to});
    }

    Graph GetTransposed() {
        Graph new_graph(this->vertex_number);
        for (size_t i = 0; i < this->adj.size(); ++i) {
            for (size_t j = 0; j < this->adj[i].size(); ++j) {
                new_graph.AddEdge(this->adj[i][j].to, i);
            }
        }
        return new_graph;
    }

    std::forward_list<int> TopSort() {
        Reset();
        std::forward_list<int> answer;
        for (size_t v = 0; v < vertex_number; ++v) {
            if (colors[v] == "White") {
                DfsVisitTopSort(v, answer);
            }
        }
        return answer;
    }

    void DfsVisitTopSort(const int vertex, std::forward_list<int>& answer) {
        colors[vertex] = "Gray";
        start[vertex] = ++time;
        for (const auto& edge : adj[vertex]) {
            if (colors[edge.to] == "White") {
                DfsVisitTopSort(edge.to, answer);
            }
        }
        colors[vertex] = "Black";
        finish[vertex] = ++time;
        answer.push_front(vertex);
    }

    void SCC_found(const int vertex, std::vector<int>& answer) {
        colors[vertex] = "Gray";
        for (const auto& edge : adj[vertex]) {
            if (colors[edge.to] == "White") {
                SCC_found(edge.to, answer);
            }
        }
        colors[vertex] = "Black";
        answer.push_back(vertex);
    }

    std::vector<std::vector<int>> Kosaraju() {
        std::forward_list<int> sortedVertexes = this->TopSort();
        Graph newGraph = this->GetTransposed();
        std::vector<std::vector<int>> scc;
        scc.resize(vertex_number, std::vector<int>());
        size_t i = 0;
        for (const auto& vertex : sortedVertexes) {
            if (newGraph.colors[vertex] == "White") {
                newGraph.SCC_found(vertex, scc[i]);
                ++i;
            }
        }
        scc.shrink_to_fit();
        return scc;
    }

    void PrintAdj() {
        for (const auto& arr : adj) {
            for (const auto& val : arr) {
                std::cout << val.to << ' ';
            }
            std::cout << std::endl;
        }
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

    std::vector<std::vector<int>> answer = graph.Kosaraju();

    for (const auto& scc : answer) {
        for (const auto &val : scc) {
            std::cout << val << ' ';
        }
        std::cout << std::endl;
    }

    return 0;
}

