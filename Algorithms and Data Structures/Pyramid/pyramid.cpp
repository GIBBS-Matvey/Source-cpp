#include <iostream>
#include <vector>
#include "pyramid.h"


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
