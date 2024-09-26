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
    size_t vNumber;


public:
    explicit Graph(size_t vNumber) : vNumber(vNumber), adj(vNumber) {}

    Graph(const std::vector<std::vector<int>>& matrix) {

    }


    void addAdj(int from, int to) {
        adj[from].push_back({to});
    }

    template <class Visitor>
    void Dfs(Visitor vis, std::vector<std::string>& color, const std::vector<int>& callOrderV = std::vector<int>()) {

        std::vector<int> callOrderVector(vNumber);

        if (!callOrderV.empty()) {
            for (auto v : callOrderV) {
                callOrderVector.emplace_back(v);
            }
        } else {
            for (int v = 0; v < vNumber; ++v) {
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
        Graph trGraph(vNumber);
        for (int from = 0; from < vNumber; ++from) {
            for (auto& neiEdge : adj[from]) {
                trGraph.addAdj(neiEdge.to, from);
            }
        }
        return trGraph;
    }

    void FindStrongComponents() {

        std::vector<std::string> color(vNumber);
        std::vector<int> callVector(vNumber);
        TopSortVisitor sortVisitor(&callVector);

        Dfs(sortVisitor, color);
        std::reverse(callVector.begin(), callVector.end());

        int strongComponentNumber = 0;
        std::vector<std::vector<int>> strongComponent(vNumber);
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
        std::vector<std::string> color(vNumber);
        std::vector<int> callVector(vNumber);
        TopSortVisitor sortVisitor(&callVector);

        Dfs(sortVisitor, color);
        std::reverse(callVector.begin(), callVector.end());

        int strongComponentNumber = 0;
        std::vector<std::vector<int>> scc(vNumber);
        StrongComponentVisitor sccVisitor(&scc, &strongComponentNumber);

        getTransposed().Dfs(sccVisitor, color, callVector);

        std::vector<int> vSccNumber(vNumber);
        for (int i = 0; i < scc.size(); ++i) {
            for (int vertex : scc[i]) {
                vSccNumber[vertex] = i;
            }
        }

        Graph condensed_graph(strongComponentNumber);
        for (int i = 0; i < strongComponentNumber; ++i) {
            for (int j = 0; j < strongComponentNumber; ++j) {
                if (i != j) {
                    //todo
                }
            }
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
        graph.addAdj(from - 1, to - 1);
    }

    graph.FindStrongComponents();

    return 0;
}
