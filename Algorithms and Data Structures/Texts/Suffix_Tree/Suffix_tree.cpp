#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <unordered_set>

class Suffix_Tree {
private:

    struct Node;
    struct Location;

    struct Edge {
        int start;
        int finish;
        Node* to;
    };

    struct Node {
        std::map<char, Edge> map;
        Node* suffix_link;
    };

    std::string text;
    Node* root;

    struct Location {
        Node* expl_parent;
        int delta_start;
        int delta_finish;
        const int& iter_index;

        Node* root;
        const std::string& text;

        Location(Node* expl_parent, int delta_start, int delta_finish,
                 Node* root, const std::string& text, const int& iter_index) : expl_parent(expl_parent), root(root), text(text),
                 delta_start(delta_start), delta_finish(delta_finish), iter_index(iter_index) {}



        void move_suffix_link() {
            // гарантировано не вызываем от корня
            if (is_explicit()) {
                expl_parent = expl_parent->suffix_link;
            }
            else {
                if (expl_parent == root) {
                    this->skip_count(expl_parent, delta_start + 1, delta_finish);
                }
                else {
                    this->skip_count(expl_parent->suffix_link, delta_start, delta_finish);
                }
            }
        }

        /*void print_loc() {
            std::cout << "expl_parent = " << expl_parent << '\n';
            std::cout << "delta_start = " << delta_start << '\n';
            std::cout << "delta_finish = " << delta_finish << '\n';
        }*/

        void skip_count(Node* node, int start, int finish) {
            // спускаемся из node в явную или неявную локацию (отчитываем T[start:finish])
            //std::cout << "call skip_count: start = " << start << ", finish = " << finish << '\n';
            int cur_len = finish - start + 1;
            if (cur_len <= 0) {
                this->expl_parent = node;
                this->delta_start = -1;
                this->delta_finish = -1;
            } else {
                int cur_start = start;
                const Edge* cur_edge = &(node->map.find(text[cur_start])->second);

                while (cur_len > get_edge_len(*cur_edge)) {
                    node = cur_edge->to;
                    cur_start += get_edge_len(*cur_edge);
                    cur_len -= get_edge_len(*cur_edge);
                    cur_edge = &(node->map.find(text[cur_start])->second);
                }

                if (cur_len < get_edge_len(*cur_edge)) {  // случай попадания в неявную локацию из неявной
                    this->expl_parent = node;
                    if (cur_len == 0) {
                        this->delta_start = -1;
                        this->delta_finish = -1;
                    } else {
                        this->delta_start = cur_edge->start;
                        this->delta_finish = cur_edge->start + cur_len - 1;
                    }
                }

                else if (cur_len == get_edge_len(*cur_edge)) {  // случай попадания в явную локацию из неявной
                    this->expl_parent = cur_edge->to;
                    this->delta_start = -1;
                    this->delta_finish = -1;
                }
            }
        }

        bool is_root() const {
            return expl_parent == root && delta_start == -1;
        }

        bool is_leaf() const {
            if (!is_explicit()) {
                auto it = expl_parent->map.find(text[delta_start]);
                const Edge& edge = it->second;
                //std::cout << "in leaf checking: edge_len = " << get_edge_len(edge) << " ; delta_len = " << delta_len() << '\n';
                return get_edge_len(edge) == delta_len();
            }
            return false;
        }

        int delta_len() const {
            return std::max(0, delta_finish - delta_start + 1);
        }

        bool is_explicit() const {
            return delta_start == -1 && delta_finish == -1;
        }

        bool has_symbol_after(char symbol) {
            // точно вызвали функцию не от листовой вершины
            if (is_explicit()) {
                return expl_parent->map.find(symbol) != expl_parent->map.end();
            }
            else {
                return text[delta_finish + 1] == symbol;
            }
        }

        int get_edge_len(const Edge& edge) const {
            int real_len = (edge.to == nullptr) ? iter_index - edge.start : edge.finish - edge.start + 1;
            return real_len;
        }

        void add_symbol(char symbol, int i) const {
            // гарантированно вызываем от явной локации, после которой нет symbol
            expl_parent->map.insert({symbol, {i, -1, nullptr}});
        }

