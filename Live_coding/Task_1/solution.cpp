#include <iostream>
#include <vector>

// Даны два массива A и B. Нужно найти минимум abs(A[i] - B[j])
/*
A = {0}, B = {1}
Ответ: 1



A = {10, 0, 2}, B = {5, 100, 12}

//        it_1       it_2
sA= {0, 2, 10}, sB = {5, 12, 100}
Ответ: 2

*/

size_t min_dist(std::vector<int>& v_1, std::vector<int>& v_2) {
    std::sort(v_1.begin(), v_1.end());
    std::sort(v_2.begin(), v_2.end());
    size_t cur_dist = std::abs(v_2[0] - v_1[0]);
    size_t new_dist, it_1 = 0, it_2 = 1;
    size_t min_dist = cur_dist;
    bool is_new_it_1 = false;
    while (it_1 < v_1.size() && it_2 < v_2.size()) {
        new_dist = std::abs(v_1[it_1] - v_2[it_2]);
        if (is_new_it_1 || new_dist < cur_dist) {
            is_new_it_1 = false;
            cur_dist = new_dist;
            ++it_2;
            min_dist = std::min(min_dist, cur_dist);
        } else {
            ++it_1;
            --it_2;
            is_new_it_1 = true;
        }
    }
    return min_dist;
}

int main() {
    std::vector<int> v_1 = {10, 0, 2};
    std::vector<int> v_2 = {5, 100, 12};
    std::cout << min_dist(v_1, v_2);
    return 0;
}
