#include <iostream>
#include <cmath>
#include <limits>
#include <vector>
using namespace std;

int parent(int index) {
    return (index - 1) / 2;
}

int leftChild(int index) {
    return 2 * index + 1;
}

int rightChild(int index) {
    return 2 * index + 2;
}

void siftDown(int* index_array, int *heap, int size, int index) {
    while ((leftChild(index) <= size - 1 && heap[leftChild(index)] < heap[index]) ||
    (rightChild(index) <= size - 1 && heap[rightChild(index)] < heap[index])) {
        int indexMin = (rightChild(index) <= size - 1 && heap[rightChild(index)] < heap[leftChild(index)]) ? rightChild(index) : leftChild(index);
        swap(heap[indexMin], heap[index]);
        swap(index_array[indexMin], index_array[index]);
        index = indexMin;
    }
}


void BuildHeap(int* index_array, int* array, const int& size) {
    for (int i = parent(size - 1); i >= 0; --i) {
        siftDown(index_array, array, size, i);
    }
}

void HeapSort(int* index_array, int* array, const int& size) {
    BuildHeap(index_array, array, size);
    int temp_size = size;
    for (int i = 0; i < size; ++i) {
        swap(array[0], array[temp_size - 1]);
        swap(index_array[0], index_array[temp_size - 1]);
        --temp_size;
        siftDown(index_array, array, temp_size, 0);
    }
}



bool is_valid_position(int i, int n) {
    return (i >= 0) && (i < n);
}

int dist(int* array, int n, int target, int nei) {
    if (is_valid_position(nei, n)) {
        return abs(array[target] - array[nei]);
    } else {
        return INT32_MAX;
    }
}

int f(int* array, int size, int k, int target_index) {
    int right_nei = target_index + 1;
    int left_nei = target_index - 1;
    int result = 0;
    for (int i = 0; i != k; ++i) {
        if (dist(array, size, target_index, left_nei) < dist(array, size, target_index, right_nei)) {
            result += dist(array, size, target_index, left_nei);
            --left_nei;
        } else {
            result += dist(array, size, target_index, right_nei);
            ++right_nei;
        }
    }
    return result;
}

void solution(int* index_array, int* heap, int size, int k) {
    int* answer_array = new int[size];
    for (int i = 0; i < size; ++i) {
        answer_array[index_array[i]] = f(heap, size, k, i);
    }
    for (int i = 0; i < size; ++i) {
        cout << answer_array[i] << ' ';
    }
}

int main() {

    int n, k;
    cin >> n >> k;

    int* array = new int[n];
    for (int i = 0; i < n; ++i) {
        cin >> array[i];
    }

    int* index_array = new int[n];
    for (int i = 0; i < n; ++i) {
        index_array[i] = i;
    }

    HeapSort(index_array, array, n);
    solution(index_array, array, n, k);


    return 0;
}
