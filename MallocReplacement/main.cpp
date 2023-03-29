#include <iostream>
#include <vector>
#include <chrono>
#include "MyMalloc.hpp"



void tests() {
    //initialzie malloc
    MyMalloc testMalloc = MyMalloc();
    std::cout << "CONFIRM VALUES AFTER MALLOC INITIALIZATION:" << std::endl;
    std::cout << "table size should be 0: " << testMalloc.myHashTable.tableSize << std::endl;
    std::cout << "capacity should be 10: " << testMalloc.myHashTable.capacity << std::endl;
    std::cout << "address should not be null: " << testMalloc.myHashTable.myHashTableStart << std::endl << std::endl;

    //confirm with asserts
    assert(testMalloc.myHashTable.tableSize == 0);
    assert(testMalloc.myHashTable.capacity == 10);
    assert(testMalloc.myHashTable.myHashTableStart != nullptr);

    // test insertion and deletion of one byte
    void* ptrOfAdded = testMalloc.allocate(1);
    size_t indexOfAdded = testMalloc.myHashTable.hashFunctor(ptrOfAdded);
    std::cout << "CONFIRM VALUES AFTER ONE BYTE ALLOCATED:" << std::endl;
    std::cout << "ptr address of allocated byte: " << ptrOfAdded << std::endl;
    std::cout << "ptr address stored in the table: " << testMalloc.myHashTable.myHashTableStart[indexOfAdded].ptr << std::endl;
    std::cout << "table size should be 1: " << testMalloc.myHashTable.tableSize << std::endl << std::endl;

    // now deallocate!
    testMalloc.deallocate(ptrOfAdded);
    std::cout << "CONFIRM VALUES AFTER ONE BYTE DELLOCATED:" << std::endl;
    std::cout << "ptr address should now indicate lazy delete: " << testMalloc.myHashTable.myHashTableStart[indexOfAdded].ptr << std::endl;
    std::cout << "table size should be 0: " << testMalloc.myHashTable.tableSize << std::endl << std::endl;

    // allocate a bunch of stuff and print the hash table
    void* ptrsOfAllocated[3000];
    for (int i = 0; i < 3000; i++) {
        ptrsOfAllocated[i] = nullptr;
    }
    std::cout << "ALLOCATING 1000 BYTES ONE AT A TIME:" << std::endl;
    for (int i = 0; i < 1000; i++) {
        ptrsOfAllocated[i] = testMalloc.allocate(i+1);
    }
    std::cout << "table size should be 1000: " << testMalloc.myHashTable.tableSize << std::endl << std::endl;
    //now allocate more stuff
    std::cout << "NOW ALLOCATING 2000 MORE BYTES ONE AT A TIME:" << std::endl;

    for (int i = 1000; i < 3000; i++) {
        ptrsOfAllocated[i] = testMalloc.allocate(i);
    }
    std::cout << "table size should be 3000: " << testMalloc.myHashTable.tableSize << std::endl << std::endl;
    // now test partial deallocation
    std::cout << "NOW DEALLOCATING 333 BYTES ONE AT A TIME:" << std::endl;
    for (int i = 0; i < 333; i++) {
        testMalloc.deallocate(ptrsOfAllocated[i]);
    }
    std::cout << "table size should be 2667: " << testMalloc.myHashTable.tableSize << std::endl << std::endl;

    std::cout << "NOW DEALLOCATING REMAINING BYTES ONE AT A TIME:" << std::endl;
    for (int i = 333; i < 3000; i++) {
        testMalloc.deallocate(ptrsOfAllocated[i]);
    }
    std::cout << "table size should be 0: " << testMalloc.myHashTable.tableSize << std::endl << std::endl;

    std::cout << "NOW TRYING TO DEALLOCATE SAME 2667 BYTES (SHOULD GET ERROR):" << std::endl;
    std::cout << "ERROR SHOULD SAY: 'error! passed node was not removed!': " << std::endl;
    try {
        for (int i = 333; i < 3000; i++) {
            testMalloc.deallocate(ptrsOfAllocated[i]);
        }
    } catch (std::runtime_error e) {
        std::cout << "ACTUAL ERROR THROWN IS: " << e.what() << std::endl << std::endl;
    }

    std::cout << "TEST ADDING SAME THING TWICE" << std::endl;

    HashNode nodeToInsert = HashNode(ptrOfAdded, 4096);
    testMalloc.myHashTable.insert(nodeToInsert);
    std::cout << "AFTER FIRST INSERT table size should be 1: " << testMalloc.myHashTable.tableSize << std::endl;
    std::cout << "now when we try inserting again, we should get an error!" << std::endl;
    try {
        testMalloc.myHashTable.insert(nodeToInsert);
    } catch (std::runtime_error e) {
        std::cout << "ACTUAL ERROR THROWN IS: " << e.what() << std::endl << std::endl;
    }

    std::cout << "before removing table size should be 1: " << testMalloc.myHashTable.tableSize << std::endl;
    std::cout << "TEST REMOVING:" << std::endl;
    void* ptr = nodeToInsert.ptr;
    testMalloc.myHashTable.remove(ptr);
    std::cout << "after removing table size should be 0: " << testMalloc.myHashTable.tableSize << std::endl;
};

