#pragma once

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
};
