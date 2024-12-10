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

    class Location {
    private:
        Node* expl_parent;
        int delta_start;
        int delta_finish;

        const Node* const root;
        const std::string& text;
    public:

        Location(Node* expl_parent, int delta_start, int delta_finish,
                 const Node* const root, const std::string& text) : root(root), text(text) {}

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
                delta_finish = delta_start + cur_delta_len - 1; // надо ли в принципе хранить?
            }
        }

        bool is_root() const {
            return expl_parent == root && delta_len() == 0;
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
        
    }

};
