#include <iostream>
#include <vector>
#include <forward_list>
#include <unordered_set>
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

    int get_hashed_edge(int from, int to, int vertex_num) {
        return from * vertex_num + to;
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

    /*std::vector<std::vector<int>> Kosaraju() {
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
    }*/

    std::pair<int, std::vector<int>> Kosaraju() {
        std::forward_list<int> sortedVertexes = this->TopSort();
        Graph newGraph = this->GetTransposed();
        size_t num = 1;

        std::vector<int> answer(vertex_number);
        for (const auto& vertex : sortedVertexes) {
            if (newGraph.colors[vertex] == "White") {
                newGraph.SCC_found(vertex, answer , num);
                ++num;
            }
        }

        return {num - 1, answer};
    }

    Graph get_condensed() {
        std::pair<int, std::vector<int>> kosaraju_pair = Kosaraju();

        int scc_num = kosaraju_pair.first;
        const std::vector<int>& scc_number = kosaraju_pair.second;

        Graph condensed_graph(scc_num);
        std::unordered_set<int> hash_table;
        for (int i = 0; i < vertex_number; ++i) {
            for (Edge edge : adj[i]) {
                if (scc_number[i] != scc_number[edge.to]) {
                    int hashed_edge = get_hashed_edge(scc_number[i] - 1, scc_number[edge.to] - 1, scc_num);
                    if (hash_table.find(hashed_edge) == hash_table.end()) {
                        hash_table.insert(hashed_edge);
                        condensed_graph.AddEdge(scc_number[i] - 1, scc_number[edge.to] - 1);
                    }
                }
            }
        }

        return condensed_graph;
    }

    void SCC_found(const int vertex, std::vector<int>& answer, int scc_index) {
        colors[vertex] = "Gray";
        for (const auto& edge : adj[vertex]) {
            if (colors[edge.to] == "White") {
                SCC_found(edge.to, answer, scc_index);
            }
        }
        colors[vertex] = "Black";
        answer[vertex] = scc_index;
    }

    void PrintAdj() {
        std::cout << "vertex_number: " << vertex_number << '\n';
        for (int i = 0; i < vertex_number; ++i) {
            std::cout << "vertex " << i << ": ";
            for (Edge edge : adj[i]) {
                std::cout << edge.to << ' ';
            }
            std::cout << '\n';
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
        graph.AddEdge(from - 1, to - 1);
    }

    graph.get_condensed().PrintAdj();

    return 0;
}
/*
7 9
1 2
3 2
1 4
4 5
2 5
5 6
6 3
4 7
5 7*/
