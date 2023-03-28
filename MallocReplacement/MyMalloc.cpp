//
// Created by Randi Prince on 3/19/23.
//

#include "MyMalloc.hpp"

MyMalloc::MyMalloc() {
    myHashTable = HashTable();
}

void* MyMalloc::allocate(size_t bytesToAllocate) {
    int pagesSize = (bytesToAllocate + (4095)) / pageSize_;
    size_t allocateSize = (size_t) (pagesSize * pageSize_); // get tableSize in pages to allocate
    void *ptrAddress = mmap(nullptr, bytesToAllocate, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, 0, 0); //get ptr to mapped address region

    if (ptrAddress == MAP_FAILED) { // check that mmap was successful
        perror("mmap failed!");
        exit(1);
    }

    HashNode node = HashNode(ptrAddress, allocateSize); // create a node with the ptr address and tableSize of the data that we mapped
    myHashTable.insert(node); // put this into the hashtable

    return ptrAddress;
}

void MyMalloc::deallocate(void *ptr) {
    //bascially here we need to find the node at the ptr and remove it from our hash table
    HashNode nodeToDeallocate = myHashTable.getNodeToDelete(ptr);
    if (nodeToDeallocate.ptr != nullptr) {
        myHashTable.remove(ptr);
        if (munmap(nodeToDeallocate.ptr, nodeToDeallocate.size) == -1) {
            perror("munmap failed!");
            exit(1);
        }
    }
}