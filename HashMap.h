#ifndef HASHMAP_H
#define HASHMAP_H

#include "LinkedList.h"
#include <string>
#include <utility>
#include <stdexcept>
using namespace std;

// ==================== MANUAL HASHMAP IMPLEMENTATION ====================
// Template class for HashMap using separate chaining for collision resolution
template<typename K, typename V>
class HashMap {
private:
    // Node structure for chaining
    struct HashNode {
        K key;
        V value;
        HashNode* next;
        HashNode(K k, V v) : key(k), value(v), next(nullptr) {}
    };

    static const int TABLE_SIZE = 100;
    HashNode** table;
    int size;

    // Hash function to convert key to index
    int hashFunction(const K& key) {
        // Simple hash for strings
        if (typeid(K) == typeid(string)) {
            int hash = 0;
            string strKey = to_string(key);
            for (char c : strKey) {
                hash = (hash * 31 + c) % TABLE_SIZE;
            }
            return hash;
        }
        // For integers
        return key % TABLE_SIZE;
    }

public:
    // Constructor - allocate table and initialize to null
    HashMap() : size(0) {
        table = new HashNode*[TABLE_SIZE]();
    }

    // Destructor - clean up all nodes
    ~HashMap() {
        clear();
        delete[] table;
    }

    // Insert key-value pair
    void insert(const K& key, const V& value) {
        int index = hashFunction(key);
        HashNode* newNode = new HashNode(key, value);

        // Insert at beginning of chain
        newNode->next = table[index];
        table[index] = newNode;
        size++;
    }

    // Check if key exists
    bool contains(const K& key) {
        int index = hashFunction(key);
        HashNode* current = table[index];

        // Traverse chain to find key
        while (current) {
            if (current->key == key) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Get value by key, insert default if not found
    V& get(const K& key) {
        int index = hashFunction(key);
        HashNode* current = table[index];

        // Search for existing key
        while (current) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }

        // If not found, insert default and return it
        insert(key, V());
        return get(key);
    }

    // Remove key-value pair
    bool remove(const K& key) {
        int index = hashFunction(key);
        HashNode* current = table[index];
        HashNode* prev = nullptr;

        // Find and remove node
        while (current) {
            if (current->key == key) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    table[index] = current->next;
                }
                delete current;
                size--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    // Clear all entries from hashmap
    void clear() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashNode* current = table[i];
            while (current) {
                HashNode* temp = current;
                current = current->next;
                delete temp;
            }
            table[i] = nullptr;
        }
        size = 0;
    }

    // Utility methods
    int getSize() const { return size; }
    bool isEmpty() const { return size == 0; }

    // Display all key-value pairs
    void displayAll() const {
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashNode* current = table[i];
            while (current) {
                cout << "Key: " << current->key << ", Value: " << current->value << endl;
                current = current->next;
            }
        }
    }

    // Get all keys as a LinkedList
    LinkedList<K> getAllKeys() const {
        LinkedList<K> keys;
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashNode* current = table[i];
            while (current) {
                keys.insertAtEnd(current->key);
                current = current->next;
            }
        }
        return keys;
    }

    // ==================== ITERATOR FOR HASHMAP ====================
    class Iterator {
    private:
        HashMap<K, V>& map;
        int bucketIndex;
        HashNode* current;

        // Helper to find next non-empty bucket
        void findNext() {
            while (bucketIndex < HashMap::TABLE_SIZE && !current) {
                bucketIndex++;
                if (bucketIndex < HashMap::TABLE_SIZE) {
                    current = map.table[bucketIndex];
                }
            }
        }

    public:
        // Constructor - Initialize iterator starting at specified bucket index
        Iterator(HashMap<K, V>& m, int index = 0) : map(m), bucketIndex(index) {
            if (bucketIndex < HashMap::TABLE_SIZE) {
                current = map.table[bucketIndex];
            }
            findNext();
        }

        // Check if iterator has more elements
        bool hasNext() const { return current != nullptr; }

        // Get current data
        pair<K, V> next() {
            if (!hasNext()) throw runtime_error("No more elements");

            pair<K, V> result = {current->key, current->value};
            current = current->next;
            findNext();

            return result;
        }
    };

    // Return iterator positioned at first element in hashmap
    Iterator begin() { return Iterator(*this); }
};

#endif

