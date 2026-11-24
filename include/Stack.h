#ifndef STACK_H
#define STACK_H

#include <stdexcept>

// Generic LIFO stack, linked-list based (no fixed capacity).
// Used to record recent user actions for undo.
template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* topNode;
    int count;

public:
    Stack() : topNode(nullptr), count(0) {}
    ~Stack() { clear(); }

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    void push(const T& value);
    T pop();          // remove and return the top element
    T& top();         // peek at the top element without removing it
    bool isEmpty() const;
    int size() const;
    void clear();
};

#include "Stack.tpp"

#endif // STACK_H
