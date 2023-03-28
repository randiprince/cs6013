#include <iostream>
#include <vector>
#include <chrono>
#include "MyMalloc.hpp"



void tests() {
    MyMalloc testMalloc = MyMalloc();
    int size = testMalloc.myHashTable.tableSize;
    std::cout << size << std::endl;
    std::cout << testMalloc.myHashTable.tableSize << std::endl;
    std::cout << testMalloc.myHashTable.capacity << std::endl;
    std::cout << testMalloc.myHashTable.myHashTableStart << std::endl;
    assert(testMalloc.myHashTable.tableSize == 0);
    assert(testMalloc.myHashTable.capacity == 10);
    assert(testMalloc.myHashTable.myHashTableStart == nullptr);
};

void benchmarkTests() {
    MyMalloc mallocTest = MyMalloc();
    auto realMallocStart = std::chrono::high_resolution_clock::now();
    malloc(1000);
    auto realMallocEnd = std::chrono::high_resolution_clock::now();

    auto myMallocStart = std::chrono::high_resolution_clock::now();
    mallocTest.allocate(1000);
    auto myMallocEnd = std::chrono::high_resolution_clock::now();

    long myTime = std::chrono::duration_cast<std::chrono::microseconds>(myMallocEnd-myMallocStart).count();
    long realTime = std::chrono::duration_cast<std::chrono::nanoseconds>(realMallocEnd-realMallocStart).count();

    std::cout << "my malloc time was: " << myTime << std::endl;
    std::cout << "real malloc time was: " << realTime << std::endl;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    benchmarkTests();
   tests();

    return 0;
}
