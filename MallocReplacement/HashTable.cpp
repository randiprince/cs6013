//
// Created by Randi Prince on 3/19/23.
//

#include "HashTable.hpp"

HashNode::HashNode(void *ptr, size_t size) { // hash node constructor which make up the array that is our hash table
    this->ptr = ptr;
    this->size = size;
}

HashTable::~HashTable() {
    this->myHashTableStart = nullptr;
}

HashTable::HashTable() {
    this->capacity = initialCapacity; // give table capacity to hold 10 values
    this->tableSize = 0; // initially set this to 0 because hash table should be empty
    myHashTableStart = (HashNode*) mmap(nullptr, capacity * sizeof(HashNode), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

    if (myHashTableStart == MAP_FAILED) { // check that mmap was successful
        perror("myHashTableStart mmap failed!");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < capacity; i++) { // initialize each of the 10 slots (each of which are a hashnode) with empty data indicating no memory has been mapped
        myHashTableStart[i].ptr = nullptr;
        myHashTableStart[i].size = 0;
    }
}

void HashTable::insert(HashNode hashNode) {
    if (tableSize >= capacity / 2) { // if the size > capacity we need to grow so we do not go out of bounds of our array
        grow();
    }

    int index = hashFunctor(hashNode.ptr);

    // confirm that where we are trying to insert does not contain data or a lazy delete
    while (myHashTableStart[index].ptr != nullptr && myHashTableStart[index].ptr != hashNode.ptr && myHashTableStart[index].ptr != (void*)-1) {
        index++;
        index %= capacity;
    }

    if (myHashTableStart[index].ptr == hashNode.ptr) {
        throw std::runtime_error("you cannot use the same address twice!!!!!");
    }

    myHashTableStart[index] = hashNode;
    tableSize++;

}

size_t HashTable::remove(void *ptr) {
    size_t index = hashFunctor(ptr);
    size_t sizeToReturn = -1;
    while(true) {
        if (myHashTableStart[index].ptr == nullptr) {
            return sizeToReturn;
        } else if (myHashTableStart[index].ptr == ptr) { // gives us the node we want to remove
            myHashTableStart[index].ptr = (void*)-1; // indicates lazy delete
            sizeToReturn = myHashTableStart[index].size;
            myHashTableStart[index].size = -1;
            tableSize--;
            return sizeToReturn;
        }
        index++;
        index %= capacity;
    }

}

void HashTable::grow() {
    int oldCapcity = capacity; //store old capacity to use for copying values
    capacity *=2; // double capcity so we can create a new table twice the size

    // initialize new table with twice the capacity
    HashNode* newBiggerTable = (HashNode*) mmap(nullptr, capacity * sizeof(HashNode), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

    if (newBiggerTable == MAP_FAILED) { // check that mmap was successful
        perror("newBiggerTable mmap failed in grow!");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < this->capacity; i++) { // initialize new table with empty data indicating no memory has been mapped
        newBiggerTable[i].ptr = nullptr;
        newBiggerTable[i].size = 0;

    }

    for (int i = 0; i < oldCapcity; i++) { // for the size of the old table
        if (myHashTableStart[i].ptr != (void*)-1 && myHashTableStart[i].ptr != nullptr) { // if the data has not been lazy deleted or is nullptr
            size_t index = hashFunctor(myHashTableStart[i].ptr); // get index of the value in the old table
            while(newBiggerTable[index].ptr != nullptr) { // as we loop through, don't want to copy to a place we've already copied to
                index++;
            }
            newBiggerTable[index].ptr = myHashTableStart[i].ptr; // and use that as the index in the new table to copy the data to
            newBiggerTable[index].size = myHashTableStart[i].size;
        }
    }

    std::swap(myHashTableStart, newBiggerTable);
    if (munmap(newBiggerTable, oldCapcity*sizeof(HashNode)) == -1) { // release old memory
        perror("munmap of old table failed!");
        exit(EXIT_FAILURE);
    }
}

size_t HashTable::hashFunctor(void *ptr) {
    size_t hashSize = reinterpret_cast<size_t>(ptr);
    return (long) hashSize % capacity;
}

//helper function used in malloc's deallocate
//HashNode HashTable::getNodeToDelete(void *ptr) {
//    size_t index = hashFunctor(ptr);
//    while(true) {
//        if (myHashTableStart[index].ptr == nullptr || myHashTableStart[index].size == -1) { // don't want to delete a node that is empty and doesn't need to be deleted
//            return HashNode(nullptr, -1);
//        } else if (myHashTableStart[index].ptr == ptr) { // gives us node to delete
//            return myHashTableStart[index];
//        }
//        index++;
//        index %= capacity;
//    }
//}