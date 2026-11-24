#include <functional>
#include <stdexcept>

template <typename K, typename V>
HashTable<K, V>::HashTable(int bucketCount)
    : buckets(bucketCount > 0 ? bucketCount : DEFAULT_BUCKET_COUNT), elementCount(0) {}

template <typename K, typename V>
int HashTable<K, V>::hashFunction(const K& key) const {
    return static_cast<int>(std::hash<K>{}(key) % buckets.size());
}

template <typename K, typename V>
void HashTable<K, V>::insert(const K& key, const V& value) {
    LinkedList<std::pair<K, V>>& bucket = buckets[hashFunction(key)];
    for (int i = 0; i < bucket.size(); ++i) {
        if (bucket.get(i).first == key) {
            bucket.get(i).second = value;
            return;
        }
    }
    bucket.insertEnd(std::make_pair(key, value));
    ++elementCount;
}

template <typename K, typename V>
V& HashTable<K, V>::get(const K& key) {
    LinkedList<std::pair<K, V>>& bucket = buckets[hashFunction(key)];
    for (int i = 0; i < bucket.size(); ++i) {
        if (bucket.get(i).first == key) {
            return bucket.get(i).second;
        }
    }
    throw std::out_of_range("HashTable key not found");
}

template <typename K, typename V>
bool HashTable<K, V>::contains(const K& key) const {
    const LinkedList<std::pair<K, V>>& bucket = buckets[hashFunction(key)];
    for (const auto& pair : bucket.toVector()) {
        if (pair.first == key) {
            return true;
        }
    }
    return false;
}

template <typename K, typename V>
bool HashTable<K, V>::remove(const K& key) {
    LinkedList<std::pair<K, V>>& bucket = buckets[hashFunction(key)];
    bool removed = bucket.removeIf([&key](const std::pair<K, V>& pair) {
        return pair.first == key;
    });
    if (removed) {
        --elementCount;
    }
    return removed;
}

template <typename K, typename V>
std::vector<V> HashTable<K, V>::values() const {
    std::vector<V> result;
    result.reserve(elementCount);
    for (const auto& bucket : buckets) {
        for (const auto& pair : bucket.toVector()) {
            result.push_back(pair.second);
        }
    }
    return result;
}
