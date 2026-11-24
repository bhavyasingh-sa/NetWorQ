#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <utility>
#include "LinkedList.h"

// Generic hash table with separate chaining (each bucket is a LinkedList
// of key-value pairs). Used for user ID -> User* lookup and for tracking
// parents during the BFS shortest-path search.
template <typename K, typename V>
class HashTable {
private:
    static const int DEFAULT_BUCKET_COUNT = 64;
    std::vector<LinkedList<std::pair<K, V>>> buckets;
    int elementCount;

    int hashFunction(const K& key) const;

public:
    explicit HashTable(int bucketCount = DEFAULT_BUCKET_COUNT);

    // Inserts the pair, or updates the value if the key already exists.
    void insert(const K& key, const V& value);

    // Returns the stored value; throws std::out_of_range if the key is absent.
    V& get(const K& key);

    bool contains(const K& key) const;

    // Returns true if an entry was removed.
    bool remove(const K& key);

    // Returns every stored value, in no particular order.
    std::vector<V> values() const;

    int size() const { return elementCount; }
};

#include "HashTable.tpp"

#endif // HASHTABLE_H
