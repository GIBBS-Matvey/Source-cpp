#include <iostream>
#include <vector>
#include <algorithm>
#include <random>


bool compare_val(const std::pair<long long int, long long int>& first, const std::pair<long long int, long long int>& second) {
    return first.first < second.first;
}

bool compare_number(const std::pair<long long int, long long int>& first, const std::pair<long long int, long long int>& second) {
    return first.second < second.second;
}

class Solution {
private:
    std::vector<std::pair<long long int, long long int>> v;
    long long int k;

    std::vector<long long int> pref_s;
    std::vector<std::pair<long long int, long long int>> result;
    long long int left;
    long long int right;

    void init_prefix_sum() {
        pref_s.resize(v.size());
        pref_s[0] = v[0].first;
        for (long long int i = 1; i < v.size(); ++i) {
            pref_s[i] = pref_s[i - 1] + v[i].first;
        }
    }

    long long int dist(long long int i, long long int l, long long int r) {
        if (l < i) {
            long long int more_elems, less_elems;
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

    long long int l_next(long long int i) {
        if (r_next(i) != right) {
            long long int new_l = left + 1;
            new_l += (new_l == i) ? 1 : 0;
            return new_l;
        } else {
            return left;
        }
    }

    long long int r_next(long long int i) {
        long long int new_r = right + 1;
        new_r += (new_r == i) ? 1 : 0;
        if (new_r < v.size()) {
            return new_r;
        } else {
            return right;
        }
    }

public:
    Solution(const std::vector<std::pair<long long int, long long int>>& values, long long int k) : v(values), k(k), left(1), right(k) {

        std::sort(v.begin(), v.end(), compare_val);
        result.resize(v.size());
        init_prefix_sum();

        if (k == 1) {
            result[0].first = v[1].first - v[0].first;
            result[0].second = v[0].second;
            for (long long int i = 1; i < v.size() - 1; ++i) {
                result[i].first = std::min(v[i].first - v[i - 1].first, v[i + 1].first - v[i].first);
                result[i].second = v[i].second;
            }
            result[v.size() - 1].first = v[v.size() - 1].first - v[v.size() - 2].first;
            result[v.size() - 1].second = v[v.size() - 1].second;
        }

        else if (k > 1) {
            result[0].first = dist(0, 1, k);
            result[0].second = v[0].second;

            long long int cur_res, new_res;
            for (long long int i = 1; i < v.size(); ++i) {
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

    std::vector<long long int> get_answer() {
        std::sort(result.begin(), result.end(), compare_number);
        std::vector<long long int> answer;
        answer.reserve(result.size());
        for (const std::pair<long long int, long long int>& pair : result) {
            answer.push_back(pair.first);
        }
        return answer;
    }
};



///////////////////////////////////////////////////////////////////

int parent(int index) {
    return (index - 1) / 2;
}

int leftChild(int index) {
    return 2 * index + 1;
}

int rightChild(int index) {
    return 2 * index + 2;
}

void siftDown(int* index_array, int *heap, int size, int index) {
    while ((leftChild(index) <= size - 1 && heap[leftChild(index)] < heap[index]) ||
    (rightChild(index) <= size - 1 && heap[rightChild(index)] < heap[index])) {
        int indexMin = (rightChild(index) <= size - 1 && heap[rightChild(index)] < heap[leftChild(index)]) ? rightChild(index) : leftChild(index);
        std::swap(heap[indexMin], heap[index]);
        std::swap(index_array[indexMin], index_array[index]);
        index = indexMin;
    }
}


void BuildHeap(int* index_array, int* array, const int& size) {
    for (int i = parent(size - 1); i >= 0; --i) {
        siftDown(index_array, array, size, i);
    }
}

void HeapSort(int* index_array, int* array, const int& size) {
    BuildHeap(index_array, array, size);
    int temp_size = size;
    for (int i = 0; i < size; ++i) {
        std::swap(array[0], array[temp_size - 1]);
        std::swap(index_array[0], index_array[temp_size - 1]);
        --temp_size;
        siftDown(index_array, array, temp_size, 0);
    }
}



bool is_valid_position(int i, int n) {
    return (i >= 0) && (i < n);
}

int dist(int* array, int n, int target, int nei) {
    if (is_valid_position(nei, n)) {
        return abs(array[target] - array[nei]);
    } else {
        return INT32_MAX;
    }
}

int f(int* array, int size, int k, int target_index) {
    int right_nei = target_index + 1;
    int left_nei = target_index - 1;
    int result = 0;
    for (int i = 0; i != k; ++i) {
        if (dist(array, size, target_index, left_nei) < dist(array, size, target_index, right_nei)) {
            result += dist(array, size, target_index, left_nei);
            --left_nei;
        } else {
            result += dist(array, size, target_index, right_nei);
            ++right_nei;
        }
    }
    return result;
}

std::vector<long long int> trivial_solution(int* index_array, int* heap, int size, int k) {
    int* answer_array = new int[size];
    for (int i = 0; i < size; ++i) {
        answer_array[index_array[i]] = f(heap, size, k, i);
    }

    std::vector<long long int> result;
    result.reserve(size);
    for (int i = 0; i < size; ++i) {
        result.push_back(answer_array[i]);
    }
    return result;
}

//////////////////////////////////////////////////////////////////

void stress_testing() {
    std::random_device dev;
    std::mt19937 generator(dev());
    std::uniform_int_distribution<int> first_distribution(1, 1000);
    int test_number = 1000;

    long long int n = 1001;
    long long int k;

    bool ex_status = true;
    for (int l = 0; l < test_number; ++l) {
        k = first_distribution(generator);
        int* array = new int[n];
        for (int i = 0; i < n; ++i) {
            array[i] = first_distribution(generator);
        }

        int* index_array = new int[n];
        for (int i = 0; i < n; ++i) {
            index_array[i] = i;
        }

        std::vector<std::pair<long long int, long long int>> values;
        values.resize(n);
        for (int i = 0; i < n; ++i) {
            values[i] = {array[i], i};
        }

        HeapSort(index_array, array, n);
        std::vector<long long int> correct_answers = trivial_solution(index_array, array, n, k);


        Solution solution(values, k);
        std::vector<long long int> my_answers = solution.get_answer();


        bool success = true;
        for (int i = 0; i < n; ++i) {
            if (my_answers[i] != correct_answers[i]) {
                std::cout << "Error on test " << l + 1 << '\n';
                std::cout << "my answer[" << i << "] = " << my_answers[i] << " ; correct_answer[" << i << "] = " << correct_answers[i] << '\n';
                success = false;
                ex_status = false;
                break;
            }
        }
        if (success) {
            std::cout << "test number:  " << l + 1 << ": OK\n";
        }
    }
    if (ex_status) {
        std::cout << "All tests completed\n";
    }
}


int main() {
    stress_testing();
    return 0;
}
