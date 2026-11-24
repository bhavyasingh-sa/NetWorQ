#ifndef BST_H
#define BST_H

#include <vector>
#include <functional>

// Generic binary search tree, ordered by a comparator (defaults to
// operator< on T). Used to list users sorted by name or by profession.
//
// Compare must behave like std::less: Compare()(a, b) returns true when
// `a` belongs strictly before `b`. Two elements are treated as equal when
// neither is less than the other, so T needs no operator==.
template <typename T, typename Compare = std::less<T>>
class BST {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        Node(const T& value) : data(value), left(nullptr), right(nullptr) {}
    };

    Node* root;
    int count;
    Compare comp;

    // Recursive helpers.
    Node* insertRec(Node* node, const T& value, bool& inserted);
    void inorderRec(Node* node, std::vector<T>& out) const;
    void clearRec(Node* node);
    Node* removeRec(Node* node, const T& value, bool& removed);
    Node* findMinNode(Node* node) const;

public:
    BST() : root(nullptr), count(0) {}
    ~BST() { clear(); }

    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

    void insert(const T& value);

    // Returns all elements in ascending order (per Compare).
    std::vector<T> inorderTraversal() const;

    bool remove(const T& value);

    int size() const { return count; }
    bool isEmpty() const { return count == 0; }
    void clear();
};

#include "BST.tpp"

#endif // BST_H
