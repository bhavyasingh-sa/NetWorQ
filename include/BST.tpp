// `comp(a, b)` means "a belongs strictly before b" (like operator<).

template <typename T, typename Compare>
typename BST<T, Compare>::Node* BST<T, Compare>::insertRec(Node* node, const T& value,
                                                           bool& inserted) {
    if (!node) {
        inserted = true;
        return new Node(value);
    }
    if (comp(value, node->data)) {
        node->left = insertRec(node->left, value, inserted);
    } else if (comp(node->data, value)) {
        node->right = insertRec(node->right, value, inserted);
    }
    return node;
}

template <typename T, typename Compare>
void BST<T, Compare>::insert(const T& value) {
    bool inserted = false;
    root = insertRec(root, value, inserted);
    if (inserted) {
        ++count;
    }
}

template <typename T, typename Compare>
void BST<T, Compare>::inorderRec(Node* node, std::vector<T>& out) const {
    if (!node) {
        return;
    }
    inorderRec(node->left, out);
    out.push_back(node->data);
    inorderRec(node->right, out);
}

template <typename T, typename Compare>
std::vector<T> BST<T, Compare>::inorderTraversal() const {
    std::vector<T> result;
    result.reserve(count);
    inorderRec(root, result);
    return result;
}

template <typename T, typename Compare>
typename BST<T, Compare>::Node* BST<T, Compare>::findMinNode(Node* node) const {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

template <typename T, typename Compare>
typename BST<T, Compare>::Node* BST<T, Compare>::removeRec(Node* node, const T& value,
                                                           bool& removed) {
    if (!node) {
        return nullptr;
    }

    if (comp(value, node->data)) {
        node->left = removeRec(node->left, value, removed);
    } else if (comp(node->data, value)) {
        node->right = removeRec(node->right, value, removed);
    } else {
        removed = true;
        if (!node->left) {
            Node* right = node->right;
            delete node;
            return right;
        }
        if (!node->right) {
            Node* left = node->left;
            delete node;
            return left;
        }
        Node* successor = findMinNode(node->right);
        node->data = successor->data;
        bool ignored = false;
        node->right = removeRec(node->right, successor->data, ignored);
    }
    return node;
}

template <typename T, typename Compare>
bool BST<T, Compare>::remove(const T& value) {
    bool removed = false;
    root = removeRec(root, value, removed);
    if (removed) {
        --count;
    }
    return removed;
}

template <typename T, typename Compare>
void BST<T, Compare>::clearRec(Node* node) {
    if (!node) {
        return;
    }
    clearRec(node->left);
    clearRec(node->right);
    delete node;
}

template <typename T, typename Compare>
void BST<T, Compare>::clear() {
    clearRec(root);
    root = nullptr;
    count = 0;
}
