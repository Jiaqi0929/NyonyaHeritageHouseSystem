#include "OrderManager.h"
#include "OrderItem.h"
#include "Utilities.h"
#include <iostream>
#include <iomanip>
using namespace std;

// Constructor: initialize reservation ID to 0
OrderManager::OrderManager() : currentReservationId(0) {}

// Set the current customer's name
void OrderManager::setCustomer(string customer) { currentCustomer = customer; }

// Set the current reservation ID
void OrderManager::setReservationId(int resId) { currentReservationId = resId; }

// Find the index of an order by item name
int OrderManager::findOrderIndex(const string& itemName) {
    auto it = orders.begin();   // Iterator to traverse linked list
    int index = 0;
    while (it.hasNext()) {      // Loop through all orders
        OrderItem& order = it.next();
        if (order.getName() == itemName) {
            return index;
        }
        index++;
    }
    return -1;
}

// Add a new order or update existing order
void OrderManager::addOrder(MenuItem* item, int quantity) {
    int existingIndex = findOrderIndex(item->getName());

    if (existingIndex >= 0) {       // If item already exists
        OrderItem* existingOrder = orders.get(existingIndex);
        int newQuantity = existingOrder->getQuantity() + quantity;

        if (newQuantity <= 100) {
            existingOrder->setQuantity(newQuantity);
            cout << "\n\t\tOrder quantity updated! Total " << item->getName()
                 << " x" << newQuantity << endl;
        } else {
            cout << "\n\t\tCannot add more than 100 of the same item!\n";
            return;
        }
    } else {
        if (quantity > 0 && quantity <= 100) {      // If item is new
            // Insert new order into linked list
            orders.insertAtEnd(OrderItem(item, quantity, currentCustomer, currentReservationId));
            cout << "\n\t\tOrder added successfully!\n";
        } else {
            cout << "\n\t\tInvalid quantity! Must be between 1-100.\n";
            return;
        }
    }
}

// Edit the quantity of an existing order
void OrderManager::editOrder(int index, int quantity) {
    OrderItem* order = orders.get(index);
    if (order && quantity > 0 && quantity <= 100) {
        order->setQuantity(quantity);       // Update quantity
    } else {
        cout << "\t\tInvalid index or quantity (must be in range 1-100).\n";
    }
}

// Delete an order by index
bool OrderManager::deleteOrder(int index) {
    return orders.deleteNode(index);        // Remove node from linked list
}

// Display all current orders
void OrderManager::displayOrders() const {
    if (orders.isEmpty()) {                 // Check if list is empty
        cout << "\t\tNo orders yet.\n";
        return;
    }

    cout << "\n\n\t\t" << string(60, '=') << "\n";
    cout << "\t\t                        YOUR ORDERS\n";
    cout << "\t\t" << string(60, '=') << "\n";
    cout << left << setw(7) << "\t\tNO" << left << setw(30) << "ITEM"
         << left << setw(10) << "QTY" << left << setw(15) << "PRICE" << endl;
    cout << "\t\t" << string(60, '-') << "\n";

    int i = 1;
    auto it = orders.begin();
    while (it.hasNext()) {          // Loop through orders
        const OrderItem& order = it.next();

        cout << left << "\t\t" << setw(5) << i++
             << left << setw(30) << (order.getName().length() > 28 ? order.getName().substr(0, 28) + ".." : order.getName())
             << left << setw(10) << order.getQuantity()
             << "RM " << fixed << setprecision(2) << left << setw(12) << order.getPrice() << endl;
    }
    cout << "\t\t" << string(60, '=') << "\n";
}

// Search orders by name (case-insensitive)
void OrderManager::searchOrder(string name) {
    bool found = false;
    cout << "\n\n\t\tSearch Results for \"" << name << "\":\n";
    cout << "\t\t" << string(60, '-') << "\n";

    int i = 1;
    auto it = orders.begin();
    while (it.hasNext()) {
        OrderItem& order = it.next();
        // Check if keyword exists in item name
        if (caseInsensitiveContains(order.getName(), name)) {
            cout << left << "\t\t" << setw(5) << i++
                 << left << setw(25) << order.getName()
                 << left << setw(10) << order.getQuantity()
                 << "RM " << fixed << setprecision(2) << order.getPrice() << endl;
            found = true;
        }
    }
    if (!found) cout << "\t\tNo matching order found.\n";
}

// Sort orders by price using Bubble Sort
void OrderManager::sortOrdersByPrice() {
    if (orders.isEmpty()) {
        cout << "\t\tNo orders to sort.\n";
        return;
    }

    // Lambda function for comparing prices
    auto comparePrice = [](OrderItem a, OrderItem b) {
        return a.getPrice() < b.getPrice();
    };

    orders.bubbleSort(comparePrice);
    cout << "\n\t\tOrders sorted by price (ascending) using Bubble Sort:\n";
    displayOrders();
}

// Calculate total price of all orders
float OrderManager::getTotal() const {
    float total = 0;
    auto it = orders.begin();
    while (it.hasNext()) {
        total += it.next().getPrice();  // Sum up prices
    }
    return total;
}

// Check if there are any orders
bool OrderManager::hasOrders() const { return !orders.isEmpty(); }

// Return the whole order list
LinkedList<OrderItem> OrderManager::getOrders() const { return orders; }

// Get a single order by index
OrderItem* OrderManager::getOrderAt(int index) { return orders.get(index); }

// Get number of orders
int OrderManager::getOrderCount() const { return orders.getSize(); }

// Save all orders to file
void OrderManager::saveAllOrders() {
    auto it = orders.begin();
    while (it.hasNext()) {
        OrderItem& order = it.next();
        string orderData = order.getOrderData();
        if (!orderData.empty()) {
            saveOrder(orderData);       // Write to file
        }
    }
}

// Clear all orders from memory
void OrderManager::clearOrders() { orders.clear(); }

// Display food menu
void OrderManager::displayFoodMenu(const LinkedList<Food>& menu) const {
    cout << "\n\t\t" << string(50, '=') << "\n";
    cout << "\t\t                   FOOD MENU\n";
    cout << "\t\t" << string(50, '=') << "\n";
    cout << left << setw(7) << "\t\tNO" << left << setw(31) << "ITEM NAME" << left << setw(10) << "PRICE" << endl;
    cout << "\t\t" << string(50, '-') << "\n";

    int i = 1;
    auto it = menu.begin();
    while (it.hasNext()) {
        cout << left << "\t\t" << setw(5) << i++;
        it.next().display();             // Call display() of Food object
    }
    cout << "\t\t" << string(50, '=') << "\n\n";
}

// Display beverage menu
void OrderManager::displayBeverageMenu(const LinkedList<Beverage>& menu) const {
    cout << "\n\t\t" << string(50, '=') << "\n";
    cout << "\t\t                 BEVERAGE MENU\n";
    cout << "\t\t" << string(50, '=') << "\n";
    cout << left << setw(7) << "\t\tNO" << left << setw(25) << "ITEM NAME"
         << left << setw(12) << "TYPE" << left << setw(20) << "PRICE" << endl;
    cout << "\t\t" << string(50, '-') << "\n";

    int i = 1;
    auto it = menu.begin();
    while (it.hasNext()) {
        cout << left << "\t\t" << setw(5) << i++;
        it.next().display();             // Call display() of Beverage object
    }
    cout << "\t\t" << string(50, '=') << "\n";
}
