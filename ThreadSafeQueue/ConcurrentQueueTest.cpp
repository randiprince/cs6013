//
// Created by Randi Prince on 4/12/23.
//
#include <iostream>
#include <vector>
#include <thread>

#include "ConcurrentQueue.hpp"

void enqueue(ConcurrentQueue<int>* queue, int numInts) {
    for (int i = 0; i < numInts; i++) {
        queue->enqueue(i);
    }
}

void dequeue(ConcurrentQueue<int>* queue, int numInts) {
    for (int i = 0; i < numInts; i++) {
        int dequeue;
        queue->dequeue(&dequeue);
    }
}

bool testQueue(int num_producers, int num_consumers, int num_ints) {
    std::vector<std::thread> threads;
    threads.reserve(num_producers + num_consumers);
    auto concurrentQueue = new ConcurrentQueue<int>();

    for (int i = 0; i < num_producers; i++) {
        threads.push_back(std::thread(enqueue, concurrentQueue, num_ints));
    }

    for (int i = 0; i < num_consumers; i++) {
        threads.push_back(std::thread(dequeue, concurrentQueue, num_ints));
    }

    for (int i = 0; i < (num_producers + num_consumers); i++) {
        threads[i].join();
    }

    return (concurrentQueue->size() == (num_producers - num_consumers) * num_ints);
}

int main(int argc, char *argv[]) {
    int result = testQueue(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
    std::cout << result << std::endl;
    std::cout << atoi(argv[1]) << std::endl;
    std::cout << atoi(argv[2]) << std::endl;
    std::cout << atoi(argv[3]) << std::endl;
    return 0;
}