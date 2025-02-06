#include <iostream>
#include <vector>
#include <unordered_set>
#include <random>


#include <iostream>
#include <vector>
#include <unordered_set>

// preprocessing of all tails in O(n)
// Main idea: final_location = cur_loc + error_command + tail

class Solution {
private:
    const std::string& text;

    struct Tail {
        int cur_forward = 0;
        int left_forward = 0;
        int right_forward = 0;
    };

    std::vector<Tail> get_tails() {
        std::vector<Tail> v;
        v.resize(text.size());
        if (text[text.size() - 1] == 'F') {
            v[text.size() - 1].cur_forward = 1;
        }
        for (int i = text.size() - 2; i >= 0; --i) {
            v[i] = v[i + 1];
            if (text[i] == 'L') {
                v[i].left_forward += v[i].cur_forward;
                v[i].cur_forward = 0;
            }
            else if (text[i] == 'R') {
                v[i].right_forward += v[i + 1].cur_forward;
                v[i].cur_forward = 0;
            }
            else if (text[i] == 'F') {
                ++v[i].cur_forward;
            }
        }
        return v;
    }


    struct Location {
        int x;
        char dir;

        Location() = default;

        Location(int x, char dir) : x(x), dir(dir) {}

        Location operator+(char command) const {
            Location new_loc = *this;
            if (command == 'F') {
                new_loc.x += (new_loc.dir == 'R') ? 1 : -1;
            } else {
                new_loc.dir = command;
            }
            return new_loc;
        }

        Location& operator=(const Location& other) = default;

        int operator+(const Tail& seq) {
            x += (dir == 'R') ? seq.cur_forward : -seq.cur_forward;
            x += seq.right_forward;
            x -= seq.left_forward;
            return x;
        }
    };


public:
    Solution(const std::string& text) : text(text) {}

    int get_answer() {
        std::unordered_set<int> un_set;
        std::vector<Tail> tails = get_tails();

        Location cur_loc(0, 'R'), new_loc;
        int real_command, final_x;

        for (int i = 0; i < text.size(); ++i) {
            real_command = text[i];
            std::unordered_set<char> error_commands = {'L', 'R', 'F'};
            error_commands.extract(real_command);

            for (char error_command : error_commands) {
                new_loc = cur_loc + error_command;
                final_x = (i < text.size() - 1) ? new_loc + tails[i + 1] : new_loc.x;
                if (un_set.find(final_x) == un_set.end()) {
                    un_set.insert(final_x);
                }
            }
            cur_loc = cur_loc + real_command;
        }

        return un_set.size();
    }
};

int get_naive_x(const std::string& text) {
    int cur_x = 0;
    char cur_dir = 'R';

    for (int i = 0; i < text.size(); ++i) {
        if (text[i] == 'F') {
            cur_x += (cur_dir == 'R') ? 1 : -1;
        } else {
            cur_dir = text[i];
        }
    }

    return cur_x;
}


int naive_algo(const std::string& text) {
    std::unordered_set<int> un_set;
    int final_x;
    for (int i = 0; i < text.size(); ++i) {
        int real_command = text[i];
        std::unordered_set<char> error_commands = {'L', 'R', 'F'};
        error_commands.extract(real_command);

        for (char error_command : error_commands) {
            std::string new_text;
            for (int j = 0; j < i; ++j) {
                new_text += text[j];
            }
            new_text += error_command;
            for (int j = i + 1; j < text.size(); ++j) {
                new_text += text[j];
            }

            final_x = get_naive_x(new_text);
            if (un_set.find(final_x) == un_set.end()) {
                un_set.insert(final_x);
            }
        }
    }

    return un_set.size();
}


void stress_testing() {
    std::string letters = "LRF";
    std::random_device dev;
    std::mt19937 generator(dev());
    std::uniform_int_distribution<int> first_distribution(0, 2);

    bool success = true;

    for (int i = 0; i < 10000; ++i) {

        std::string text;
        int text_len = 1000;

        int rand_index;
        for (int j = 0; j < text_len; ++j) {
            rand_index = first_distribution(generator);
            text += letters[rand_index];
        }

        int answer_naive = naive_algo(text);
        Solution solution(text);
        int my_answer = solution.get_answer();
        std::cout << "test number: " << i + 1 << " : ";
        std::cout << text << ' ' << answer_naive << ' ' << my_answer << '\n';

        if (answer_naive != my_answer) {
            std::cout << "test number: " << i + 1 << '\n';
            std::cout << "Error: " << '\n' << text << '\n';
            std::cout << "naive answer: " << answer_naive << '\n';
            std::cout << "my answer: " << my_answer << '\n';
            success = false;
            break;
        }
    }

    if (success) {
        std::cout << "All tests passed.\n";
    }
}


int main() {
    stress_testing();
    return 0;
}
