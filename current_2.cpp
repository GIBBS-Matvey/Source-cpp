#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <unordered_set>


class Visitor {
public:
    void initVertex(int v) {}

    void startVertex(int v) {}
    void discoverVertex(int v) {}
    void finishVertex(int v) {}
};



class TopSortVisitor : public Visitor {
private:
    std::vector<int>* callVectorPtr;
public:
    explicit TopSortVisitor(std::vector<int>* callVectorPtr) : callVectorPtr(callVectorPtr) {}

    void finishVertex(int v) {
        callVectorPtr->emplace_back(v);
    }
};


class StrongComponentVisitor : public Visitor {
private:
    std::vector<std::vector<int>>* strongComponentsPtr;
    int curIndex;
    int* strongComponentNumber;

public:
    StrongComponentVisitor(std::vector<std::vector<int>>* strongComponentsPtr, int* sccNumber) : curIndex(-1), strongComponentNumber(sccNumber),
    strongComponentsPtr(strongComponentsPtr) {}

    void startVertex(int v) {
        ++(*strongComponentNumber);
        ++curIndex;
    }

    void discoverVertex(int v) {
        (*strongComponentsPtr)[curIndex].push_back(v + 1);
    }
};


class Graph {
private:
    struct Edge {
        int to;
    };

    std::vector<std::vector<Edge>> adj;
    size_t vertex_number;

    int get_vertex(int i, int j, int cols) {
        return i * cols + j;
    }

    std::pair<int, int> get_pos(int vertex, int cols) {
        return {vertex / cols, vertex % cols};
    }

    bool is_valid_pos(int i, int j, int rows, int cols) {
        return i >= 0 && i < rows && j >=0 && j < cols;
    }

    std::vector<Edge> get_neighbours(int i, int j, const std::vector<std::vector<int>>& matrix) {
        int rows = matrix.size();
        int cols = matrix[0].size();
        std::vector<Edge> result;
        result.reserve(4);

        if (is_valid_pos(i, j - 1, rows, cols) && matrix[i][j - 1] <= matrix[i][j]) {
            result.push_back({get_vertex(i, j - 1, cols)});
        }

        if (is_valid_pos(i - 1, j, rows, cols) && matrix[i - 1][j] <= matrix[i][j]) {
            result.push_back({get_vertex(i - 1, j, cols)});
        }

        if (is_valid_pos(i, j + 1, rows, cols) && matrix[i][j + 1] <= matrix[i][j]) {
            result.push_back({get_vertex(i, j + 1, cols)});
        }

        if (is_valid_pos(i + 1, j, rows, cols) && matrix[i + 1][j] <= matrix[i][j]) {
            result.push_back({get_vertex(i + 1, j, cols)});
        }

        return result;
    }

    int get_hash_edge(int from, int to, int n) {
        return from * n + to;
    }


public:
    explicit Graph(size_t vNumber) : vertex_number(vNumber), adj(vNumber) {}

