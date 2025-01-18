#include <iostream>
#include <vector>
#include <algorithm>


bool compare_val(const std::pair<size_t, size_t>& first, const std::pair<size_t, size_t>& second) {
    return first.first < second.first;
}

bool compare_number(const std::pair<size_t, size_t>& first, const std::pair<size_t, size_t>& second) {
    return first.second < second.second;
}

class Solution {
private:
    std::vector<std::pair<size_t, size_t>> v;
    size_t k;

    std::vector<size_t> pref_s;
    std::vector<std::pair<size_t, size_t>> result;
    size_t left;
    size_t right;

    void init_prefix_sum() {
        pref_s.resize(v.size());
        pref_s[0] = v[0].first;
        for (size_t i = 1; i < v.size(); ++i) {
            pref_s[i] = pref_s[i - 1] + v[i].first;
        }
    }

    size_t dist(size_t i, size_t l, size_t r) {
        if (l < i) {
            size_t more_elems, less_elems;
            more_elems = pref_s[r] - pref_s[i] - (r - i) * v[i].first;
            less_elems = (i - l) * v[i].first - (pref_s[i - 1] - pref_s[l - 1] * (l > 0));
            return less_elems + more_elems;
        }
        else if (i < l) {
            return (pref_s[r] - pref_s[l - 1] * (l > 0)) - v[i].first * k;
        }
        else if (i > r) {
            return v[i].first * k - (pref_s[r] - pref_s[l - 1] * (l > 0));
        }
    }

    size_t l_next(size_t i) {
        if (r_next(i) != right) {
            size_t new_l = left + 1;
            new_l += (new_l == i) ? 1 : 0;
            return new_l;
        } else {
            return left;
        }
    }

    size_t r_next(size_t i) {
        size_t new_r = right + 1;
        new_r += (new_r == i) ? 1 : 0;
        if (new_r < v.size()) {
            return new_r;
        } else {
            return right;
        }
    }

public:
    Solution(const std::vector<std::pair<size_t, size_t>>& values, size_t k) : v(values), k(k), left(1), right(k) {

        std::sort(v.begin(), v.end(), compare_val);
        result.resize(v.size());
        init_prefix_sum();

        if (k == 1) {
            result[0].first = v[1].first - v[0].first;
            result[0].second = v[0].second;
            for (size_t i = 1; i < v.size() - 1; ++i) {
                result[i].first = std::min(v[i].first - v[i - 1].first, v[i + 1].first - v[i].first);
                result[i].second = v[i].second;
            }
            result[v.size() - 1].first = v[v.size() - 1].first - v[v.size() - 2].first;
            result[v.size() - 1].second = v[v.size() - 1].second;
        }

        else if (k > 1) {
            result[0].first = dist(0, 1, k);
            result[0].second = v[0].second;

            size_t cur_res, new_res;
            for (size_t i = 1; i < v.size(); ++i) {
                left -= (i == left) ? 1 : 0;
                right -= (i == right) ? 1 : 0;
                cur_res = dist(i, left, right);
                new_res = dist(i, l_next(i), r_next(i));

                while (new_res < cur_res) {
                    left = l_next(i);
                    right = r_next(i);
                    cur_res = new_res;
                    new_res = dist(i, l_next(i), r_next(i));
                }

                result[i].first = cur_res;
                result[i].second = v[i].second;
            }
        }
    }

    void get_answer() {
        std::sort(result.begin(), result.end(), compare_number);
        for (const std::pair<size_t, size_t>& pair : result) {
            std::cout << pair.first << ' ';
        }
    }
};


int main() {
    size_t n, k;
    std::cin >> n >> k;
    std::vector<std::pair<size_t, size_t>> v;
    v.reserve(n);
    size_t val;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> val;
        v.push_back({val, i});
    }
    Solution solution(v, k);
    solution.get_answer();
    return 0;
}
