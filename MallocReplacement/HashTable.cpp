//
// Created by Randi Prince on 3/19/23.
//

#include "HashTable.hpp"

HashNode::HashNode(void *ptr, size_t size) {
    this->ptr = ptr;
    this->size = size;
}

HashTable::HashTable() {
    HashTable(this->initialCapacity);
}

HashTable::~HashTable() {
    this->myHashTableStart = nullptr;
}

HashTable::HashTable(size_t capacity) {
    this->capacity = capacity;
    this->tableSize = 0;
    myHashTableStart = (HashNode*) mmap(nullptr, capacity * sizeof(HashNode), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (myHashTableStart == MAP_FAILED) { // check that mmap was successful
        perror("myHashTableStart mmap failed!");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < capacity; i++) {
        myHashTableStart[i].ptr = nullptr;
        myHashTableStart[i].size = 0;
    }
}

void HashTable::insert(HashNode hashNode) {
    if (tableSize >= capacity / 2) {
        grow();
    }

    int index = hashFunctor(hashNode.ptr);
    while (myHashTableStart[index].ptr != nullptr && myHashTableStart[index].ptr != hashNode.ptr && myHashTableStart[index].ptr != (void*)-1) {
        index++;
        index %= capacity;
    }

    myHashTableStart[index] = hashNode;
    tableSize++;

}

void HashTable::remove(void *ptr) {
    size_t index = hashFunctor(ptr);

    while(true) {
        if (myHashTableStart[index].ptr == nullptr || myHashTableStart[index].size == -1) {
            return;
        } else if (myHashTableStart[index].ptr == ptr) {
            myHashTableStart[index].ptr = nullptr;
            myHashTableStart[index].size = -1;
            tableSize--;
            return;
        }
        index++;
        index %= capacity;
    }

}

void HashTable::grow() {
    int oldCapcity = capacity;
    capacity *=2;
    HashNode* newBiggerTable = (HashNode*) mmap(nullptr, capacity * sizeof(HashNode), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

    for (int i = 0; i < this->capacity; i++) {
        newBiggerTable[i].ptr = nullptr;
        newBiggerTable[i].size =0;

    }

    for (int i = 0; i < oldCapcity; i++) {
        if (myHashTableStart[i].ptr !=(void*)-1 && myHashTableStart[i].ptr != nullptr) {
            size_t index = hashFunctor(myHashTableStart[i].ptr);
            while(newBiggerTable[index].ptr != nullptr) {
                index++;
            }
            newBiggerTable[index].ptr = myHashTableStart[i].ptr;
            newBiggerTable[index].size = myHashTableStart[i].size;
        }
    }

    std::swap(myHashTableStart, newBiggerTable);
    if (munmap(newBiggerTable, oldCapcity*sizeof(HashNode)) == -1) {
        perror("munmap of old table failed!");
        exit(EXIT_FAILURE);
    }
}

size_t HashTable::hashFunctor(void *ptr) {
    size_t hashSize = reinterpret_cast<size_t>(ptr);
    return (long) hashSize % capacity;
}

HashNode HashTable::getNodeToDelete(void *ptr) {
    size_t index = hashFunctor(ptr);
    while(true) {
        if (myHashTableStart[index].size == -1) {
            return HashNode(nullptr, -1);
        } else if (myHashTableStart[index].ptr == ptr) {
            return myHashTableStart[index];
        }
        index++;
        index %= capacity;
    }
}