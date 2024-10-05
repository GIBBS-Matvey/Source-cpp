#include <iostream>
#include <fstream>
#include <vector>
#include <forward_list>
#include <string>
#include <unordered_set>
#include <random>
#include <queue>


class Visitor {
protected:
    std::vector<int> v_call_order;
    std::vector<std::string> color;
    int vertex_number;
public:

    Visitor(int v_number) : vertex_number(v_number), color(v_number, "white") {
        v_call_order.reserve(v_number);
        for (int i = 0; i < v_number; ++i) {
            v_call_order.push_back(i);
        }
    }

    const std::vector<int>& get_order_vector() {
        return v_call_order;
    }

    const std::string& get_color(int v) {
        return color[v];
    }

    void has_nei(int v) {}

    void start_vertex(int v) {}

    void discover_vertex(int v) {
        color[v] = "gray";
    }

    void finish_vertex(int v) {
        color[v] = "black";
    }
};



class Top_sort_visitor : public Visitor {
private:
    std::forward_list<int> sorted_v;
public:
    Top_sort_visitor(int v_number) : Visitor(v_number) {}

    void discover_vertex(int v) {
        color[v] = "gray";
    }

    void finish_vertex(int v) {
        sorted_v.push_front(v);
        color[v] = "black";
    }

    std::forward_list<int> get_sorted_v() {
        return sorted_v;
    }
};



class Scc_visitor : public Visitor {
private:
    std::vector<int> scc;
    int cur_scc;

public:
    Scc_visitor(int v_number, std::vector<int>& call_order) : Visitor(v_number), scc(v_number) {
        for (int i = 0; i < v_number; ++i) {
            v_call_order[i] = call_order[i];
        }
        cur_scc = -1;
    }

    void start_vertex(int v) {
        ++cur_scc;
    }

    void discover_vertex(int v) {
        color[v] = "gray";
        scc[v] = cur_scc;
    }

    void finish_vertex(int v) {
        color[v] = "black";
    }

    std::pair<int, std::vector<int>> get_kosaraju_pair() const {
        return {cur_scc + 1, scc};
    }
};


class Drain_visitor : public Visitor {
private:
    std::vector<bool> has_neighbours;
    int drain_number;
    int cur_time;
public:
    Drain_visitor(int v_number) : Visitor(v_number),  drain_number(0), cur_time(0), has_neighbours(v_number, false) {}

    void discover_vertex(int v) {
        color[v] = "gray";
    }

    void finish_vertex(int v) {
        color[v] = "black";
    }

    void has_nei(int v) {
        has_neighbours[v] = true;
    }

    int get_drain_number() {
        for (int i = 0; i < vertex_number; ++i) {
            if (!has_neighbours[i]) {
                ++drain_number;
            }
        }
        return drain_number;
    }

};



class Graph_2 {
private:
    std::vector<std::vector<int>> adj;
    int vertex_number;

    int get_vertex(int i, int j, int cols) {
        return i * cols + j;
    }

    std::pair<int, int> get_pos(int vertex, int cols) {
        return {vertex / cols, vertex % cols};
    }

    bool is_valid_pos(int i, int j, int rows, int cols) {
        return i >= 0 && i < rows && j >= 0 && j < cols;
    }

    std::vector<int> get_neighbours(const std::vector<std::vector<int>> & matrix, int rows, int cols, int i, int j) {
        std::vector<int> result;
        result.reserve(4);

        if (is_valid_pos(i, j - 1, rows, cols) && matrix[i][j - 1] <= matrix[i][j]) {
            result.push_back(get_vertex(i, j - 1, cols));
        }

        if (is_valid_pos(i - 1, j, rows, cols) && matrix[i - 1][j] <= matrix[i][j]) {
            result.push_back(get_vertex(i - 1, j, cols));
        }

        if (is_valid_pos(i, j + 1, rows, cols) && matrix[i][j + 1] <= matrix[i][j]) {
            result.push_back(get_vertex(i, j + 1, cols));
        }

        if (is_valid_pos(i + 1, j, rows, cols) && matrix[i + 1][j] <= matrix[i][j]) {
            result.push_back(get_vertex(i + 1, j, cols));
        }

        return result;
    }

    int hash_edge(int from, int to, int v_number) const {
        return from * v_number + to;
    }

public:
    Graph_2(int v_number) : vertex_number(v_number), adj(v_number, std::vector<int>()) {}

