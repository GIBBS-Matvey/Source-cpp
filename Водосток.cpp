#include <iostream>
#include <queue>
#include <vector>


class Graph {
private:
    std::vector<std::vector<int>> graph;
    size_t dimM, dimN;
    std::vector<std::vector<bool>> used;
    std::vector<std::vector<bool>> hasDrain;
    std::vector<std::vector<bool>> completed;
    size_t drainNumber;

    bool isAvailable(const std::pair<int, int>& position) const {
        return position.first >= 0 && position.first < dimM && position.second >= 0 && position.second < dimN;
    }

    int getValue(std::pair<int, int> position) const {
        return graph[position.first][position.second];
    }

    bool areNeighbours(const std::pair<int, int>& tmpPosition, const std::pair<int, int>& targetPosition) const {
        return isAvailable(targetPosition) && (getValue(targetPosition) <= getValue(tmpPosition));
    }

    std::vector<std::pair<int, int>> getAdj(const std::pair<int, int>& position) const {

        std::vector<std::pair<int, int>> adj;
        adj.reserve(4);

        std::pair<int, int> possiblePosition(position.first, position.second - 1);
        if (areNeighbours(position, possiblePosition)) adj.emplace_back(possiblePosition);

        possiblePosition = {position.first - 1, position.second};
        if (areNeighbours(position, possiblePosition)) adj.emplace_back(possiblePosition);

        possiblePosition = {position.first, position.second + 1};
        if (areNeighbours(position, possiblePosition)) adj.emplace_back(possiblePosition);

        possiblePosition = {position.first + 1, position.second};
        if (areNeighbours(position, possiblePosition)) adj.emplace_back(possiblePosition);

        return adj;
    }

    void usedReset() {
        for (auto& line : used) {
            for (int i = 0; i < dimN; ++i) {
                line[i] = false;
            }
        }
    }


public:
    explicit Graph(std::vector<std::vector<int>>&& map) : graph(std::move(map)), dimM(graph.size()), dimN(graph[0].size()), drainNumber(0) {
        used.resize(dimM, std::vector<bool>());
        for (int i = 0; i < dimM; ++i) {
            used[i].resize(dimN, false);
        }
        hasDrain.resize(dimM, std::vector<bool>());
        for (int i = 0; i < dimM; ++i) {
            hasDrain[i].resize(dimN, false);
        }
        completed.resize(dimM, std::vector<bool>());
        for (int i = 0; i < dimM; ++i) {
            completed[i].resize(dimN, false);
        }
    }

    void Print() {
        for (const auto& str : graph) {
            for (auto val : str) {
                std::cout << val << ' ';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    bool WillDrainBFS(const std::pair<int, int>& position) {

        for (const auto& neighbour : getAdj(position)) {
            if (getValue(neighbour) < getValue(position) || completed[neighbour.first][neighbour.second] || hasDrain[neighbour.first][neighbour.second]) {
                completed[position.first][position.second] = true;
                return false;
            }
        }

        std::queue<std::pair<int, int>> queue;
        used[position.first][position.second] = true;

        for (const auto& neighbour : getAdj(position)) {
            if (getValue(neighbour) == getValue(position) && !used[neighbour.first][neighbour.second]) {
                queue.push(neighbour);
                used[neighbour.first][neighbour.second] = true;
            }
        }

        while (!queue.empty()) {
            std::pair<int, int> tmp_position = queue.front();
            queue.pop();

            for (const auto& neighbour : getAdj(tmp_position)) {
                if (getValue(neighbour) < getValue(tmp_position) || completed[neighbour.first][neighbour.second] || hasDrain[neighbour.first][neighbour.second]) {
                    completed[tmp_position.first][tmp_position.second] = true;
                    return false;
                }
            }

            for (const auto& neighbour : getAdj(tmp_position)) {
                if (getValue(neighbour) == getValue(tmp_position) && !used[neighbour.first][neighbour.second]) {
                    queue.push(neighbour);
                    used[neighbour.first][neighbour.second] = true;
                }
            }
        }

        hasDrain[position.first][position.second] = true;
        return true;
    }

    size_t CountDrainNumber() {
        drainNumber = 0;
        for (int i = 0; i < dimM; ++i) {
            for (int j = 0; j < dimN; ++j) {
                if (!completed[i][j]) {
                    drainNumber += WillDrainBFS({i, j});
                    usedReset();
                }
            }
        }
        return drainNumber;
    }

    ~Graph() = default;
};

int main() {
    size_t dimM, dimN;
    std::cin >> dimM >> dimN;

    std::vector<std::vector<int>> v;
    v.resize(dimM, std::vector<int>());
    for (auto& line : v) {
        line.resize(dimN, 0);
    }
    for (auto& line : v) {
        for (auto& val : line) {
            std::cin >> val;
        }
    }
    Graph graph(std::move(v));
    std::cout << graph.CountDrainNumber() << std::endl;

    return 0;
}
