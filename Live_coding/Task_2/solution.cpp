#include <iostream>
#include <vector>

// Даётся массив из 0 и 1. Найти длину макимального подотрезка, состоящего из 1, после удаления ровно одного нуля
// [1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0]
size_t max_subarray_with_1(const std::vector<int>& v) {
    // в каждом префиксе найдём суффикс с масксимальной суммой
    std::vector<int> prefix_results;
    prefix_results.resize(v.size());
    prefix_results[0] = 1;
    bool has_zero = (v[0] == 0);
    int count = (v[0] == 1) ? 1 : 0;
    for (int i = 1; i < v.size(); ++i) {
        if (v[i] == 0) {
            prefix_results[i] = (has_zero) ? count + 1 : prefix_results[i - 1] + 1;
            has_zero = true;
            count = 0;
        } else {
            ++count;
            prefix_results[i] = prefix_results[i - 1] + 1;
        }
    }
    int result = *std::max_element(prefix_results.begin(), prefix_results.end());
    result -= (has_zero);
    return result;
}

int main() {
    std::vector<int> v = {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0};
    std::cout << max_subarray_with_1(v);
    return 0;
}
