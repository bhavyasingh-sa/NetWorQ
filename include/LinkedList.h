#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <vector>
#include <stdexcept>

// Generic singly linked list with head and tail pointers.
// Used for a user's skills, achievements and connection IDs, and for the
// buckets of the HashTable.
template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    LinkedList() : head(nullptr), tail(nullptr), count(0) {}
    ~LinkedList() { clear(); }

    LinkedList(const LinkedList& other);
    LinkedList& operator=(const LinkedList& other);

    void insertEnd(const T& value);
    void insertFront(const T& value);
    bool removeAt(int index);

    template <typename Predicate>
    bool removeIf(Predicate pred);

    T& get(int index);
    int size() const;
    bool isEmpty() const { return size() == 0; }
    std::vector<T> toVector() const;
    void clear();
};

#include "LinkedList.tpp"

#endif // LINKEDLIST_H
