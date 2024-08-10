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


class Segment {
private:
    int start;
    int finish;
public:
    Segment(int start, int finish) : start(start), finish(finish) {}

    int get_start() const {
        return start;
    }

    int get_finish() const {
        return finish;
    }

    bool operator==(const Segment& other) const {
        return start == other.start && finish == other.finish;
    }

    Segment(const Segment& other) = default;

    friend std::ostream& operator<<(std::ostream& out, const Segment& segment) {
        out << segment.start << ' ' << segment.finish << std::endl;
        return out;
    }
};


class Comparator_seg_start {
public:
    bool less(const Segment& first, const Segment& second) {
        return first.get_start() < second.get_start();
    }

    bool more(const Segment& first, const Segment& second) {
        return first.get_start() > second.get_start();
    }
};


class Comparator_seg_finish {
public:
    bool less(const Segment& first, const Segment& second) {
        if (first.get_finish() == second.get_finish()) {
            return first.get_start() < second.get_start();
        } else {
            return first.get_finish() < second.get_finish();
        }
    }

    bool more(const Segment& first, const Segment& second) {
        return !less(first, second) && !(first == second);
    }
};



class Solution {
public:
    Solution() = default;

    int get_answer(const std::vector<Segment>& v_start, const std::vector<Segment>& v_finish) const {
        int size = v_start.size();
        int count = 0;

        //processing the first segment
        if (v_start[0] == v_finish[0] && v_finish[1].get_finish() > v_finish[0].get_finish()) {
            ++count;
        }

        int tmp_max_start = v_finish[0].get_start();
        int tmp_max_finish = v_start[0].get_finish();

        //processing two types of intersections (see picture)
        for (int i = 1; i < size; ++i) {
            if (v_start[i] == v_finish[i]) {
                int tmp_finish = v_finish[i].get_finish();
                int tmp_start = v_start[i].get_start();
                bool type_1 = tmp_finish > tmp_max_finish && (i == size - 1 || tmp_finish < v_finish[i + 1].get_finish());
                bool type_2 = tmp_start > tmp_max_start && (i == size - 1 || tmp_start < v_start[i + 1].get_start());
                if (type_1 && type_2) {
                    ++count;
                }
            }

            tmp_max_finish = (v_start[i].get_finish() > tmp_max_finish) ? v_start[i].get_finish() : tmp_max_finish;
            tmp_max_start = (v_finish[i].get_start() > tmp_max_start) ? v_finish[i].get_start() : tmp_max_start;
        }
        return count;
    }
};


int main() {
    int segment_number;
    std::cin >> segment_number;

    std::vector<Segment> v;
    for (int i = 0; i < segment_number; ++i) {
        int start;
        int finish;
        std::cin >> start >> finish;
        v.reserve(segment_number);
        v.push_back({start, finish});
    }

    Pyramid<Segment, Comparator_seg_start> pyramid_start(v);
    Pyramid<Segment, Comparator_seg_finish> pyramid_finish(v);

    pyramid_start.pyramid_sort();
    pyramid_finish.pyramid_sort();

    Solution solution;
    std::cout << solution.get_answer(pyramid_start.get_vector(), pyramid_finish.get_vector()) << std::endl;
    return 0;
}
