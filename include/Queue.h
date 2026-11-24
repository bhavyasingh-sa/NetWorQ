#ifndef QUEUE_H
#define QUEUE_H

#include <stdexcept>

// Generic FIFO queue, linked-list based.
// Used for notifications (processed in arrival order) and as the BFS frontier.
template <typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* frontNode;
    Node* rearNode;
    int count;

public:
    Queue() : frontNode(nullptr), rearNode(nullptr), count(0) {}
    ~Queue() { clear(); }

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    void enqueue(const T& value);
    T dequeue();       // remove and return the front element
    T& front();        // peek at the front element without removing it
    bool isEmpty() const;
    int size() const;
    void clear();
};

#include "Queue.tpp"

#endif // QUEUE_H
