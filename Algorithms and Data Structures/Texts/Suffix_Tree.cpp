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

        void move_next() {
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

    };


public:

    Suffix_Tree(const std::string& text) : text(text) {
        root = new Node;
        root->suffix_link = nullptr;
        root->map.insert({text[0], {0, 0, nullptr}});
        Location loc(root, 0, 0, root, text);

        for (int i = 0; i < text.size(); ++i) {
            char new_sym = text[i];

            while (loc.is_leaf()) {
                loc.move_next();
            }

            // попали на первую нелистовую вершину

            bool processed = false;

            while (!processed) {


                if (loc.is_explicit()) {    /// если находимся в явной вершине
                    auto it = loc.expl_parent->map.find(new_sym);
                    const Edge& edge = it->second;
                    if (it == loc.expl_parent->map.end()) { // если не нашли new_sym в ветках
                        Edge new_edge = {i, i, nullptr};    // отращиваем ребро
                        loc.expl_parent->map.insert({new_sym, new_edge});
                    } else { // если нашли new_sym в каком-то ребре, тогда начинаем с этой локации на след.итерации
                        loc.delta_start = edge.start;
                        loc.delta_finish = edge.start;
                        processed = true;
                    }
                }

                else {    /// если находимся в неявной вершине
                    auto it = loc.expl_parent->map.find(text[loc.delta_start]);
                    Edge& loc_edge = it->second;
                    if (text[loc.delta_finish + 1] == new_sym) { // если new_sym лежит дальше по ребру, обновляем локацию для след.итерации
                        if (loc.delta_len() < loc_edge.get_len()) { // локация остаётся неявной, продлеваем finish на 1
                            ++loc.delta_finish;
                        } else {
                            loc.expl_parent = loc_edge.to; // локация становится явной
                            loc.delta_start = -1;
                            loc.delta_finish = -1;
                        }
                        processed = true;
                    }
                    else { // если new_sym нет дальше по ребру, тогда материализуем Node
                        Node* new_node = new Node;
                        if (loc_edge.get_len() > loc.delta_len()) {
                            int after_index = loc.delta_finish + 1;
                            new_node->map.insert({text[after_index], {after_index, loc_edge.finish, loc_edge.to}});
                        }
                        loc_edge.finish = loc.delta_finish; // +/- 1
                        loc_edge.to = new_node;
                        
                        new_node->map.insert({new_sym, {i, i, nullptr}});
                        //todo посчитать "руками" суффиксную ссылку для новой ноды и сохранить её в new_node->suffix_link
                        
                        loc.move_next();
                    }
                }
            }


        }

    }

};
