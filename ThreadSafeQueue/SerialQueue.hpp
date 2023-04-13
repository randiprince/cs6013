#pragma once

template <typename T>
class SerialQueue {
private:
  struct Node {
	T data;
	Node* next;
  };

  Node* head;
  Node* tail;
  int size_;

public:
  SerialQueue() : head(new Node{T{}, nullptr}), size_(0) {
      tail = head;
  }


  void enqueue(const T& x) {
      Node* node = new Node{x, nullptr};

      if (size_ == 0) { // if size is 0 then adding one means head and tail are the same
          head = node;
          tail = node;
      } else {
          tail->next = node;
          tail = node;
      }
      size_++;
  }

  bool dequeue(T* ret) {
      if (head->next == nullptr) { // if only head, nothing to dequeue
          return false;
      }
      Node* tempHead = head;
      *ret = tempHead->next->data;
      head = tempHead->next;
      delete tempHead;
      size_--;
      return true;
  }

  ~SerialQueue() {
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
