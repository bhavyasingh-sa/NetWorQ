#ifndef TRIE_H
#define TRIE_H

#include <cctype>
#include <string>
#include <unordered_map>
#include <vector>

// Prefix tree over lowercase user names, mapping each stored name to the
// user ID(s) that own it. Powers the case-insensitive prefix search
// (e.g. "al" finds every name that starts with "Al", "AL" or "al").
class Trie {
private:
    struct TrieNode {
        std::unordered_map<char, TrieNode*> children;
        bool isEndOfWord = false;
        std::vector<int> userIds;  // supports duplicate names

        ~TrieNode() {
            for (auto& pair : children) {
                delete pair.second;
            }
        }
    };

    TrieNode* root;

    // Depth-first collection of every user ID stored at or below `node`.
    void collectUserIds(TrieNode* node, std::vector<int>& out) const;

public:
    Trie() : root(new TrieNode()) {}
    ~Trie() { delete root; }

    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;

    // Stores `name` (lowercased) and associates it with `userId`.
    void insert(const std::string& name, int userId);

    // Empties the trie (used to rebuild it after a profile-name update).
    void clear();

    // Returns the IDs of all users whose name starts with `prefix`
    // (case-insensitive); empty if there is no match.
    std::vector<int> searchByPrefix(const std::string& prefix) const;
};

inline void Trie::collectUserIds(TrieNode* node, std::vector<int>& out) const {
    if (!node) {
        return;
    }
    if (node->isEndOfWord) {
        out.insert(out.end(), node->userIds.begin(), node->userIds.end());
    }
    for (const auto& pair : node->children) {
        collectUserIds(pair.second, out);
    }
}

inline void Trie::insert(const std::string& name, int userId) {
    TrieNode* current = root;
    for (unsigned char raw : name) {
        char c = static_cast<char>(std::tolower(raw));
        auto it = current->children.find(c);
        if (it == current->children.end()) {
            it = current->children.emplace(c, new TrieNode()).first;
        }
        current = it->second;
    }
    current->isEndOfWord = true;
    current->userIds.push_back(userId);
}

inline void Trie::clear() {
    delete root;
    root = new TrieNode();
}

inline std::vector<int> Trie::searchByPrefix(const std::string& prefix) const {
    TrieNode* current = root;
    for (unsigned char raw : prefix) {
        char c = static_cast<char>(std::tolower(raw));
        auto it = current->children.find(c);
        if (it == current->children.end()) {
            return {};
        }
        current = it->second;
    }

    std::vector<int> result;
    collectUserIds(current, result);
    return result;
}

#endif  // TRIE_H