        Node* create_new_node(char symbol, int i, Node* prev_node) const {
            // гарантированно вызываем от неявной локации, после которой нет symbol
            Edge& edge = expl_parent->map.find(text[delta_start])->second;

            int new_edge_finish = (edge.to == nullptr) ? i - 1 : edge.finish;
            int new_edge_start = delta_finish + 1;

            Node* new_node = new Node;
            new_node->map.insert({text[new_edge_start], {new_edge_start, new_edge_finish, edge.to}});
            new_node->map.insert({symbol, {i, -1, nullptr}});

            edge.finish = delta_finish;
            edge.to = new_node;

            if (prev_node != nullptr) {
                prev_node->suffix_link = new_node;
            }
            //std::cout << "cuting cur edge: edge.start = " << edge.start << " ; edge.finish = " << edge.finish << '\n';
            return new_node;
        }

        void move_down(char symbol) {
            // точно знаем, что symbol есть в продолжении текущей локации
            //std::cout << "move down description: " << '\n';
            const Edge& edge = expl_parent->map.find(symbol)->second;
            if (is_explicit()) {
                //std::cout << "cur_loc is explicit" << '\n';
                int real_edge_len = (edge.to == nullptr) ? get_edge_len(edge) + 1 : get_edge_len(edge);
                //std::cout << "edge_len = " << real_edge_len << '\n';
                if (real_edge_len > 1 || (real_edge_len == 1 && edge.to == nullptr)) {
                    //std::cout << "move down on one symbol" << '\n';
                    delta_start = edge.start;
                    delta_finish = edge.start;
                }
                else {
                    //std::cout << "move down to next node" << '\n';
                    expl_parent = edge.to;
                    delta_start = -1;
                    delta_finish = -1;
                }
            }
            else {
                //std::cout << "cur_loc is implicit" << '\n';
                if (delta_len() == get_edge_len(edge) - 1 && edge.to != nullptr) {
                    //std::cout << "move down to next node" << '\n';
                    expl_parent = edge.to;
                    delta_start = -1;
                    delta_finish = -1;
                } else {
                    //std::cout << "move down on one symbol" << '\n';
                    ++delta_finish;
                }
            }
        }
    };

    int get_edge_len(const Edge& edge) const {
        int real_len = (edge.to == nullptr) ? text.size() - edge.start : edge.finish - edge.start + 1;
        return real_len;
    }

    void print_edge(const Edge& edge) const {
        for (int i = edge.start; i < edge.start + get_edge_len(edge); ++i) {
            std::cout << text[i];
        }
        std::cout << '\n';
    }

public:

    Suffix_Tree(const std::string& text) : text(text) {
        root = new Node;
        root->suffix_link = nullptr;
        root->map.insert({text[0], {0, 0, nullptr}});
        int iter_index = 0;
        Location loc(root, 0, 0, root, text, iter_index);
        //std::cout << "created the first node with edge: " << text[0] << '\n';
        //std::cout << "put start loc on this edge" << "\n";

        //loc.print_loc();

        char new_sym;
        iter_index = 0;

        for (int i = 1; i < text.size(); ++i) {

            iter_index = i;
            new_sym = text[i];
            //std::cout << "\niter_index = " << iter_index << '\n';
            //std::cout << "ADDING NEW SYMBOL: " << text[i] << '\n';

            //std::cout << "beginning: ";
            /*if (loc.is_root()) {
                std::cout << "in root" << '\n';
            } else {
                std::cout << "not in root" << '\n';
            }
            std::cout << '\n';*/

            //loc.print_loc();
            /*if (!loc.is_leaf()) {
                std::cout << "no skipping, loc is not leaf" << '\n';
            }*/
            while (loc.is_leaf()) { // скипаем все листы
                //std::cout << "skip leaf" << '\n';
                loc.move_suffix_link();
                //loc.print_loc();
            }
            //std::cout << '\n';

            // попадаем в первую нелистовую вершину
            //std::cout << "stand on not-leaf" << '\n';
            Node* new_node = nullptr;
            bool added = false;

            while (!loc.has_symbol_after(new_sym)) {
                //std::cout << "no cur_symbol in continue" << '\n';
                if (loc.is_explicit()) {
                    //std::cout << "loc is explicit => add new edge with symbol " << new_sym << '\n';
                    loc.add_symbol(new_sym, i);
                    if (new_node && !added) {
                        //std::cout << "init suffix_link for prev node" << '\n';
                        new_node->suffix_link = loc.expl_parent;
                        added = true;
                    }
                } else {
                    new_node = loc.create_new_node(new_sym, i, new_node);   // продуктивный блок - добавление новых рёбер
                    //std::cout << "loc is implicit => create new node" << '\n';
                }

                if (!loc.is_root()) {
                    loc.move_suffix_link();
                    //std::cout << "move suffix_link" << '\n';
                    //loc.print_loc();
                } else {
                    //std::cout << "no move, because stand in root" << '\n';
                    break;
                }
            }

            if (new_node && !added) {
                new_node->suffix_link = loc.expl_parent;
            }
            //std::cout << "move down on symbol " << new_sym << '\n';
            loc.move_down(new_sym);
            //loc.print_loc();

            // todo возможно, сейчас проблема с инициализацией suffix_link для новых нод
        }
    }

