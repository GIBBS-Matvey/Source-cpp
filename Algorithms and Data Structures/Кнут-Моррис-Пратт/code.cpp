#include <iostream>
#include <vector>


class Prefix_function {
private:
    std::vector<int> pref;
    int size;

public:
    Prefix_function(const std::string& text): size(text.size()) {
        pref.resize(size, 0);
        pref[0] = 0; // у односимвольных строк pref-function всегда 0, ибо мы рассматриваем max_border, не              совпадающий со всей строкой
        
        for (int i = 1; i < size; ++i) {
            char new_symbol = text[i];
            int border_len = pref[i - 1];
            bool flag = false;
            
            while (text[border_len] != new_symbol && !flag) {
                if (border_len == 0) {
                    flag = true;
                }
                border_len = pref[border_len];
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


class KMP {
private:
    std::string text;
    std::string sub_text;
    Prefix_function prefix_function;
    
public:
    KMP(const std::string& text, const std::string& sub_text) : prefix_function(sub_text), text(text), sub_text(sub_text) {}
    
    int find_sub_text() {
        int compare_index = 0;
        int j = 0;
        bool found = false;
        int k = 0;
        int delta = 0;
        
        while (j < text.size() && compare_index < sub_text.size()) {
            if (text[j] == sub_text[compare_index]) {
                ++j;
                ++compare_index;
                ++k;
                if (k == sub_text.size()) {
                    return j - k;
                }
            } else {
                if (k == 0) {
                    ++j;
                }
                k = prefix_function.get_value(k);
                compare_index = k;
            }
        }
        return -1;
    }
};


int main() {
    //std::string text = "abacabaz abacabax abacabam";
    //std::string p = "abacabaxx";
    std::cin >> text >> p;
    KMP kmp(text, p);
    std::cout << kmp.find_sub_text();
    return 0;
}
