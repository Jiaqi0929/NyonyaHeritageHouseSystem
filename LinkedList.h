#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Node.h"
#include <iostream>
using namespace std;

// ==================== MANUAL LINKED LIST IMPLEMENTATION ====================
// Template class for LinkedList
template <typename T>
class LinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int size;

public:
    LinkedList();
    ~LinkedList();
    void insertAtEnd(T value);
    void insertAtBeginning(T value);
    bool deleteNode(int index);
    T* get(int index);
    Node<T>* search(T key, bool (*compare)(T, T));

    template<typename Compare>
    void bubbleSort(Compare compare);

    int getSize() const;
    bool isEmpty() const;
    void clear();
    Node<T>* getHead() const;
    void traverse();

    // ==================== ITERATOR CLASSES ====================
    // Iterator for non-const traversal
    class Iterator {
    private:
        Node<T>* current;
    public:
        Iterator(Node<T>* node) : current(node) {}
        bool hasNext() const { return current != nullptr; }
        T& next() {
            T& data = current->data;
            current = current->next;
            return data;
        }
        T* currentPtr() { return current ? &(current->data) : nullptr; }
        void moveNext() { if (current) current = current->next; }
    };

    // Iterator for const traversal
    class ConstIterator {
    private:
        const Node<T>* current;
    public:
        ConstIterator(const Node<T>* node) : current(node) {}
        bool hasNext() const { return current != nullptr; }
        const T& next() {
            const T& data = current->data;
            current = current->next;
            return data;
        }
        const T* currentPtr() const { return current ? &(current->data) : nullptr; }
        void moveNext() { if (current) current = current->next; }
    };

    // Begin iterator methods
    Iterator begin() { return Iterator(head); }
    ConstIterator begin() const { return ConstIterator(head); }
};

// Include implementation
#include "LinkedList.cpp"

#endif
