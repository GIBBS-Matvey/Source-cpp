#include <iostream>
#include <vector>


template <typename T, typename Comparator>
class Pyramid {
private:
    std::vector<T> array;
    Comparator comparator;
    int size;

    int left_child(int index);

    int right_child(int index);

    int parent(int index);

    bool has_two_children(int index);

    bool has_the_only_child(int index);

    bool has_children(int index);

    void sift_down(int start_index);

    void sift_up(int index);

public:
    Pyramid(const std::vector<T>& v);

    void build_pyramid();

    void pyramid_sort();

    const std::vector<T>& get_vector() const;

    void print() const;

    int get_size() const;

    void extract_root();

    const T& get_root() const;
};


template <typename T, typename Comparator>
int Pyramid<T, Comparator>::left_child(int index) {
    return 2 * index + 1;
}


template <typename T, typename Comparator>
int Pyramid<T, Comparator>::right_child(int index) {
    return 2 * index + 2;
}


template <typename T, typename Comparator>
int Pyramid<T, Comparator>::parent(int index) {
    return (index - 1) / 2;
}


template <typename T, typename Comparator>
bool Pyramid<T, Comparator>::has_two_children(int index) {
    return right_child(index) < size;
}


template <typename T, typename Comparator>
bool Pyramid<T, Comparator>::has_the_only_child(int index) {
    return left_child(index) == size - 1;
}


template <typename T, typename Comparator>
bool Pyramid<T, Comparator>::has_children(int index) {
    return left_child(index) < size;
}


template <typename T, typename Comparator>
void Pyramid<T, Comparator>::sift_down(int start_index) {
    int index = start_index;
    bool swapped = true;
    while (has_children(index) && swapped) {
        if (has_two_children(index)) {
            int max_child_index = (comparator.more(array[left_child(index)], array[right_child(index)])) ? left_child(index) : right_child(index);
            if (comparator.less(array[index], array[max_child_index])) {
                std::swap(array[index], array[max_child_index]);
                index = max_child_index;
            } else {
                swapped = false;
            }
        }
        else if (has_the_only_child(index)) {
            if (comparator.less(array[index], array[left_child(index)])) {
                std::swap(array[index], array[left_child(index)]);
                index = left_child(index);
            } else {
                swapped = false;
            }
        }
    }
}


template <typename T, typename Comparator>
void Pyramid<T, Comparator>::sift_up(int index) {
    while (comparator.less(array[parent(index)], array[index])) {
        std::swap(array[parent(index)], array[index]);
        index = parent(index);
    }
}


template <typename T, typename Comparator>
Pyramid<T, Comparator>::Pyramid(const std::vector<T>& v) : array(v), size(v.size()), comparator() {
    build_pyramid();
}


template <typename T, typename Comparator>
void Pyramid<T, Comparator>::build_pyramid() {
    for (int i = 1; i < size; ++i) {
        sift_up(i);
    }
}


template <typename T, typename Comparator>
void Pyramid<T, Comparator>::pyramid_sort() {
    for (int i = 0; i < array.size(); ++i) {
        std::swap(array[size - 1], array[0]);
        --size;
        sift_down(0);
    }
    size = array.size();
}


template <typename T, typename Comparator>
const std::vector<T>& Pyramid<T, Comparator>::get_vector() const {
    return array;
}


template <typename T, typename Comparator>
void Pyramid<T, Comparator>::print() const {
    for (const T& val : array) {
        std::cout << val;
    }
}


template <typename T, typename Comparator>
void Pyramid<T, Comparator>::extract_root() {
    std::swap(array[0], array[size - 1]);
    --size;
    sift_down(0);
}


template <typename T, typename Comparator>
const T& Pyramid<T, Comparator>::get_root() const {
    return array[0];
}


template <typename T, typename Comparator>
int Pyramid<T, Comparator>::get_size() const {
    return size;
}


class Comparator_int {
public:
    bool less(int first, int second) const {
        return first > second;
    }

    bool more(int first, int second) const {
        return first < second;
    }
};


class Solution {
public:
    int get_answer(const std::vector<int>& v_start, const std::vector<int>& v_finish) {
        Pyramid<int, Comparator_int> pyramid_start(v_start);
        Pyramid<int, Comparator_int> pyramid_finish(v_finish);
        int free_workers = 0;
        int min_val = 0;
        while (pyramid_start.get_size() > 0) {
            if (pyramid_start.get_root() < pyramid_finish.get_root()) {
                pyramid_start.extract_root();
                --free_workers;
                min_val -= (free_workers < min_val) ? 1 : 0;
            } else {
                pyramid_finish.extract_root();
                ++free_workers;
            }
        }
        return abs(min_val);
    }
};


int main() {
    int n;
    std::cin >> n;
    std::vector<int> v_start;
    std::vector<int> v_finish;
    v_start.reserve(n);
    v_finish.reserve(n);
    for (int i = 0; i < n; ++i) {
        int start, process_time;
        std::cin >> start >> process_time;
        v_start.push_back(start);
        v_finish.push_back(start + process_time);
    }

    Solution solution;
    std::cout << solution.get_answer(v_start, v_finish);
    return 0;
}