    Graph(const std::vector<std::vector<int>>& matrix) {
        int rows = matrix.size();
        int cols = matrix[0].size();
        vertex_number = rows * cols;

        adj.reserve(vertex_number);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                adj.push_back(get_neighbours(i, j, matrix));
            }
        }
    }

    void add_edge(int from, int to) {
        adj[from].push_back({to});
    }

    template <class Visitor>
    void Dfs(Visitor vis, std::vector<std::string>& color, const std::vector<int>& callOrderV = std::vector<int>()) {

        std::vector<int> callOrderVector(vertex_number);

        if (!callOrderV.empty()) {
            for (auto v : callOrderV) {
                callOrderVector.emplace_back(v);
            }
        } else {
            for (int v = 0; v < vertex_number; ++v) {
                callOrderVector.emplace_back(v);
            }
        }

        for (auto v : callOrderVector) {
            color[v] = "white";
            vis.initVertex(v);
        }

        for (auto v : callOrderVector) {
            if (color[v] == "white") {
                vis.startVertex(v);
                DfsVisit(v, vis, color);
            }
        }
    }

    template<class Visitor>
    void DfsVisit(int current, Visitor vis, std::vector<std::string>& color) {
        color[current] = "gray";
        vis.discoverVertex(current);
        for (auto neiEdge : adj[current]) {
            if (color[neiEdge.to] == "white") {
                DfsVisit(neiEdge.to, vis, color);
            }
        }
        color[current] = "black";
        vis.finishVertex(current);
    }

    Graph getTransposed() const {
        Graph trGraph(vertex_number);
        for (int from = 0; from < vertex_number; ++from) {
            for (auto& neiEdge : adj[from]) {
                trGraph.add_edge(neiEdge.to, from);
            }
        }
        return trGraph;
    }

    void FindStrongComponents() {

        std::vector<std::string> color(vertex_number);
        std::vector<int> callVector(vertex_number);
        TopSortVisitor sortVisitor(&callVector);

        Dfs(sortVisitor, color);
        std::reverse(callVector.begin(), callVector.end());

        int scc_number = 0;
        std::vector<std::vector<int>> scc(vertex_number);
        StrongComponentVisitor sccVisitor(&scc, &scc_number);

        getTransposed().Dfs(sccVisitor, color, callVector);

        std::cout << "scc_number: " << scc_number << '\n';
        for (const auto& component : scc) {
            if (component.empty()) {
                break;
            }
            std::cout << "component.size(): " << component.size() << '\n';
            for (auto v : component) {
                std::cout << v << ' ';
            }
            std::cout << '\n';
        }
    }

    Graph get_condensed() {
        std::vector<std::string> color(vertex_number);
        std::vector<int> v_call(vertex_number);
        TopSortVisitor sort_visitor(&v_call);

        Dfs(sort_visitor, color);
        std::reverse(v_call.begin(), v_call.end());

        int scc_number = 0;
        std::vector<std::vector<int>> scc(vertex_number);
        StrongComponentVisitor scc_visitor(&scc, &scc_number);

        getTransposed().Dfs(scc_visitor, color, v_call);

        std::vector<int> v_scc_number(vertex_number);
        for (int i = 0; i < scc.size(); ++i) {
            for (int vertex : scc[i]) {
                v_scc_number[vertex] = i;
            }
        }

        for (auto& cur_num : v_scc_number) {
            std::cout << cur_num << ' ';
        }
        std::cout << '\n';

        Graph condensed_graph(scc_number);
        std::unordered_set<int> hash_table;
        for (int i = 0; i < vertex_number; ++i) {
            for (const Edge& edge : adj[i]) {
                if (v_scc_number[i] != v_scc_number[edge.to]) { /// in different components

                    int hashed_edge = get_hash_edge(v_scc_number[i], v_scc_number[edge.to], scc_number);
                    if (hash_table.find(hashed_edge) == hash_table.end()) {
                        hash_table.insert(hashed_edge);
                        condensed_graph.add_edge(v_scc_number[i], v_scc_number[edge.to]);
                    }
                }
            }
        }

        return condensed_graph;
    }

    void print() const {
        for (auto& neighbours : adj) {
            for (auto& nei : neighbours) {
                std::cout << nei.to << ' ';
            }
            std::cout << '\n';
        }
    }
};


int main() {

    int rows, cols;
    std::ifstream fin("C:\\Users\\PC\\CLionProjects\\untitled\\input.txt");
    fin >> rows >> cols;
    std::vector<std::vector<int>> matrix;

    matrix.resize(rows, std::vector<int>());
    for (auto& row : matrix) {
        row.resize(cols);
    }

    for (auto& row : matrix) {
        for (auto& cell : row) {
            fin >> cell;
        }
    }

    Graph graph(matrix);
    graph.FindStrongComponents();

    return 0;
}
