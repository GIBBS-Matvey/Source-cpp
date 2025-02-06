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


int main() {
    int n;
    std::cin >> n;
    std::string text;
    std::cin >> text;
    Solution solution(text);
    std::cout << solution.get_answer();
    return 0;
}