    bool has_substring(const std::string& target) {
        Node* cur_node = root;
        int cur_index = 0;

        while (true) {

            auto it = cur_node->map.find(target[cur_index]);
            if (it == cur_node->map.end()) {
                return false;
            }
            const Edge* edge_ptr = &it->second;

            int edge_index = edge_ptr->start;
            int edge_finish = (edge_ptr->to != nullptr) ? edge_ptr->finish : text.size() - 1;
            while (edge_index <= edge_finish) {
                if (text[edge_index] != target[cur_index]) {
                    return false;
                }
                ++edge_index;
                ++cur_index;
                if (cur_index == target.size()) {
                    return true;
                }
            }

            if (edge_ptr->to) {
                cur_node = edge_ptr->to;
            } else {
                return false;
            }
        }

    }

    void bfs_print_tree() const {
        Node* cur;
        std::queue<Node*> q;
        q.push(root);
        int level_number = 0;
        while (!q.empty()) {
            cur = q.front();
            ++level_number;
            q.pop();
            std::cout << "level_number: " << level_number << '\n';
            for (const auto& pair : cur->map) {
                const Edge& edge = pair.second;
                print_edge(edge);
                if (edge.to) {
                    q.push(edge.to);
                }
            }
            std::cout << '\n';
        }
    }

    int substrings_number() const {
        Node* cur;
        std::queue<Node*> q;
        q.push(root);
        int answer = 0;
        while (!q.empty()) {
            cur = q.front();
            q.pop();
            for (const auto& pair : cur->map) {
                const Edge& edge = pair.second;
                answer += get_edge_len(edge);
                if (edge.to) {
                    q.push(edge.to);
                }
            }
        }
        return answer;
    }

    ~Suffix_Tree() {
        //todo
    }
};


int naive_substrings_number(const std::string& text) {
    std::unordered_set<std::string> h_t;

    for (int i = 0; i < text.size(); ++i) {
        for (int j = i; j < text.size(); ++j) {
            std::string sub_str;
            for (int k = i; k <= j; ++k) {
                sub_str += text[k];
            }
            if (h_t.find(sub_str) == h_t.end()) {
                h_t.insert(sub_str);
            }
        }
    }

    return h_t.size();
}

void stress_testing() {
    int test_number = 1000;
    int max_len = 8;
    std::string text;
    std::random_device dev;
    std::mt19937 generator(dev());
    std::uniform_int_distribution<int> first_distribution(97, 99);

    int rand_index;
    bool success = true;

    for (int j = 0; j < test_number; ++j) {
        text = "";
        for (int i = 0; i < max_len; ++i) {
            rand_index = first_distribution(generator);
            text += char(rand_index);
        }

        Suffix_Tree tree(text);
        int my_answer = tree.substrings_number();
        int correct_answer = naive_substrings_number(text);

        std::cout << "test number: " << j + 1 << '\n';
        std::cout << text << '\n';
        if (my_answer != correct_answer) {
            std::cout << "Error: correct answer: " << correct_answer << " ; my_answer: " << my_answer << '\n';
            std::cout << "text: " << text << '\n';
            tree.bfs_print_tree();
            success = false;
            break;
        } else {
            std::cout << "success: correct answer: " << correct_answer << " ; my_answer: " << my_answer << '\n';
        }
        std::cout << '\n';
    }

    if (success) {
        std::cout << "All tests are completed.\n";
    }
}


int main() {
    Suffix_Tree tree("cbcbacbb");
    tree.bfs_print_tree();
    return 0;
}
