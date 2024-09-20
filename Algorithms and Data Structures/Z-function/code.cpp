#include <iostream>
#include <vector>

class Z_function {
private:
    std::vector<int> z_fun;
    int index_max_block;
    
    int find_len(const std::string& text, int first_index, int second_index) {
        int len = 0;
        for (int i = 0; second_index + i < text.size(); ++i) {
            if (text[first_index + i] == text[second_index + i]) {
                ++len;
            } else {
                break;
            }
        }
        return len;
    }
    
    
public:
    Z_function(const std::string& text) : z_fun(text.size()) {
        
        z_fun[0] = text.size();
        z_fun[1] = find_len(text, 0, 1);
        index_max_block = 1;
        
        for (int i = 2; i < text.size(); ++i) {
            int k = i - index_max_block;
            int l = z_fun[index_max_block] - k;
            if (z_fun[k] < l) { // no update the rightest block
                z_fun[i] = z_fun[k];
            } else { // use len of the rightest block 
                if (l < 0) {
                    z_fun[i] = find_len(text, 0, i);
                } else {
                    z_fun[i] = l + find_len(text, l, i + l);
                }
                index_max_block = i;
            }
        }
    }
    
    void print() const {
        for (int val : z_fun) {
            std::cout << val << ' ';
        }
    }
    
    const std::vector<int>& get_function() const {
        return z_fun;    
    }
};



class Solution {
private:
    std::string text;
    std::string sub_text;
    Z_function z_fun;
    
public:
    Solution(const std::string& text, const std::string& sub_text) : z_fun(sub_text + '$' + text), 
                                                                    text(text), sub_text(sub_text) {}
                                                                    
    void print_answer() const {
        for (int i = 0; i < z_fun.get_function().size(); ++i) {
            if (z_fun.get_function()[i] == sub_text.size()) {
                std::cout << i - 1 - sub_text.size() << ' '; 
            }
        }
    }
};



int main() {
    std::string text, sub_text;
    std::cin >> text >> sub_text;
    Solution solution(text, sub_text);
    solution.print_answer();
    return 0;
}