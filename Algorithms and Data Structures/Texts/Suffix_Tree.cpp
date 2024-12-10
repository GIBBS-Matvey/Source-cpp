#include <iostream>
#include <map>

class Suffix_Tree {
private:
    
    std::string text;
    
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
    }
    
    Node* root;
    
public:

    
    class Location {
    private:
        Node* expl_parent;
        int delta_start;
        int delta_finish;
    
    public:
    
        void move_next() {
            if (is_explicit()) {
                expl_parent = expl_parent->suffix_link;
            }
            else {
                char start_symbol;
                int cur_delta_start, cur_delta_len;
                
                if (expl_parent != root) {
                    expl_parent = expl_parent->suffix_link;
                    cur_delta_start = delta_start;  // текущая delta, которую надо отчитать
                    cur_delta_len = delta_len();    // индекс первого символа cur_delta
                } else {
                    cur_delta_start = delta_start + 1;  // если стоим в корне, тогда надо от корня отчитать строку на один символ короче
                    cur_delta_len = delta_len() - 1;
                }
                
                while (cur_delta_len > 0) { // пока есть что читать
                    
                    start_symbol = text[cur_delta_start];
                    auto it_edge = expl_parent->map.find(start_symbol);
                    const Edge& cur_edge = it->second;
                    
                    if (cur_edge.get_len() < cur_delta_len) {
                        cur_delta_start += cur_edge.get_len();
                        cur_delta_len -= cur_edge.get_len();
                        expl_parent = cur_edge.to;
                    }
                    
                    else if (cur_edge.get_len() >= cur_delta_len) { // случай равенства означает, что мы в листе
                        break;
                    }
                }
                
                delta_start = cur_delta_start;
                delta_finish = delta_start + cur_delta_len - 1;
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
            } else {
                return false;
            }
        }
        
    
        int delta_len() const {
            return delta_finish - delta_start + 1;
        }
        
        bool is_explicit() const {
            return delta_len() == 0;
        }
        
    };
    
    Suffix_tree(const std::string& text) : text(text) {
        //todo
    }
    
    
    
    
};
