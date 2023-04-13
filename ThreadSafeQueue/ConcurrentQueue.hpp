//
// Created by Randi Prince on 4/12/23.
//

#ifndef THREADSAFEQUEUE_CONCURRENTQUEUE_HPP
#define THREADSAFEQUEUE_CONCURRENTQUEUE_HPP

#include <mutex>

template <typename T>
class ConcurrentQueue {
private:
    struct Node {
        T data;
        Node* next;
    };

    Node* head;
    Node* tail;
    int size_;
    std::mutex mutexHead, mutextTail;

public:
    ConcurrentQueue() : head(new Node{T{}, nullptr}), size_(0) {
        tail = head;
    }

    void enqueue(const T& x) {
        Node* node = new Node{x, nullptr};
        mutextTail.lock();
        if (size_ == 0) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        size_++;
        mutextTail.unlock();
    }

    bool dequeue(T* ret) {
        mutexHead.lock();
        if (head->next == nullptr) {
            return false;
        }
        Node* tempHead = head;
        *ret = tempHead->next->data;
        head = tempHead->next;
        delete tempHead;
        size_--;
        mutexHead.unlock();
        return true;
    }

    ~ConcurrentQueue() {
        while(head) {
            Node* temp = head->next;
            delete head;
            head = temp;
        }
    }

    int size() const {
        return size_;
    }
};


#endif //THREADSAFEQUEUE_CONCURRENTQUEUE_HPP
