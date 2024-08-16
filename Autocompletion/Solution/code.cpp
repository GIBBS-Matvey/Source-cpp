#include <iostream>
#include <vector>


class Solution {
private:
    bool started_with_prefix(const std::string& word, const std::string& prefix) {
        if (prefix.size() <= word.size()) {
            for (int i = 0; i < prefix.size(); ++i) {
                if (prefix[i] != word[i]) {
                    return false;
                }
            }
        } else {
            return false;
        }
        return true;

    }


    int lower_bound(const std::vector<std::string>& v_text, const std::string& prefix) {
        int start = 0;
        int finish = v_text.size() - 1;
        int middle;
        while (start <= finish) {
            middle = (start + finish) / 2;

            if (started_with_prefix(v_text[middle], prefix) && (middle == 0 || !started_with_prefix(v_text[middle - 1], prefix))) {
                return middle;
            }

            else if (v_text[middle] < prefix) {
                start = middle + 1;
            }

            else if (v_text[middle] >= prefix) {
                finish = middle - 1;
            }
        }
        return -1;
    }


public:
    int get_answer(const std::vector<std::string>& v_text, const std::string& prefix, int number) {
        int index = lower_bound(v_text, prefix);
        if (index != -1 && index + number - 1 < v_text.size() && started_with_prefix(v_text[index + number - 1], prefix)) {
            return index + number;
        } else {
            return -1;
        }
    }
};


int main() {
    int n, request_number;
    std::cin >> n >> request_number;
    std::vector<std::string> v_text;
    std::vector<std::string> v_prefix;
    v_text.reserve(n);
    v_prefix.reserve(request_number);
    for (int i = 0; i < n; ++i) {
        std::string text;
        std::cin >> text;
        v_text.push_back(text);
    }

    Solution solution;
    int number;
    std::string prefix;
    for (int i = 0; i < request_number; ++i) {
        std::cin >> number >> prefix;
        std::cout << solution.get_answer(v_text, prefix, number) << '\n';
    }

    return 0;
}
