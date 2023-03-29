//
// Created by Randi Prince on 3/19/23.
//

#ifndef MALLOCREPLACEMENT_HASHTABLE_HPP
#define MALLOCREPLACEMENT_HASHTABLE_HPP
#include <cstdio>
#include <sys/mman.h>
#include <iostream>

class HashNode {
public:
    void* ptr;
    size_t size;
    HashNode(void *ptr, size_t size);
};

class HashTable {
public:
    int tableSize;
    size_t capacity;
    size_t initialCapacity = 10;
    HashNode* myHashTableStart;

    HashTable();
    ~HashTable();

    void insert(HashNode hashNode);
    size_t remove(void *ptr);
    void grow();
    size_t hashFunctor(void *ptr);
};


#endif //MALLOCREPLACEMENT_HASHTABLE_HPP