void benchmarkTests() {
    MyMalloc mallocTest = MyMalloc();
    void* ptrsOfAllocated[1000];
    for (int i = 0; i < 1000; i++) {
        ptrsOfAllocated[i] = nullptr;
    }
    auto realMallocStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; i++) {
        ptrsOfAllocated[i] = malloc(i+1);
    }
    auto realMallocEnd = std::chrono::high_resolution_clock::now();

    void* ptrsOfAllocatedMyMalloc[1000];
    for (int i = 0; i < 1000; i++) {
        ptrsOfAllocatedMyMalloc[i] = nullptr;
    }
    auto myMallocStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; i++) {
        ptrsOfAllocatedMyMalloc[i] = mallocTest.allocate(i+1);
    }
    auto myMallocEnd = std::chrono::high_resolution_clock::now();

    long myTime = std::chrono::duration_cast<std::chrono::nanoseconds>(myMallocEnd-myMallocStart).count();
    long realTime = std::chrono::duration_cast<std::chrono::nanoseconds>(realMallocEnd-realMallocStart).count();

    std::cout << "MY ALLOCATED/MALLOC time in nanoseconds was: " << myTime << std::endl;
    std::cout << "REAL malloc time in nanoseconds was: " << realTime << std::endl;

    auto realFreeStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; i++) {
        free(ptrsOfAllocated[i]);
    }
    auto realFreeEnd = std::chrono::high_resolution_clock::now();

    auto myDeallocateStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; i++) {
        mallocTest.deallocate(ptrsOfAllocatedMyMalloc[i]);
    }
    auto myDeallocateEnd = std::chrono::high_resolution_clock::now();

    long myDeallocateTime = std::chrono::duration_cast<std::chrono::nanoseconds>(myDeallocateEnd-myDeallocateStart).count();
    long realFreeTime = std::chrono::duration_cast<std::chrono::nanoseconds>(realFreeEnd-realFreeStart).count();

    std::cout << "MY DEALLOCATE/FREE time in nanoseconds was: " << myDeallocateTime << std::endl;
    std::cout << "REAL free time in nanoseconds was: " << realFreeTime << std::endl;
}

int main() {
    std::cout << "_______________________________" << std::endl;
    std::cout << "*****BENCH MARK TESTS*****" << std::endl << std::endl;
    benchmarkTests();
    std::cout << "--------------------------------" << std::endl;
    std::cout << std::endl << std::endl << std::endl;
    std::cout << "_______________________________" << std::endl;
    std::cout << "*****TEST OF ALLOCATION AND DEALLOCATION*****" << std::endl << std::endl;
    tests();
    std::cout << "-----------------------------------" << std::endl;
    std::cout << std::endl << std::endl << std::endl;

    return 0;
}
