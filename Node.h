#ifndef NODE_H
#define NODE_H

// ==================== MANUAL LINKED LIST IMPLEMENTATION ====================
// Template class for LinkedList
template <typename T>
class Node {
public:
    T data;         // The actual data stored in the node
    Node<T>* next;  // Pointer to the next node in the list

    Node(T value) : data(value), next(nullptr) {}   // Constructor initializes data and sets next to null
};

#endif
