#include <iostream>
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
    
    int get_hash_edge(int from, int to, int n) {
        return from * n + to;
    }


public:
    explicit Graph(size_t vNumber) : vertex_number(vNumber), adj(vNumber) {}

    Graph(const std::vector<std::vector<int>>& matrix) {

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

        int strongComponentNumber = 0;
        std::vector<std::vector<int>> strongComponent(vertex_number);
        StrongComponentVisitor sccVisitor(&strongComponent, &strongComponentNumber);

        getTransposed().Dfs(sccVisitor, color, callVector);



        std::cout << strongComponentNumber << std::endl;
        for (const auto& scc : strongComponent) {
            if (scc.empty()) {
                break;
            }
            std::cout << scc.size() << '\n';
            for (auto v : scc) {
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
        graph.add_edge(from - 1, to - 1);
    }
    
    graph.FindStrongComponents();

    return 0;
}
