#include <iostream>
#include <vector>
#include <list>


class Point {
private:
    int number;
    bool used;
public:
    Point(int number) : number(number), used(false) {}

    void change_status() {
        used = true;
    }

    bool is_used() const {
        return used;
    }

    int get_number() const {
        return number;
    }

    Point& operator=(const Point& other) = default;

    bool operator!=(const Point& other) const {
        return number != other.number;
    }
};


class Cycle {
private:
    int max_number;
    int min_number;
public:
    Cycle() : max_number(-1), min_number(INT32_MAX) {}

    void push_point(Point& point) {
        point.change_status();
        max_number = std::max(point.get_number(), max_number);
        min_number = std::min(point.get_number(), min_number);
    }

    int get_max() const {
        return max_number;
    }

    int get_min() const {
        return min_number;
    }
};


Cycle find_cycle(int start_index, std::vector<Point>& permutation) {
    Cycle cycle;

    int tmp_index = start_index;
    //std::cout << tmp_index << ' ';
    do {
        cycle.push_point(permutation[tmp_index]);
        tmp_index = permutation[tmp_index].get_number();
        //std::cout << tmp_index << ' ';
    }
    while (permutation[tmp_index].get_number() != permutation[start_index].get_number());
    return cycle;
}


class Location {
private:
    bool is_start;
    bool is_finish;
public:
    Location() : is_start(false), is_finish(false) {}

    void set_start() {
        is_start = true;
    }

    void set_finish() {
        is_finish = true;
    }

    bool has_start() const {
        return is_start;
    }

    bool has_finish() const {
        return is_finish;
    }
};



class Solution {
public:
    size_t get_answer(std::vector<Point>& permutation) {
        std::list<Cycle> list_cycle;
        int answer = 0;

        for (int i = 0; i < permutation.size(); ++i) { //O(n)
            if (!permutation[i].is_used()) {
                Cycle new_cycle = find_cycle(i, permutation);
                list_cycle.push_back(new_cycle);
                //std::cout << new_cycle.get_min() + 1 << ' ' << new_cycle.get_max() + 1 << std::endl; for checking
            }
        }

        std::vector<Location> v_loc;
        v_loc.resize(permutation.size());
        for (const auto& cycle : list_cycle) { //O(n)
            v_loc[cycle.get_min()].set_start();
            v_loc[cycle.get_max()].set_finish();
        }

        int number_cur_cycles = 0;
        for (int i = 0; i < permutation.size(); ++i) { //O(n)
            if (v_loc[i].has_start() && v_loc[i].has_finish() && number_cur_cycles == 0) {
                ++answer;
            }
            number_cur_cycles += (v_loc[i].has_start()) ? 1 : 0;
            number_cur_cycles -= (v_loc[i].has_finish()) ? 1 : 0;
        }
        return answer;
    }

};



int main() {
    int n;
    std::cin >> n;
    std::vector<Point> permutation;
    permutation.resize(n, Point(-1));
    for (int i = 0; i < n; ++i) {
        int start, finish;
        std::cin >> start >> finish;
        permutation[start - 1] = Point(finish - 1);
    }
    for (int i = 0; i < n; ++i) {
        std::cout << permutation[i].get_number() + 1 << ' ';
    }
    std::cout << std::endl;

    Solution solution;
    std::cout << solution.get_answer(permutation);
    return 0;
}
