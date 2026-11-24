template <typename T>
void Stack<T>::push(const T& value) {
    Node* node = new Node(value);
    node->next = topNode;
    topNode = node;
    ++count;
}

template <typename T>
T Stack<T>::pop() {
    if (isEmpty()) {
        throw std::underflow_error("Stack is empty");
    }
    Node* old = topNode;
    T value = old->data;
    topNode = old->next;
    delete old;
    --count;
    return value;
}

template <typename T>
T& Stack<T>::top() {
    if (isEmpty()) {
        throw std::underflow_error("Stack is empty");
    }
    return topNode->data;
}

template <typename T>
bool Stack<T>::isEmpty() const {
    return count == 0;
}

template <typename T>
int Stack<T>::size() const {
    return count;
}

template <typename T>
void Stack<T>::clear() {
    while (topNode) {
        Node* next = topNode->next;
        delete topNode;
        topNode = next;
    }
    count = 0;
}
