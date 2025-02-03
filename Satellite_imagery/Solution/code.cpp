#include <iostream>
#include <map>
// решение через map.lower/upper_bound + map.erase(Itarator_begin, Iterator_end)

int main() {

    std::map<int, char> map;
    map.insert({4, 'a'});
    map.insert({5, 'a'});
    map.insert({9, 'a'});
    map.insert({2, 'a'});
    map.insert({7, 'a'});
    map.insert({3, 'a'});
    map.insert({1, 'a'});
    map.insert({13, 'a'});
    for (const auto& pair : map) {
        std::cout << pair.first << ' ';
    }
    std::cout << '\n';
    auto it = map.insert({8, 'a'});
    std::cout << it.first->first << '\n';
    ++it.first;
    std::cout << it.first->first << '\n';
    ----it.first;
    std::cout << it.first->first << '\n';

    std::cout << '\n';
    std::map<int, char>::iterator it_in = map.begin();
    while (it_in->first < 3) {
        ++it_in;
    }

    std::map<int, char>::iterator it_out = map.begin();
    while (it_out->first < 7) {
        ++it_out;
    }
    map.erase(it_in, it_out);
    for (const auto& pair : map) {
        std::cout << pair.first << ' ';
    }
    return 0;
}
