template <typename T>
LinkedList<T>::LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), count(0) {
    for (Node* current = other.head; current; current = current->next) {
        insertEnd(current->data);
    }
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other) {
    if (this == &other) {
        return *this;
    }
    clear();
    for (Node* current = other.head; current; current = current->next) {
        insertEnd(current->data);
    }
    return *this;
}

template <typename T>
void LinkedList<T>::insertEnd(const T& value) {
    Node* node = new Node(value);
    if (!head) {
        head = tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
    ++count;
}

template <typename T>
void LinkedList<T>::insertFront(const T& value) {
    Node* node = new Node(value);
    node->next = head;
    head = node;
    if (!tail) {
        tail = node;
    }
    ++count;
}

template <typename T>
bool LinkedList<T>::removeAt(int index) {
    if (index < 0 || index >= count) {
        return false;
    }

    Node* previous = nullptr;
    Node* doomed = head;
    for (int i = 0; i < index; ++i) {
        previous = doomed;
        doomed = doomed->next;
    }

    if (previous) {
        previous->next = doomed->next;
    } else {
        head = doomed->next;
    }
    if (doomed == tail) {
        tail = previous;
    }

    delete doomed;
    --count;
    return true;
}

template <typename T>
template <typename Predicate>
bool LinkedList<T>::removeIf(Predicate pred) {
    Node* previous = nullptr;
    for (Node* current = head; current; previous = current, current = current->next) {
        if (!pred(current->data)) {
            continue;
        }
        if (previous) {
            previous->next = current->next;
        } else {
            head = current->next;
        }
        if (current == tail) {
            tail = previous;
        }
        delete current;
        --count;
        return true;
    }
    return false;
}

template <typename T>
T& LinkedList<T>::get(int index) {
    if (index < 0 || index >= count) {
        throw std::out_of_range("LinkedList index out of range");
    }
    Node* current = head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

template <typename T>
int LinkedList<T>::size() const {
    return count;
}

template <typename T>
std::vector<T> LinkedList<T>::toVector() const {
    std::vector<T> result;
    result.reserve(count);
    for (Node* current = head; current; current = current->next) {
        result.push_back(current->data);
    }
    return result;
}

template <typename T>
void LinkedList<T>::clear() {
    while (head) {
        Node* next = head->next;
        delete head;
        head = next;
    }
    tail = nullptr;
    count = 0;
}
