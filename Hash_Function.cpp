#include <iostream>
#include <optional>
#include <vector>
#include <random>

const int kPrimeNumber = 2000000011;
const int kMaxTimes = 4;



class HashFunction {
private:
    int64_t first_parameter_, second_parameter_, prime_number_, numberOfValues_;

public:
    HashFunction() = default;
    HashFunction(int64_t first, int64_t second, int64_t prime, int64_t number) : first_parameter_(first),
                                                                                 second_parameter_(second),
                                                                                 prime_number_(prime),
                                                                                 numberOfValues_(number){}
    int64_t GetHashValue(const int value) const {
        return ((first_parameter_ * value + second_parameter_) % prime_number_) % numberOfValues_;
    }
};


HashFunction GenerateHashFunction(std::mt19937 *generator, int64_t numberOfValues);

std::vector<int> CountDistributionByBuckets(const std::vector<int>& numbers, size_t buckets_size,
                                            HashFunction hashFunction);

int64_t CountSquaresOfBucketsSizes(std::vector<int>&& distributeBucket);

bool CheckHashFunction(const std::vector<int>& numbers, int64_t numberOfSquares);

bool CheckCollisions(const std::vector<int>& distributeBucket);




class HashTable {
private:
    HashFunction hashFunction_;
    std::vector<int> buckets_;

public:
    HashTable() = default;

    void Initialize(const std::vector<int>& numbers) {
        buckets_.reserve(numbers.size() * numbers.size());
        std::random_device dev;
        std::mt19937 generator(dev());
        do {
            hashFunction_ = GenerateHashFunction(&generator, numbers.size());
        } while(CheckCollisions(CountDistributionByBuckets(numbers, buckets_.size(),
                                                           hashFunction_)));
        for (const auto& value: numbers) {
            buckets_[hashFunction_.GetHashValue(value)] = value;
        }
    }

    void PrintHashTable() const {
        for (const auto& value :  buckets_) {
            std::cout << value << ' ';
        }
    }



    bool Contains(const int number) const {
        // TODO
    }
};


class FixedSet {
private:
    std::vector<HashTable> buckets_;
    HashFunction hash_function_;

public:
    void Initialize(const std::vector<int>& numbers) {
        buckets_.reserve(numbers.size());
        std::random_device dev;
        std::mt19937 generator(dev());

        do {
            hash_function_ = GenerateHashFunction(&generator, numbers.size());
        } while(CheckHashFunction(numbers, CountSquaresOfBucketsSizes(
                CountDistributionByBuckets(numbers, numbers.size(),hash_function_))));

        std::vector<int> distributeBuffer(std::move(CountDistributionByBuckets(numbers, numbers.size(),
                                                                               hash_function_)));
        std::vector<std::vector<int>> valuesBuffer(numbers.size());
        
        for (const auto& value : numbers) {
            valuesBuffer[hash_function_.GetHashValue(value)].push_back(value);
        }

        for (size_t i = 0; i < valuesBuffer.size(); ++i) {
            buckets_[i].Initialize(valuesBuffer[i]);
        }

    }

    void PrintFixedSet() {
        for (const auto& bucket : buckets_) {
            bucket.PrintHashTable();
            std::cout << std::endl;
        }
    }

    bool Contains(int number) const {
        // TODO
    }
};


HashFunction GenerateHashFunction(std::mt19937 *generator, int64_t numberOfValues) {
    std::uniform_int_distribution<int> first_distribution(1, kPrimeNumber - 1);
    int first_parameter = first_distribution(*generator);
    std::uniform_int_distribution<int> second_distribution(0, kPrimeNumber - 1);
    int second_parameter = second_distribution(*generator);
    return {first_parameter, second_parameter, kPrimeNumber, numberOfValues};
}


int64_t CountSquaresOfBucketsSizes(std::vector<int>&& distributeBucket) {
    int64_t result = 0;
    for (const auto& value : distributeBucket) {
        result += value * value;
    }
    return result;
}

bool CheckHashFunction(const std::vector<int>& numbers, const int64_t numberOfSquares) {
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
            return false;
        }
    }
    return true;
}



int main() {
    FixedSet fixedSet;
    std::vector<int> buffer = {2, 13, 89, 55, 69, 34, 9, 21, 8, 91};
    fixedSet.Initialize(buffer);

    fixedSet.PrintFixedSet();
    return 0;
}
