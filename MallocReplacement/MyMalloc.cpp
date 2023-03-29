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
    void *ptrAddress = mmap(nullptr, allocateSize, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, 0, 0); // sys call to get ptr to mapped address region

    if (ptrAddress == MAP_FAILED) { // check that mmap was successful
        perror("mmap failed!");
        exit(1);
    }

    HashNode node = HashNode(ptrAddress, allocateSize); // create a node with the ptr address and the size of the data that we mapped
    myHashTable.insert(node); // put this into the hashtable

    return ptrAddress;
}

void MyMalloc::deallocate(void *ptr) {
    //bascially here we need to find the node at the ptr and remove it from our hash table
    size_t sizeOfRemovedNode = myHashTable.remove(ptr);
    if (sizeOfRemovedNode == -1) {
        throw std::runtime_error("error! passed node was not removed!");
    }
    if (munmap(ptr, sizeOfRemovedNode) == -1) { // here use munmap sys call to remove info from memory
        perror("munmap failed in deallocate!");
        exit(1);
    }
}