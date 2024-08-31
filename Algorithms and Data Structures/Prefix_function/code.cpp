#include <iostream>
#include <vector>


class Prefix_function {
private:
    std::vector<int> pref;
    int size;

public:
    Prefix_function(const std::string& text): size(text.size()) {
        pref.resize(size, 0);
        pref[0] = 0;
        
        for (int i = 1; i < size; ++i) {
            char new_symbol = text[i];
            int border_len = pref[i - 1];
            bool flag = false;
            
            while (text[border_len] != new_symbol && !flag) {
                if (border_len == 0) {
                    flag = true;
                } else {
                    border_len = pref[border_len - 1];   
                }
            }
            pref[i] = (flag) ? 0 : border_len + 1;
        }
    }

    void print() const {
        for (int val: pref) {
            std::cout << val << ' ';
        }
    }

    int get_value(int len) const {
        return pref[len - 1];
    }

};


int main() {
    std::string text;
    std::cin >> text;
    Prefix_function pref(text);
    pref.print();
    return 0;
}
