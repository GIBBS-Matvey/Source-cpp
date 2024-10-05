#include <iostream>
#include <fstream>
#include <vector>
#include <random>

const size_t kPrimeNumber = 2000011;


class Graph {
private:
    std::vector<std::vector<int>> adj;
    size_t vertex_number;
    std::vector<std::string> colors;
    std::vector<bool> has_drain;
    int rows;
    int cols;
    const std::vector<std::vector<int>>& matrix;
    int drain_number;

    int get_vertex(int i, int j) {
        return i * cols + j;
    }

    std::pair<int, int> get_pos(int vertex) {
        return {vertex / cols, vertex % cols};
    }

    bool is_valid_pos(int i, int j) {
        return i >= 0 && i < rows && j >=0 && j < cols;
    }

    std::vector<int> get_neighbours(int i, int j) {
        std::vector<int> result;
        result.reserve(4);

        std::vector<int> highs;
        highs.reserve(4);

        if (is_valid_pos(i, j - 1) && matrix[i][j - 1] <= matrix[i][j]) {
            result.push_back(get_vertex(i, j - 1));
            highs.push_back(matrix[i][j - 1]);
        }

        if (is_valid_pos(i - 1, j) && matrix[i - 1][j] <= matrix[i][j]) {
            result.push_back(get_vertex(i - 1, j));
            highs.push_back(matrix[i - 1][j]);
        }

        if (is_valid_pos(i, j + 1) && matrix[i][j + 1] <= matrix[i][j]) {
            result.push_back(get_vertex(i, j + 1));
            highs.push_back(matrix[i][j + 1]);
        }

        if (is_valid_pos(i + 1, j) && matrix[i + 1][j] <= matrix[i][j]) {
            result.push_back(get_vertex(i + 1, j));
            highs.push_back(matrix[i + 1][j]);
        }

        for (int k = 0; k < result.size(); ++k) {
            for (int l = 0; l < result.size() - 1 - k; ++l) {
                if (highs[l] > highs[l + 1]) {
                    std::swap(highs[l], highs[l + 1]);
                    std::swap(result[l], result[l + 1]);
                }
            }
        }

        return result;
    }



public:
    Graph(const std::vector<std::vector<int>>& matrix) : matrix(matrix), rows(matrix.size()), cols(matrix[0].size()),
    drain_number(0) {
        vertex_number = rows * cols;
        colors.resize(vertex_number, "white");
        has_drain.resize(vertex_number, false);

        adj.reserve(vertex_number);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                adj.push_back(get_neighbours(i, j));
            }
        }
    }

    void Dfs_visit() {
        for (int i = 0; i < vertex_number; ++i) {
            if (colors[i] == "white") {
                Dfs(i);
                //std::cout << "has_drain_matrix: " << '\n';
                //print_has_drain_matrix();
                //std::cout << '\n';
            }
        }
    }

    void Dfs(int vertex) {
        //print_path(vertex);
        colors[vertex] = "gray";
        std::vector<int> neighbours = adj[vertex];

        for (int nei : neighbours) {
            if (has_drain[nei]) {
                has_drain[vertex] = true;
            }
        }

        if (!has_drain[vertex]) {
            bool has_path = false;
            for (int nei : neighbours) {
                if (colors[nei] == "white") {
                    has_path = true;
                    Dfs(nei);
                    break;
                }
            }

            if (!has_path) {
                ++drain_number;
                //std::cout << "drain_here: " << get_pos(vertex).first + 1 << ' ' <<  get_pos(vertex).second + 1 << '\n';
                //print_has_drain_matrix();
            }
            has_drain[vertex] = true;
        }

        colors[vertex] = "black";
    }

    int find_drain_number() {
        Dfs_visit();
        return drain_number;
    }

    void print_path(int vertex) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (get_vertex(i, j) == vertex) {
                    std::cout << '+' << ' ';
                } else {
                    std::cout << '_' << ' ';
                }
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    void print_has_drain_matrix() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                std::cout << has_drain[get_vertex(i, j)] << ' ';
            }
            std::cout << '\n';
        }
    }
};


void stress_test() {
    std::random_device dev;
    std::mt19937 generator(dev());
    std::uniform_int_distribution<size_t> first_distribution(1, 3);

    std::vector<std::vector<int>> matrix;
    matrix.resize(4, std::vector<int>());
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            int random_value = first_distribution(generator);
            matrix[i].push_back(random_value);
        }
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';

    Graph graph(matrix);
    std::cout << graph.find_drain_number();
}


int main() {
    int rows, cols;
    //std::ifstream fin("C:\\Users\\PC\\CLionProjects\\untitled\\input.txt");
    std::cin >> rows >> cols;
    std::vector<std::vector<int>> matrix;
    matrix.resize(rows, std::vector<int>());
    for (std::vector<int>& row : matrix) {
        row.resize(cols);
    }
    for (auto& row : matrix) {
        for (auto& cell : row) {
            std::cin >> cell;
        }
    }

    Graph graph(matrix);
    std::cout << graph.find_drain_number();

    //stress_test();

    return 0;
}
