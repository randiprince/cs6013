//
// Created by Randi Prince on 3/19/23.
//

#ifndef MALLOCREPLACEMENT_MYMALLOC_HPP
#define MALLOCREPLACEMENT_MYMALLOC_HPP
#include <stdio.h>
#include "HashTable.hpp"

class MyMalloc {
public:
    HashTable myHashTable;
    size_t pageSize_ = 4096;

    MyMalloc();
//    ~MyMalloc();
    void* allocate(size_t bytesToAllocate);
    void deallocate(void* ptr);

};


#endif //MALLOCREPLACEMENT_MYMALLOC_HPP
