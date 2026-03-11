#ifndef LINKEDLIST_CPP
#define LINKEDLIST_CPP

#include "LinkedList.h"

// Constructor - initializes empty list
template <typename T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), size(0) {}

// Destructor - cleans up all nodes to prevent memory leaks
template <typename T>
LinkedList<T>::~LinkedList() {
    clear();
}

// Insert a new node at the end of the list (O(1) operation)
template <typename T>
void LinkedList<T>::insertAtEnd(T value) {
    Node<T>* newNode = new Node<T>(value);
    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

// Insert a new node at the beginning of the list (O(1) operation)
template <typename T>
void LinkedList<T>::insertAtBeginning(T value) {
    Node<T>* newNode = new Node<T>(value);
    newNode->next = head;
    head = newNode;
    if (!tail) {
        tail = newNode;
    }
    size++;
}

// Delete a node at specific index (O(n) operation)
template <typename T>
bool LinkedList<T>::deleteNode(int index) {
    if (index < 0 || index >= size || !head) return false;

    Node<T>* toDelete = nullptr;
    if (index == 0) {
        toDelete = head;
        head = head->next;
        if (!head) tail = nullptr;
    } else {
        Node<T>* current = head;
        for (int i = 0; i < index - 1; i++) {
            current = current->next;
        }
        toDelete = current->next;
        current->next = toDelete->next;
        if (toDelete == tail) {
            tail = current;
        }
    }

    delete toDelete;
    size--;
    return true;
}

// Get element at specific index
template <typename T>
T* LinkedList<T>::get(int index) {
    if (index < 0 || index >= size) return nullptr;

    Node<T>* current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return &(current->data);
}

// Search for a node using comparison function
template <typename T>
Node<T>* LinkedList<T>::search(T key, bool (*compare)(T, T)) {
    Node<T>* current = head;
    while (current) {
        if (compare(current->data, key)) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// Bubble sort implementation with custom comparator
template <typename T>
template<typename Compare>
void LinkedList<T>::bubbleSort(Compare compare) {
    if (!head || !head->next) return;

    bool swapped;
    do {
        swapped = false;
        Node<T>* current = head;
        Node<T>* prev = nullptr;

        while (current && current->next) {
            Node<T>* nextNode = current->next;
            if (!compare(current->data, nextNode->data)) {
                // Swap nodes
                if (prev) {
                    prev->next = nextNode;
                } else {
                    head = nextNode;
                }
                current->next = nextNode->next;
                nextNode->next = current;

                // Update tail if necessary
                if (!current->next) {
                    tail = current;
                } else if (!nextNode->next) {
                    tail = nextNode;
                }

                swapped = true;
                prev = nextNode;
            } else {
                prev = current;
                current = current->next;
            }
        }
    } while (swapped);
}

// Utility methods to get size
template <typename T>
int LinkedList<T>::getSize() const { return size; }

// Utility methods to check if empty
template <typename T>
bool LinkedList<T>::isEmpty() const { return size == 0; }

// Clear all nodes from the list
template <typename T>
void LinkedList<T>::clear() {
    Node<T>* current = head;
    while (current) {
        Node<T>* next = current->next;
        delete current;
        current = next;
    }
    head = tail = nullptr;
    size = 0;
}

// Getter methods
template <typename T>
Node<T>* LinkedList<T>::getHead() const { return head; }

// Display all elements in the list
template <typename T>
void LinkedList<T>::traverse() {
    Node<T>* current = head;
    while (current) {
        cout << current->data << " ";
        current = current->next;
    }
    cout << endl;
}

#endif
