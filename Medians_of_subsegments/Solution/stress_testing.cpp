#include <iostream>
#include <vector>
#include <algorithm>


class Solution {
private:
    const std::vector<int>& v;
    int target;
    int answer;

public:
    Solution(const std::vector<int>& values, int target) : v(values), target(target), answer(1) {
        int index_target = std::find(v.begin(), v.end(), target) - v.begin();
        std::vector<int> left_parts = get_left_parts(index_target);
        std::vector<int> right_parts = get_right_parts(index_target);

        std::sort(left_parts.begin(), left_parts.end());
        std::sort(right_parts.begin(), right_parts.end());

        auto lower_it_left = std::lower_bound(left_parts.begin(), left_parts.end(), 0);
        auto upper_it_left = std::upper_bound(left_parts.begin(), left_parts.end(), 0);
        auto lower_it_right = std::lower_bound(right_parts.begin(), right_parts.end(), 0);
        auto upper_it_right = std::upper_bound(right_parts.begin(), right_parts.end(), 0);

        answer += (upper_it_left - lower_it_left) + (upper_it_right - lower_it_right);
        if (!left_parts.empty() && !right_parts.empty()) {
            join_left_right_parts(left_parts, right_parts);
        }
    }

    std::vector<int> get_left_parts(int index) {
        std::vector<int> left_parts;
        if (index > 0) {
            left_parts.reserve(index - 1);
            int balance = 0; // ++ if v[i] > target
            for (int i = index - 1; i >= 0; --i) {
                balance += (v[i] > target) ? 1 : -1;
                left_parts.push_back(balance);
            }
        }
        return left_parts;
    }

    std::vector<int> get_right_parts(int index) {
        std::vector<int> right_parts;
        if (index < v.size() - 1) {
            right_parts.reserve(v.size() - index - 1);
            int balance = 0; // ++ if v[i] > target
            for (int i = index + 1; i < v.size(); ++i) {
                balance += (v[i] > target) ? 1 : -1;
                right_parts.push_back(balance);
            }
        }
        return right_parts;
    }

    void join_left_right_parts(const std::vector<int>& left_parts, const std::vector<int>& right_parts) {
        for (int balance : left_parts) {
            auto lower_it = std::lower_bound(right_parts.begin(), right_parts.end(), -balance);
            auto upper_it = std::upper_bound(right_parts.begin(), right_parts.end(), -balance);
            answer += upper_it - lower_it;
        }
    }

    int get_answer() {
        return answer;
    }
};


class Trivial_Solution {
private:
    const std::vector<int>& v;
    int target;
    int answer;
public:
    Trivial_Solution(const std::vector<int>& values, int target) : v(values), target(target), answer(0) {
        int index_target = std::find(v.begin(), v.end(), target) - v.begin();
        for (int left = index_target; left >= 0; --left) {
            for (int right = index_target; right < v.size(); ++right) {
                if ((right - left) % 2 == 0) {
                    answer += is_suitable(left, right);
                }
            }
        }
    }

    bool is_suitable(int left, int right) {
        int balance = 0;
        for (int i = left; i <= right; ++i) {
            if (v[i] != target) {
                balance += (v[i] > target) ? 1 : -1;
            }
        }
        return (balance == 0);
    }

    int get_answer() {
        return answer;
    }
};

void stress_testing() {
    //todo
}

int main() {
    size_t n;
    std::cin >> n;
    int target;
    std::cin >> target;
    std::vector<int> v(n);
    for (size_t i = 0; i < v.size(); ++i) {
        std::cin >> v[i];
    }
    Trivial_Solution tr_solution(v, target);
    std::cout << tr_solution.get_answer();
    return 0;
}
