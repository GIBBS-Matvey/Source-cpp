#include <iostream>
#include <vector>
#include <algorithm>

bool compare(const std::pair<size_t, size_t>& first, const std::pair<size_t, size_t>& second) {
    return first.first < second.first;
}


void init_prefix_sum(std::vector<size_t>& pref_s, const std::vector<size_t>& v) {
    pref_s.resize(v.size());
    pref_s[0] = v[0];
    for (size_t i = 1; i < v.size(); ++i) {
        pref_s[i] = pref_s[i - 1] + v[i];
    }
}


size_t f(i, l, r, const std::vector<size_t>& v, const std::vector<size_t>& pref_s) {
    if (left < i) {
        return pref_s[r] - pref_s[i] - pref_s[i - 1] + pref_s[l - 1] + v[i] * (r + l - 2 * i);
    }
    size_t k = r - l + 1;
    else if (i < left) {
        return (pref_s[r] - pref_s[l - 1]) - v[i] * k;
    }
    else if (i < left) {
        return v[i] * k - (pref_s[r] - pref_s[l - 1]);
    }
}

void shift(const size_t& i, size_t& l, size_t& r) {
    if (i - left > 1) {
        ++l;
        ++r;
    }
    else if (i - left == 1) {
        left = i + 1;
        r += 2;
    }
}

void back(const size_t& i, size_t& l, size_t& r) {
    if (l == i + 1) {
        l -= 2;
        r -= 2;
    }
}


void solution(std::vector<std::pair<size_t, size_t>>& v, size_t k) {
    std::sort(v.begin(), v.end(), compare);
    std::vector<size_t> pref_s;
    init_prefix_sum(pref_s, v);
    
    std::vector<std::pair<size_t, size_t>> result;
    result.resize(v.size());
    size_t left = 0;
    size_t right = k;
    
    
    result[0].first = f(0, left, right, v, pref_s);
    result[0].second = v[0].second;
    
    size_t prev_res, new_res;
    
    for (int i = 1; i < v.size(); ++i) { //надо наоборот: надо чекать новое значение, не меняя l, r
        if (i == l) {
            --l;
        }
        prev_res = f(i, l, r);
        if (left < i + 1) {
            shift(i, l, r);   
        }
        new_res = f(i, l, r);
        while (new_res < prev_res && left < i + 1) {
            prev_res = new_res;
            shift(i, l, r);
            new_res = f(i, l, r);
        }
        result[i].first = std::min(prev_res, new_res);
        back(i, l, r);
        result[i].second = v[i].second;
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
