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
            //todo
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
            // точно знаем, что symbol есть в продолжении текущей локации
            const Edge& edge = expl_parent->map.find(symbol)->second;
            if (is_explicit()) {
                if (edge.get_len() > 1 || (edge.get_len() == 1 && edge.to == nullptr)) {
                    delta_start = edge.start;
                    delta_finish = edge.start;
                }
                else {
                    expl_parent = edge.to;
                    delta_start = -1;
                    delta_finish = -1;
                }
            }
            else {
                if (delta_finish == edge.finish - 1 && edge.to != nullptr) {
                    expl_parent = edge.to;
                    delta_start = -1;
                    delta_finish = -1;
                }
                else {
                    ++delta_finish;
                }
            }
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
