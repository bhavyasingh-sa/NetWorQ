template <typename T>
void Queue<T>::enqueue(const T& value) {
    Node* node = new Node(value);
    if (!rearNode) {
        frontNode = rearNode = node;
    } else {
        rearNode->next = node;
        rearNode = node;
    }
    ++count;
}

template <typename T>
T Queue<T>::dequeue() {
    if (isEmpty()) {
        throw std::underflow_error("Queue is empty");
    }
    Node* old = frontNode;
    T value = old->data;
    frontNode = old->next;
    delete old;
    if (!frontNode) {
        rearNode = nullptr;
    }
    --count;
    return value;
}

template <typename T>
T& Queue<T>::front() {
    if (isEmpty()) {
        throw std::underflow_error("Queue is empty");
    }
    return frontNode->data;
}

template <typename T>
bool Queue<T>::isEmpty() const {
    return count == 0;
}

template <typename T>
int Queue<T>::size() const {
    return count;
}

template <typename T>
void Queue<T>::clear() {
    while (frontNode) {
        Node* next = frontNode->next;
        delete frontNode;
        frontNode = next;
    }
    rearNode = nullptr;
    count = 0;
}
