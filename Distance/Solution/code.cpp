#include <iostream>
#include <vector>
#include <algorithm>


bool compare(const std::pair<size_t, size_t>& first, const std::pair<size_t, size_t>& second) {
    return first.first < second.first;
}


void init_prefix_sum(std::vector<size_t>& pref_s, const std::vector<std::pair<size_t, size_t>>& v) {
    pref_s.resize(v.size());
    pref_s[0] = v[0].first;
    for (size_t i = 1; i < v.size(); ++i) {
        pref_s[i] = pref_s[i - 1] + v[i].first;
    }
}


size_t f(size_t i, size_t l, size_t r, const std::vector<std::pair<size_t, size_t>>& v, const std::vector<size_t>& pref_s) {
    if (l < i) {
        return pref_s[r] - pref_s[i] - pref_s[i - 1] + pref_s[l - 1] + v[i].first * (2 * i - r - l);
    }
    else if (i < l) {
        size_t k = r - l + 1;
        return (pref_s[r] - pref_s[l - 1]) - v[i].first * k;
    }
    else if (i > r) {
        size_t k = r - l + 1;
        return v[i].first * k - (pref_s[r] - pref_s[l - 1]);
    }
}

// todo исправить: right - left = k - 1, если left > i
// todo исправить: right - left = k, если left < i
// todo => right = left + k - 1 * (left > i);

void shift(const size_t& i, size_t& left, size_t& right) { // k гарантировано больше 1
    if (left < i && i < right) {
        left += (left < i - 1) ? 1 : 2;
        right += 1;
    }
    else if (right < i) {
        left += 1;
        right += 2;
    }
}

void back(const size_t& i, size_t& l, size_t& r) { // k гарантировано больше 1
    //todo
}


void solution(std::vector<std::pair<size_t, size_t>>& v, size_t k) {
    std::sort(v.begin(), v.end(), compare);
    std::vector<std::pair<size_t, size_t>> result;
    result.resize(v.size());
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

        std::vector<size_t> pref_s;
        init_prefix_sum(pref_s, v);

        size_t l = 0;
        size_t r = k;


        result[0].first = f(0, l, r, v, pref_s);
        result[0].second = v[0].second;

        size_t prev_res, new_res;

        for (int i = 1; i < v.size(); ++i) { //надо наоборот: надо чекать новое значение, не меняя l, r
            if (l == i + 1) {
                result[i].first = f(i, l, r, v, pref_s);
                result[i].second = v[i].second;
                --l;
            }
            else {
                prev_res = f(i, l, r, v, pref_s);
                shift(i, l, r);
                new_res = f(i, l, r, v, pref_s);
                while (l < i + 1 && new_res < prev_res) {
                    prev_res = new_res;
                    shift(i, l, r);
                    new_res = f(i, l, r, v, pref_s);
                }
                result[i].first = std::min(prev_res, new_res);
                back(i, l, r);
                result[i].second = v[i].second;
            }
        }
    }
}


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
    solution(v, k);
    return 0;
}
