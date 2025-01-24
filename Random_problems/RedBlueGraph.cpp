#include <iostream>
#include <vector>


class PathVisitor {
    char color;
    std::vector<std::vector<bool>> isColorPath;
    size_t vertexNumber;
public:
    PathVisitor(size_t vertexNumber, char color) : color(color),
    isColorPath(vertexNumber, std::vector<bool>(vertexNumber, false)) {}

    void initColorPath(int from, int to) {
        isColorPath[from][to] = true;
    }

    bool hasColorPath(int from, int to) {
        return isColorPath[from][to];
    }

    void PrintPaths() const {
        for (int i = 0; i < vertexNumber; ++i) {
            for (int j = i + 1; j < vertexNumber; ++j) {
                std::cout << i << ',' << j << " : " <<  isColorPath[i][j] << std::endl;
            }
        }
    }

    char checkColor() {
        return color;
    }
};


class RedPathVisitor : public PathVisitor {
public:
    RedPathVisitor(size_t vertexNumber) : PathVisitor(vertexNumber, 'R') {}
};


class BluePathVisitor : public PathVisitor {
public:
    BluePathVisitor(size_t vertexNumber) : PathVisitor(vertexNumber, 'B') {}
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
    void HasPath(ColorVisitor& colorVisitor) {
        for (int i = vertexNumber - 1; i >= 1; --i) {       // run from up vertexes to save info about color paths
            for (int j = i - 1; j >= 0; --j) {
                if (HasPathVisit(j, i, colorVisitor)) {
                    colorVisitor.initColorPath(j, i);
                }
            }
        }
    }

    template<typename ColorVisitor>
    bool HasPathVisit(size_t tmp, size_t target, ColorVisitor& vis) {

        if (tmp == target - 1 && edges[tmp][target] == vis.checkColor()/*vis.checkColor(tmp, target))*/ || vis.hasColorPath(tmp, target)) {
            return true;
        }

        for (int neiVertex = tmp + 1; neiVertex <= target; ++neiVertex) {
            if (edges[tmp][neiVertex] == vis.checkColor() /*vis.checkColor(tmp, neiVertex)*/) {
                vis.initColorPath(tmp, neiVertex);
                if (HasPathVisit(neiVertex, target, vis)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool checkColorPaths() {
        BluePathVisitor bluePathVisitor(vertexNumber);
        RedPathVisitor redPathVisitor(vertexNumber);
        HasPath(redPathVisitor);
        HasPath(bluePathVisitor);

        /*std::cout << "red paths:" << std::endl;
        redPathVisitor.PrintPaths();

        std::cout << "blue paths:" << std::endl;
        bluePathVisitor.PrintPaths();*/

        for (int i = 0; i < vertexNumber; ++i) {
            for (int j = i + 1; j < vertexNumber; ++j) {
                if (redPathVisitor.hasColorPath(i, j) && bluePathVisitor.hasColorPath(i, j)) {
                    return true;
                }
            }
        }
        return false;
    }

    /*void Print() const {
        for (int i = 0; i < vertexNumber - 1; ++i) {
            for (int j = i + 1; j < vertexNumber; ++j) {
                std::cout << edges[i][j] << ' ';
            }
            std::cout << std::endl;
        }
    }*/

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
    
    if (!graph.checkColorPaths()) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
    return 0;
}

/*3
RB
R*/
