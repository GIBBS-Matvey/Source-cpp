#include <iostream>
#include <optional>
#include <vector>
#include <random>

// TODO Random Library

const size_t kPrimeNumber = 2000011;
const size_t kMaxTimes = 4;



class HashFunction {
private:
    size_t first_parameter_, second_parameter_, prime_number_, numberOfValues_;

public:
    HashFunction() = default;
    explicit HashFunction(size_t first, size_t second, size_t prime, size_t number) : first_parameter_(first),
                                                                                 second_parameter_(second),
                                                                                 prime_number_(prime),
                                                                                 numberOfValues_(number) {}
    size_t GetHashValue(const int value) const {
        return ((first_parameter_ * value + second_parameter_) % prime_number_) % numberOfValues_;
    }
};


HashFunction GenerateHashFunction(size_t numberOfValues);

std::vector<int> CountDistributionByBuckets(const std::vector<int>& numbers, size_t buckets_size,
                                            HashFunction hashFunction);

size_t CountSquaresOfBucketsSizes(std::vector<int>&& distributeBucket);

bool CheckHashFunction(const std::vector<int>& numbers, size_t numberOfSquares);

bool CheckCollisions(const std::vector<int>& distributeBucket);




class HashTable {
private:
    HashFunction hashFunction_;
    std::vector<std::optional<int>> buckets_;

public:
    HashTable() = default;

    void Initialize(const std::vector<int>& numbers) {
        buckets_.resize(numbers.size() * numbers.size());
        do {
            hashFunction_ = GenerateHashFunction(numbers.size());
        } while(CheckCollisions(CountDistributionByBuckets(numbers, numbers.size(),
                                                           hashFunction_)));
        for (const auto& value: numbers) {
            buckets_[hashFunction_.GetHashValue(value)] = value;
        }
    }

    bool Contains(const int target) const {
        return !buckets_.empty() && (buckets_[hashFunction_.GetHashValue(target)].has_value() &&
                                    buckets_[hashFunction_.GetHashValue(target)] == target);
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

        std::vector<int> distributeBuffer(std::move(CountDistributionByBuckets(numbers, numbers.size(),
                                                                               hash_function_)));
        std::vector<std::vector<int>> valuesBuffer(numbers.size());

        for (size_t i = 0; i < numbers.size(); ++i) {
            valuesBuffer.reserve(distributeBuffer[i]);
        }

        for (const int& number : numbers) {
            valuesBuffer[hash_function_.GetHashValue(number)].push_back(number);
        }

        for (size_t i = 0; i < valuesBuffer.size(); ++i) {
            buckets_[i].Initialize(valuesBuffer[i]);
        }
    }

    bool Contains(const int target) const {
        return buckets_[hash_function_.GetHashValue(target)].Contains(target);
    }
};


HashFunction GenerateHashFunction(size_t numberOfValues) {
    std::random_device dev;
    std::mt19937 generator(dev()); // TODO
    std::uniform_int_distribution<size_t> first_distribution(1, kPrimeNumber - 1);
    size_t first_parameter = first_distribution(generator);
    std::uniform_int_distribution<size_t> second_distribution(0, kPrimeNumber - 1);
    size_t second_parameter = second_distribution(generator);
    return HashFunction(first_parameter, second_parameter, kPrimeNumber, numberOfValues);
}


size_t CountSquaresOfBucketsSizes(std::vector<int>&& distributeBucket) {
    size_t result = 0;
    for (const auto& value : distributeBucket) {
        result += value * value;
    }
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

    std::vector<int> checking_buffer = {2, 40, 13, 67, 8, 1, 55, 6};

    for (const auto& value : checking_buffer) {
        std::cout << fixedSet.Contains(value) << ' ';
    }

    return 0;
}
