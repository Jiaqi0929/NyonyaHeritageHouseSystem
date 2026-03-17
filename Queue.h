#ifndef QUEUE_H
#define QUEUE_H

#include "Node.h"
#include <stdexcept>
using namespace std;

// ==================== MANUAL QUEUE IMPLEMENTATION ====================
// First-In-First-Out (FIFO) data structure implementation
template <typename T>
class Queue {
private:
    Node<T>* front;
    Node<T>* rear;
    int size;

public:
    // Constructor - initializes empty queue
    Queue() : front(nullptr), rear(nullptr), size(0) {}

    // Destructor - clears all elements
    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    // Add element to back of queue
    void enqueue(T value) {
        Node<T>* newNode = new Node<T>(value);
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }

    // Remove and return front element
    T dequeue() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }

        T value = front->data;
        Node<T>* temp = front;
        front = front->next;
        delete temp;
        size--;

        if (isEmpty()) {
            rear = nullptr;
        }

        return value;
    }

    // Peek at front element without removing
    T peek() const {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }
        return front->data;
    }

    // Utility methods
    bool isEmpty() const { return size == 0; }
    int getSize() const { return size; }

    // Clear all elements from queue
    void clear() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};

#endif
