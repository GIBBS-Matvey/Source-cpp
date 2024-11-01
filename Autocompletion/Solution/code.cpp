#include <iostream>
#include <vector>
#include <map>
#include <queue>


struct Request {
    char type;
    char symbol;
    Request(char type, char symbol = '$') : type(type), symbol(symbol) {}
};


class Bor {
private:
    struct Node {
        std::map<char, Node*> map;
        Node* parent;
        char symbol;
        int max_val;
        int index;

        Node(char symbol, Node* parent, int max, int index) : symbol(symbol), parent(parent), max_val(max), index(index) {}
    };

    Node* root;

public:
    Bor() : root(nullptr) {}

    void push_word(const std::string& word, std::size_t popularity, int index) {
        if (root == nullptr) {
            Node* new_node = new Node('$', nullptr, 0, -1);
            root = new_node;
        }

        Node* tmp = root;
        if (popularity > tmp->max_val) {
            tmp->max_val = popularity;
        }    
        for (int i = 0; i < word.size(); ++i) {
            auto it = tmp->map.find(word[i]);
            if (it != tmp->map.end()) {
                tmp = it->second;
                if (popularity > tmp->max_val) {
                    tmp->max_val = popularity;
                    tmp->index = index;
                }
            } else {
                Node* new_node = new Node(word[i], tmp, popularity, index);
                tmp->map.insert({word[i], new_node});
                tmp = new_node;
            }
        }
    }

    void process_reqs(const std::vector<Request>& v_req) {
        if (!v_req.empty()) {
            Node* tmp = root;
            for (const auto& request : v_req) {
                if (request.type == '+') {
                    auto it = tmp->map.find(request.symbol);
                    if (it != tmp->map.end()) {
                        tmp = it->second;
                        std::cout << tmp->index + 1 << '\n';
                    } else {
                        std::cout << -1 << '\n';
                    }
                } else {
                    tmp = tmp->parent;
                    std::cout << tmp->index + 1 << '\n';
                }
            }
        }
    }

    ~Bor() {
        if (root != nullptr) {
            std::queue<Node*> q;
            q.push(root);
            Node* tmp;
            while(!q.empty()) {
                tmp = q.front();
                q.pop();
                for (const auto& pair : tmp->map) {
                    q.push(pair.second);
                }
                delete tmp;
            }
        }
    }
};


class Solution {
private:
    Bor bor;

public:
    Solution(const std::vector<std::pair<std::string, std::size_t>>& v_word) {
        for (int i = 0; i < v_word.size(); ++i) {
            bor.push_word(v_word[i].first, v_word[i].second, i);
        }
    }

    void process_reqs(const std::vector<Request>& v_req) {
        bor.process_reqs(v_req);
    }
};


int main() {
    std::vector<std::pair<std::string, std::size_t>> v_word;
    int n, req_number;
    std::cin >> n >> req_number;

    v_word.reserve(n);

    std::string word;
    std::size_t popularity;
    for (int i = 0; i < n; ++i) {
        std::cin >> word >> popularity;
        v_word.push_back({word, popularity});
    }

    std::vector<Request> v_req;
    v_req.reserve(req_number);
    char type, symbol;
    for (int i = 0; i < req_number; ++i) {
        std::cin >> type;
        if (type == '+') {
            std::cin >> symbol;
        }
        v_req.push_back({type, symbol});
    }

    Solution solution(v_word);
    solution.process_reqs(v_req);
    return 0;
}
