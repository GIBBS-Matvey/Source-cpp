#include <iostream>
#include <optional>
#include <vector>
#include <random>

const int kPrimeNumber = 2000000011;
const size_t kMaxTimes = 4;



class HashFunction {
private:
    int64_t first_parameter_, second_parameter_, prime_number_, numberOfValues_;

public:
    HashFunction() = default;
    explicit HashFunction(int64_t first, int64_t second, int64_t prime, int64_t number) : first_parameter_(first),
                                                                                 second_parameter_(second),
                                                                                 prime_number_(prime),
                                                                                 numberOfValues_(number) {}
    int64_t GetHashValue(const int value) const {
        return ((first_parameter_ * value + second_parameter_) % prime_number_) % numberOfValues_;
    }
};


HashFunction GenerateHashFunction(int64_t numberOfValues);

std::vector<int> CountDistributionByBuckets(const std::vector<int>& numbers, size_t buckets_size,
                                            HashFunction hashFunction);

int64_t CountSquaresOfBucketsSizes(std::vector<int>&& distributeBucket);

bool CheckHashFunction(const std::vector<int>& numbers, size_t numberOfSquares);

bool CheckCollisions(const std::vector<int>& distributeBucket);




class HashTable {
private:
    HashFunction hashFunction_;
    std::vector<std::optional<int>> buckets_;

public:
    HashTable() = default;

    void Initialize(const std::vector<int>& numbers) {

        for (const auto& value : numbers) {
            std::cout << value << ' ';
        }
        std::cout << std::endl;

        std::cout << "STAGE_0" << std::endl;

        buckets_.resize(numbers.size() * numbers.size());

        std::cout << "STAGE_1" << std::endl;

        do {
            hashFunction_ = GenerateHashFunction(numbers.size());

            std::cout << "STAGE_2" << std::endl;

        } while(CheckCollisions(CountDistributionByBuckets(numbers, numbers.size(),
                                                           hashFunction_)));

        std::cout << "STAGE_3" << std::endl;

        for (const auto& value: numbers) {
            std::cout << hashFunction_.GetHashValue(value) << " 's place" << std::endl;
            buckets_[hashFunction_.GetHashValue(value)] = value;
            std::cout << "STAGE_3.5" << std::endl;
        }
        std::cout << "STAGE_4" << std::endl;
    }



    bool Contains(const int target) const {
        return buckets_[hashFunction_.GetHashValue(target)].has_value();
    }
};


class FixedSet {
private:
    std::vector<HashTable> buckets_;
    HashFunction hash_function_;

public:
    void Initialize(const std::vector<int>& numbers) {
        buckets_.resize(numbers.size());

        do {
            hash_function_ = GenerateHashFunction(numbers.size());
        } while(CheckHashFunction(numbers, CountSquaresOfBucketsSizes(
                CountDistributionByBuckets(numbers, numbers.size(),hash_function_))));

        std::cout << "next stage" << std::endl;             ///CHECKING

        std::vector<int> distributeBuffer(std::move(CountDistributionByBuckets(numbers, numbers.size(),
                                                                               hash_function_)));
        std::vector<std::vector<int>> valuesBuffer(numbers.size());

        std::cout << "next stage_1" << std::endl;             ///CHECKING

        for (size_t i = 0; i < numbers.size(); ++i) {
            valuesBuffer.reserve(distributeBuffer[i]);
        }

        std::cout << "next stage_2" << std::endl;             ///CHECKING

        for (const int& number : numbers) {
            valuesBuffer[hash_function_.GetHashValue(number)].push_back(number);
        }

        for (const auto& bucket : valuesBuffer) {
            for (const auto& value : bucket) {
                std::cout << value << ' ';
            }
            std::cout << std::endl;
        }

        std::cout << "next stage_3" << std::endl;             ///CHECKING

        for (size_t i = 0; i < valuesBuffer.size(); ++i) {
            buckets_[i].Initialize(valuesBuffer[i]);
        }

        std::cout << "next stage_4" << std::endl;             ///CHECKING
    }


    bool Contains(const int target) const {
        return buckets_[hash_function_.GetHashValue(target)].Contains(target);
    }
};


HashFunction GenerateHashFunction(int64_t numberOfValues) {
    std::random_device dev;
    std::mt19937 generator(dev());
    std::uniform_int_distribution<int> first_distribution(1, kPrimeNumber - 1);
    int first_parameter = first_distribution(generator);
    std::uniform_int_distribution<int> second_distribution(0, kPrimeNumber - 1);
    int second_parameter = second_distribution(generator);
    return HashFunction(first_parameter, second_parameter, kPrimeNumber, numberOfValues);
}


int64_t CountSquaresOfBucketsSizes(std::vector<int>&& distributeBucket) {
    int64_t result = 0;
    for (const auto& value : distributeBucket) {
        result += value * value;
    }
    std::cout << result << std::endl;           ///CHECKING
    return result;
}

bool CheckHashFunction(const std::vector<int>& numbers, const size_t numberOfSquares) {
    return numberOfSquares >= kMaxTimes * numbers.size();
}


std::vector<int> CountDistributionByBuckets(const std::vector<int>& numbers, size_t buckets_size,
                                            HashFunction hashFunction) {
    std::vector<int> result(buckets_size);
    for (const auto& value : numbers) {
        ++result[hashFunction.GetHashValue(value)];
    }

    for (const auto& value : result) {             ///CHECKING
        std::cout << value << ' ';
    }
    std::cout << std::endl;                        ///

    return result;
}

bool CheckCollisions(const std::vector<int>& distributeBucket) {
    for (const auto& value : distributeBucket) {
        if (value > 1) {
            return true;
        }
    }
    return false;
}



int main() {
    FixedSet fixedSet;
    std::vector<int> buffer = {2, 13, 89, 55, 69, 34, 9, 21, 8, 91};
    fixedSet.Initialize(buffer);

    std::cout << "Ok" << std::endl;

    return 0;
}