    Graph_2(const std::vector<std::vector<int>>& matrix) {
        int rows = matrix.size();
        int cols = matrix[0].size();
        vertex_number = rows * cols;

        adj.reserve(vertex_number);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                adj.push_back(get_neighbours(matrix, rows, cols, i, j));
            }
        }
    }

    void add_edge(int from, int to) {
        adj[from].push_back(to);
    }

    template<typename Visitor>
    void dfs(Visitor& vis) {
        for (int v : vis.get_order_vector()) {
            if (vis.get_color(v) == "white") {
                vis.start_vertex(v);
                dfs_visit(v, vis);
            }
        }
    }

    template<typename Visitor>
    void dfs_visit(int vertex, Visitor& vis) {
        vis.discover_vertex(vertex);
        for (int nei : adj[vertex]) {
            vis.has_nei(vertex);
            if (vis.get_color(nei) == "white") {
                dfs_visit(nei, vis);
            }
        }
        vis.finish_vertex(vertex);
    }

    std::vector<int> top_sort() {
        Top_sort_visitor top_sort_vis(vertex_number);
        dfs<Top_sort_visitor>(top_sort_vis);

        std::vector<int> sorted_v;
        sorted_v.reserve(vertex_number);
        for (int v : top_sort_vis.get_sorted_v()) {
            sorted_v.push_back(v);
        }
        return sorted_v;
    }

    Graph_2 get_transposed() const {
        Graph_2 trans_graph(vertex_number);
        for (int i = 0; i < vertex_number; ++i) {
            for (int nei : adj[i]) {
                trans_graph.add_edge(nei, i);
            }
        }
        return trans_graph;
    }

    std::pair<int, std::vector<int>> Kosaraju() {
        std::vector<int> sorted_vertexes = top_sort();
        Scc_visitor scc_visitor(vertex_number, sorted_vertexes);
        get_transposed().dfs<Scc_visitor>(scc_visitor);
        return scc_visitor.get_kosaraju_pair();
    }

    Graph_2 get_condensed() {
        std::pair<int, std::vector<int>> kos_pair = Kosaraju();
        int new_v_number = kos_pair.first;
        const std::vector<int>& scc_num = kos_pair.second;
        Graph_2 new_graph(new_v_number);

        std::unordered_set<int> un_set_edges;
        for (int i = 0; i < vertex_number; ++i) { /// O(|E|)
            for (int nei : adj[i]) {
                int hash_val_edge = hash_edge(scc_num[i], scc_num[nei], new_v_number);
                if (scc_num[i] != scc_num[nei] && un_set_edges.find(hash_val_edge) == un_set_edges.end()) {
                    un_set_edges.insert(hash_val_edge);
                    new_graph.add_edge(scc_num[i], scc_num[nei]);
                }
            }
        }
        return new_graph;
    }

    int find_drain_number() {
        Graph_2 new_graph = get_condensed();
        //new_graph.print();
        Drain_visitor drain_vis(new_graph.vertex_number);
        new_graph.dfs(drain_vis);
        return drain_vis.get_drain_number();
    }

    void print() const {
        for (int i = 0; i < vertex_number; ++i) {
            std::cout << "vertex " << i << ": ";
            for (int nei : adj[i]) {
                std::cout << nei << ' ';
            }
            std::cout << '\n';
        }
    }
};


////////////////////////////////////////////////////////////////////////////////////////



class Graph {
private:
    std::vector<std::vector<int>> graph;
    size_t dimM, dimN;
    std::vector<std::vector<bool>> used;
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
    explicit Graph(const std::vector<std::vector<int>>& map) : graph(map), dimM(graph.size()), dimN(graph[0].size()), drainNumber(0) {
        used.resize(dimM, std::vector<bool>());
        for (int i = 0; i < dimM; ++i) {
            used[i].resize(dimN, false);
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

    bool WillDrainBfs(const std::pair<int, int>& position) {
        std::queue<std::pair<int, int>> queue;
        queue.push(position);
        used[position.first][position.second] = true;

        do {
            std::pair<int, int> tmpPosition = queue.front();
            queue.pop();

            for (const auto& neighbour : getAdj(tmpPosition)) {
                if (getValue(neighbour) < getValue(tmpPosition) || completed[neighbour.first][neighbour.second]) {
                    completed[tmpPosition.first][tmpPosition.second] = true;
                    return false;
                }
            }

            for (const auto& neighbour : getAdj(tmpPosition)) {
                if (getValue(neighbour) == getValue(tmpPosition) && !used[neighbour.first][neighbour.second]) {
                    queue.push(neighbour);
                    used[neighbour.first][neighbour.second] = true;
                }
            }
        } while (!queue.empty());

        completed[position.first][position.second] = true;
        return true;
    }

    size_t CountDrainNumber() {
        drainNumber = 0;
        for (int i = 0; i < dimM; ++i) {
            for (int j = 0; j < dimN; ++j) {
                if (!completed[i][j]) {
                    drainNumber += WillDrainBfs({i, j});
                    usedReset();
                }
            }
        }
        return drainNumber;
    }

    ~Graph() = default;
};




////////////////////////////////////////////////////////////////////////////////////////



void stress_test() {
    std::random_device dev;
    std::mt19937 generator(dev());
    int max_high = 4;
    std::uniform_int_distribution<int> first_distribution(1, max_high);
    int rand_value = first_distribution(generator);
    
    
    int rows, cols;
    
    rows = 4;
    cols = 4;
    int test_number = 10000;
    
    bool status = true;
    for (int i = 0; i < test_number; ++i) {
        std::vector<std::vector<int>> matrix;
        matrix.resize(rows, std::vector<int>());
        for (auto& row : matrix) {
            row.reserve(cols);
            for (int j = 0; j < cols; ++j) {
                rand_value = first_distribution(generator);
                row.push_back(rand_value);
            }
        }
        
        Graph_2 graph_2(matrix);
        int correct_answer = graph_2.find_drain_number();
        
        Graph graph(matrix);
        int answer_2 = graph.CountDrainNumber();
        
        if (correct_answer != answer_2) {
            std::cout << "Error: test number - " << i + 1 << '\n';
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    std::cout << matrix[i][j] << ' ';
                }
                std::cout << '\n';
            }
            
            std::cout << "correct answer: " << correct_answer << '\n';
            std::cout << "BFS answer: " << answer_2 << '\n' << '\n';
            status = false;
            break;
        } else {
            if (i % 1000 == 0) {
                std::cout << "OK: test number - " << i + 1 << '\n';
                for (int i = 0; i < rows; ++i) {
                    for (int j = 0; j < cols; ++j) {
                        std::cout << matrix[i][j] << ' ';
                    }
                    std::cout << '\n';
                }
                
                std::cout << "correct answer: " << correct_answer << '\n';
                std::cout << "BFS answer: " << answer_2 << '\n' << '\n';
            }
        }
    }
    
    if (status) {
        std::cout << "All tests completed" << '\n';
    }
}


int main() {
    stress_test();
    return 0;
}
