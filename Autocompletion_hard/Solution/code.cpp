#include <iostream>
#include <vector>
#include <algorithm>



class Solution {
private:
    struct Word {
        std::string word;
        int popularity;
        int index;

        Word(const std::string& word, int popularity, int index) : word(word), popularity(popularity), index(index) {}
    };

    static bool compare(const Word& w1, const Word& w2) {
        return w1.word < w2.word;
    }

    std::vector<Word> v;

public:

    Solution(std::vector<std::string>& v_words, std::vector<int>& v_popular) {
        v.reserve(v_words.size());
        for (int i = 0; i < v_words.size(); ++i) {
            v.push_back({v_words[i], v_popular[i], i});
        }

        std::make_heap(v.begin(), v.end(), compare);
        std::sort_heap(v.begin(), v.end(), compare);


    }

    void print() const {
        for (auto& word : v) {
            std::cout << word.word << ", " << word.popularity << ", " << word.index << '\n';
        }
    }

    int process_request(const std::string& word) {
        return 0;
    }
};


int main() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<std::string> v_words;
    std::vector<int> v_popular;

    v_words.resize(N);
    v_popular.resize(N);

    for (int i = 0; i < N; ++i) {
        std::cin >> v_words[i] >> v_popular[i];
    }

    Solution solution(v_words, v_popular);
    solution.print();

    /*char symbol, operation;
    std::string cur_word;

    for (int i = 0; i < Q; ++i) {
        std::cin >> operation;
        if (operation == '+') {
            std::cin >> symbol;
            cur_word += symbol;
        } else {
            cur_word.pop_back();
        }
        solution.process_request(cur_word);
    }*/


    return 0;
}
