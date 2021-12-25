#include <iostream>
#include <vector>


class PathVisitor {
    char color;
    std::vector<std::vector<bool>> isColorPath;
    const std::vector<std::vector<char>>& edges;
public:
    PathVisitor(const std::vector<std::vector<char>>& edges, char color) : color(color), edges(edges),
    isColorPath(edges.size(), std::vector<bool>(edges.size(), false)) {}

    void initColorPath(size_t from, size_t to) {
        isColorPath[from][to] = true;
    }

    bool hasColorPath(size_t from, size_t to) {
        return isColorPath[from][to];
    }

    bool checkColor(size_t from, size_t to) {
        return edges[from][to] == color;
    }
};


class RedPathVisitor : public PathVisitor {
public:
    RedPathVisitor(const std::vector<std::vector<char>>& edgesPtr) : PathVisitor(edgesPtr, 'R') {}
};


class BluePathVisitor : public PathVisitor {
public:
    BluePathVisitor(const std::vector<std::vector<char>>& edgesPtr) : PathVisitor(edgesPtr, 'B') {}
};


class Graph {
private:
    size_t vertexNumber;
    std::vector<std::vector<char>> edges;

public:
    Graph(size_t vNumber) : vertexNumber(vNumber), edges(vNumber, std::vector<char>(vNumber)) {}

    void addEdge(size_t from, size_t to, char color) {
        edges[from][to] = color;
    }

    template<typename ColorVisitor>
    std::pair<size_t, size_t> HasPath(ColorVisitor& colorVisitor) {
        for (int i = vertexNumber - 2; i >= 0; --i) {       // run from up vertexes to save info about color paths
            for (int j = i - 1; j >= 0; --j) {
                if (HasPathVisit(j, i, colorVisitor)) {
                    colorVisitor.initColorPath(j, i);
                    return {j, i};
                }
            }
        }
    }

    template<typename ColorVisitor>
    bool HasPathVisit(size_t tmp, size_t target, ColorVisitor& colorVisitor) {
        if (tmp == target || colorVisitor.hasColorPath(tmp, target))
            return true;
        for (size_t neiVertex = tmp + 1; neiVertex <= target; ++neiVertex) {
            if (edges[tmp][neiVertex] == colorVisitor.checkColor(tmp, neiVertex)) {
                HasPathVisit(neiVertex, target, colorVisitor);
            }
        }
        return false;
    }
    
    bool checkColorPaths() {
        RedPathVisitor redPathVisitor(edges);
        BluePathVisitor bluePathVisitor(edges);
        std::pair<size_t, size_t> psbPair = HasPath(redPathVisitor);
        if (HasPathVisit(psbPair.first, psbPair.second, bluePathVisitor)) {
            return true;
        }
        //todo
    }

    void Print() const {
        for (const auto& row : edges)
            for (auto color : row)
                std::cout << color << ' ';
            std::cout << std::endl;
    }

};


int main() {
    size_t vertexNumber;
    std::cin >> vertexNumber;
    Graph graph(vertexNumber);
    for (int i = 0; i < vertexNumber - 1; ++i) {
        for (int j = i + 1; j < vertexNumber; ++j) {
            char tmpColor;
            std::cin >> tmpColor;
            graph.addEdge(i, j, tmpColor);
        }
    }
    //graph.Print();
    return 0;
}
