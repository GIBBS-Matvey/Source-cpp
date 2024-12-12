#include <iostream>
#include <map>

class Suffix_Tree {
private:

    struct Node;

    struct Edge {
        int start;
        int finish;
        Node* to;

        int get_len() const {
            return finish - start + 1;
        }
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

        const Node* const root;
        const std::string& text;

        Location(Node* expl_parent, int delta_start, int delta_finish,
                 const Node* const root, const std::string& text) : expl_parent(expl_parent), root(root), text(text),
                 delta_start(delta_start), delta_finish(delta_finish){}

        void move_suffix_link() {
            if (is_explicit()) {
                expl_parent = expl_parent->suffix_link;
            }
            else {
                char start_symbol;
                int cur_delta_start, cur_delta_len;

                if (expl_parent != root) {
                    expl_parent = expl_parent->suffix_link;
                    cur_delta_start = delta_start;  // индекс первого символа cur_delta
                    cur_delta_len = delta_len();    // текущая delta, которую надо отчитать
                } else {
                    cur_delta_start = delta_start + 1;  // если стоим в корне, тогда надо от корня отчитать строку на один символ короче
                    cur_delta_len = delta_len() - 1;
                }

                while (cur_delta_len > 0) { // пока есть что читать, оно точно прочитается

                    start_symbol = text[cur_delta_start];
                    auto it_edge = expl_parent->map.find(start_symbol);
                    const Edge& cur_edge = it_edge->second;

                    if (cur_edge.get_len() < cur_delta_len) {
                        cur_delta_start += cur_edge.get_len();
                        cur_delta_len -= cur_edge.get_len();
                        expl_parent = cur_edge.to;
                    }

                    else if (cur_edge.get_len() >= cur_delta_len) { // случай равенства означает, что мы в листе (важно ли?)
                        break;
                    }
                }

                delta_start = cur_delta_start;
                delta_finish = delta_start + cur_delta_len - 1; // надо ли в принципе хранить? upd: да
            }
        }

        bool is_root() const {
            return expl_parent == root && delta_start == -1;
        }

        bool is_leaf() const {
            if (!is_explicit()) {
                auto it = expl_parent->map.find(text[delta_start]);
                const Edge& edge = it->second;
                return edge.get_len() == delta_len();
            }
            return false;
        }

        int delta_len() const {
            return delta_finish - delta_start + 1;
        }

        bool is_explicit() const {
            return delta_start == -1;
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



        void add_symbol(char symbol, int i) {
            if (is_explicit()) {
                expl_parent->map.insert({symbol, {i, -1, nullptr}});
            }
            else {
                Edge& edge = expl_parent->map.find(text[delta_start])->second;
                
                int new_edge_finish = (edge.to == nullptr) ? i - 1 : edge.finish;
                int new_edge_start = delta_finish + 1;
                
                Node* new_node = new Node;
                new_node->map.insert({text[new_edge_start], {new_edge_start, new_edge_finish, edge.to}});
                new_node->map.insert({symbol, {i, -1, nullptr}});
                
                edge.finish = delta_finish;
                edge.to = new_node;
                //todo посчитать suffix_link для new_node
            }
        }




        void move_down(char symbol) {
            //todo
        }

    };


public:

    Suffix_Tree(const std::string& text) : text(text) {
        root = new Node;
        root->suffix_link = nullptr;
        root->map.insert({text[0], {0, 0, nullptr}});
        Location loc(root, 0, 0, root, text);

        char new_sym;

        for (int i = 0; i < text.size(); ++i) {
            new_sym = text[i];

            while (loc.is_leaf()) { /// скипаем все листы
                loc.move_suffix_link();
            }

            while (!loc.has_symbol_after(new_sym)) { /// продуктивный блок - добавление новых рёбер
                loc.add_symbol(new_sym, i);

                if (!loc.is_root()) {
                    loc.move_suffix_link();
                } else {
                    break; // частный случай: когда дошли loc-ом до корня, и у корня тоже нет new_sym
                }
            }

            loc.move_down(new_sym);
        }
    }

};
