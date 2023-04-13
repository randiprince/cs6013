//
// Created by Randi Prince on 4/12/23.
//

#include <iostream>
#include "SerialQueue.hpp"

void staticTests() {
    SerialQueue<int> staticQ{};
    staticQ.enqueue(1);
    staticQ.enqueue(2);
    staticQ.enqueue(3);
    staticQ.enqueue(4);
    staticQ.enqueue(5);
    assert(staticQ.size() == 5);
    std::cout << "size is: " << staticQ.size() << std::endl;
    int dequeued;
    staticQ.dequeue(&dequeued);
    std::cout << "dequeued value is: " << dequeued << std::endl;
    assert(staticQ.dequeue(&dequeued));
    assert(dequeued == 3);
}

void dynamicTests() {
    SerialQueue<int> *dynamicQ = new SerialQueue<int>();
    dynamicQ->enqueue(1);
    dynamicQ->enqueue(2);
    dynamicQ->enqueue(3);
    dynamicQ->enqueue(4);
    dynamicQ->enqueue(5);
    assert(dynamicQ->size() == 5);
    std::cout << "size is: " << dynamicQ->size() << std::endl;
    int dequeued;
    dynamicQ->dequeue(&dequeued);
    std::cout << "dequeued value is: " << dequeued << std::endl;
    assert(dynamicQ->dequeue(&dequeued));
    assert(dequeued == 3);
    assert(dynamicQ->size() == 3);
    delete dynamicQ;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    staticTests();
    dynamicTests();
    return 0;
}
