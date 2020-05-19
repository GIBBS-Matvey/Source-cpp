#include <iostream>
using namespace std;
int parent(int index) {
    return (index - 1) / 2;
}
int leftChild(int index) {
    return index * 2 + 1;
}
int rightChild(int index) {
    return index * 2 + 2;
}
void siftDown(int *heap, int size, int index);
void buildHeap(int *heap, const int &size);
void extractMin(int *heap, int &currentHeapSize);
int main() {
    int numberOfCargo, freeMachines = 1, result = 1;
    cin >> numberOfCargo;
    int arriveTimeHeapSize, exitTimeHeapSize;
    arriveTimeHeapSize = numberOfCargo;
    exitTimeHeapSize = numberOfCargo;
    int *arriveHeap = new int[numberOfCargo];
    int *exitHeap = new int[numberOfCargo];
    for (int i = 0; i < numberOfCargo; ++i) {
        int neededTime;
        cin >> arriveHeap[i] >> neededTime;
        exitHeap[i] = arriveHeap[i] + neededTime;
    }
    buildHeap(arriveHeap, numberOfCargo);
    buildHeap(exitHeap, numberOfCargo);
    while (arriveTimeHeapSize != 0) {
        if (arriveHeap[0] < exitHeap[0]) {
            if (freeMachines >= 1) {
                --freeMachines;
            }
            else {
                ++result;
            }
            extractMin(arriveHeap, arriveTimeHeapSize);
        }
        else {
            ++freeMachines;
            extractMin(exitHeap, exitTimeHeapSize);
        }
    }
    cout << result;
    delete []arriveHeap;
    delete []exitHeap;
    return 0;
}
void siftDown(int *heap, int size, int index) {
    while ((leftChild(index) <= size - 1 && heap[leftChild(index)] < heap[index]) ||
    (rightChild(index) <= size - 1 && heap[rightChild(index)] < heap[index])) {
        int indexMin = (rightChild(index) <= size - 1 && heap[rightChild(index)] < heap[leftChild(index)]) ? rightChild(index) : leftChild(index);
        swap(heap[indexMin], heap[index]);
        index = indexMin;
    }
}
void buildHeap(int *heap, const int &size) {
    for (int i = parent(size - 1); i >= 0; --i) {
        siftDown(heap, size, i);
    }
}
void extractMin(int *heap, int &currentHeapSize) {
    swap(heap[0], heap[currentHeapSize - 1]);
    --currentHeapSize;
    siftDown(heap, currentHeapSize, 0);
}
