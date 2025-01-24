#include <crtdbg.h>
#include <iostream>
#include <cstring>

using namespace std;


struct Element {
    int value;
    int addIndex;
};


struct Operation {
    char operationType[11];
};


int parent(int index) {
    return (index - 1) / 2;
}

int leftChild(int index) {
    return 2 * index + 1;
}

int rightChild(int index) {
    return 2 * index + 2;
}

void addElement(Element *heap, int *mass, int element);

void siftUp(Element *heap, int* mass, int index);

void siftDown(Element *heap, int *mass, int index);

void deleteElement(Element *heap, int *mass, int deleteIndex);

void changeElement(Element *heap, int *mass, int changeIndex, int newValue);

void extractMin(Element *heap, int *mass);

void getMin(Element *heap);

void size();

void clear(int *mass);

int addedIndex = 0;
int currPyramidSize = 0;
int numberOfOperation;


int main () {
    cin >> numberOfOperation;
    Element *heap = new Element[numberOfOperation];
    int *mass = new int[numberOfOperation];
    int count = 0;
    int newElement, deleteIndex, changeIndex;

    while (count < numberOfOperation) {
        Operation command;
        cin >> command.operationType;
        if (strcmp(command.operationType, "insert") == 0) {
            cin >> newElement;
            addElement(heap, mass, newElement);
        }
        else if (strcmp(command.operationType, "extract_min") == 0) {
            extractMin(heap, mass);
        }
        else if (strcmp(command.operationType, "delete") == 0) {
            cin >> deleteIndex;
            deleteElement(heap, mass, deleteIndex);
        }
        else if (strcmp(command.operationType, "change") == 0) {
            cin >> changeIndex;
            cin >> newElement;
            changeElement(heap, mass, changeIndex, newElement);
        }
        else if (strcmp(command.operationType, "get_min") == 0) {
            getMin(heap);
        }
        else if (strcmp(command.operationType, "size") == 0) {
            size();
        }
        else if (strcmp(command.operationType, "clear") == 0) {
            clear(mass);
        }
        ++count;
    }

    delete []mass;
    delete []heap;

    std::cout << "valgrind checking is "<< _CrtDumpMemoryLeaks();

}


void addElement(Element *heap, int *mass, int element) {
    ++addedIndex;
    ++currPyramidSize;
    heap[currPyramidSize - 1].value = element;
    heap[currPyramidSize - 1].addIndex = addedIndex - 1;
    mass[addedIndex - 1] = currPyramidSize - 1;
    siftUp(heap, mass, currPyramidSize - 1);
    cout << "ok" << endl;
}

void siftUp(Element *heap, int *mass, int index) {
    while (parent(index) >= 0 && heap[index].value < heap[parent(index)].value) {
        swap(heap[index], heap[parent(index)]);
        swap(mass[heap[index].addIndex], mass[heap[parent(index)].addIndex]);
        index = parent(index);
    }
}

void siftDown(Element *heap, int *mass, int index) {
    while ((leftChild(index) <= currPyramidSize - 1 && heap[leftChild(index)].value < heap[index].value) ||
           (rightChild(index) <= currPyramidSize - 1 && heap[rightChild(index)].value < heap[index].value)) {
        int indexMin = (heap[rightChild(index)].value < heap[leftChild(index)].value && rightChild(index) <=
                                                                                        currPyramidSize - 1) ? rightChild(index) : leftChild(index);
        swap(heap[index], heap[indexMin]);
        swap(mass[heap[index].addIndex], mass[heap[indexMin].addIndex]);
        index = indexMin;
    }
}

void deleteElement(Element *heap, int *mass, int deleteIndex) {
    if (currPyramidSize >= 1 && mass[deleteIndex - 1] <= currPyramidSize - 1 && deleteIndex <= addedIndex) {
        int heapRemIndex = mass[deleteIndex - 1];
        mass[heap[currPyramidSize - 1].addIndex] = mass[deleteIndex - 1];
        mass[deleteIndex - 1] = numberOfOperation;
        swap(heap[heapRemIndex], heap[currPyramidSize - 1]);
        --currPyramidSize;
        siftUp(heap, mass, heapRemIndex);
        siftDown(heap, mass, heapRemIndex);
        cout << "ok" << endl;
    }
    else {
        cout << "error" << endl;
    }
}

void changeElement(Element *heap, int *mass, int changeIndex, int newValue) {
    if (currPyramidSize >= 1 && mass[changeIndex - 1] <= currPyramidSize - 1 && changeIndex <= addedIndex) {
        heap[mass[changeIndex - 1]].value = newValue;
        siftUp(heap, mass, mass[changeIndex - 1]);
        siftDown(heap, mass, mass[changeIndex - 1]);
        cout << "ok" << endl;
    }
    else {
        cout << "error" << endl;
    }
}

void extractMin(Element *heap, int *mass) {
    if (currPyramidSize >= 1) {
        cout << heap[0].value << endl;
        swap(heap[0], heap[currPyramidSize - 1]);
        mass[heap[0].addIndex] = mass[heap[currPyramidSize - 1].addIndex];
        mass[heap[currPyramidSize - 1].addIndex] = numberOfOperation;
        --currPyramidSize;
        siftDown(heap, mass, 0);
    }
    else {
        cout << "error" << endl;
    }
}

void getMin(Element *heap) {
    if (currPyramidSize >= 1) {
        cout << heap[0].value << endl;
    }
    else {
        cout << "error" << endl;
    }
}

void size() {
    cout << currPyramidSize << endl;
}

void clear(int *mass) {
    currPyramidSize = 0;
    for (int i = 0; i < addedIndex; ++i) {
        mass[i] = numberOfOperation;
    }
    cout << "ok" << endl;
}
