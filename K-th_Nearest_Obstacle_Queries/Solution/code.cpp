#include <iostream>
#include <vector>
#include <fstream>

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

    Pyramid();

    Pyramid(const std::vector<T>& v);

    void reserve(int new_capacity);

    void build_pyramid();

    const std::vector<T>& get_vector() const;

    void print() const;

    int get_size() const;

    void extract_root();

    const T& get_root() const;

    void push(const T& new_value);
};


template <typename T, typename Comparator>
Pyramid<T, Comparator>::Pyramid() : size(0) {}


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
const std::vector<T>& Pyramid<T, Comparator>::get_vector() const {
    return array;
}


template <typename T, typename Comparator>
void Pyramid<T, Comparator>::print() const {
    std::cout << "pyramid: ";
    for (int i = 0; i < size; ++i) {
        std::cout << array[i] << ' ';
    }
    std::cout << '\n';
}


template <typename T, typename Comparator>
void Pyramid<T, Comparator>::extract_root() {
    std::swap(array[0], array[size - 1]);
    array.pop_back();
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


template<typename T, typename Comparator>
void Pyramid<T, Comparator>::push(const T& new_value) {
    array.push_back(new_value);
    sift_up(size);
    ++size;
}


template<typename T, typename Comparator>
void Pyramid<T, Comparator>::reserve(int new_capacity) {
    array.reserve(new_capacity);
}


class Comparator_int {
    public:
        bool less(int first, int second) const {
            return first < second;
        }

        bool more(int first, int second) const {
            return first > second;
        }
    };



class Solution {
public:
    std::vector<int> resultsArray(std::vector<std::vector<int>>& queries, int k) {

        std::vector<int> result;
        result.reserve(queries.size());

        Pyramid<int, Comparator_int> pyramid;
        pyramid.reserve(k);

        for (int i = 0; i < queries.size(); ++i) {

            int value = abs(queries[i][0]) + abs(queries[i][1]);

            if (i + 1 <= k) {
                pyramid.push(value);
                int new_result = (i + 1 == k) ? pyramid.get_root() : -1;
                result.push_back(new_result);
            } else {
                if (value < pyramid.get_root()) {
                    pyramid.extract_root();
                    pyramid.push(value);
                }
                result.push_back(pyramid.get_root());
            }
        }

        return result;
    }
};



int main() {
    int k, n;
    std::cin >> k >> n;
    std::vector<std::vector<int>> queries;
    queries.resize(n, std::vector<int>(2));
    for (int i = 0; i < n; ++i) {
        int first, second;
        std::cin >> first >> second;
        queries[i][0] = first;
        queries[i][1] = second;
    }
    Solution solution;
    std::vector<int> result = solution.resultsArray(queries, k);
    for (int val : result) {
        std::cout << val << ' ';
    }
    return 0;
}
