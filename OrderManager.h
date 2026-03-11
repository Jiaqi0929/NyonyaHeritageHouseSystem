#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include "LinkedList.h"
#include "OrderItem.h"
#include "Utilities.h"
#include "Food.h"
#include "Beverage.h"

// Forward declaration - tells compiler OrderItem class exists (defined in OrderItem.h)
class OrderItem;

class OrderManager {
    LinkedList<OrderItem> orders;
    string currentCustomer;
    int currentReservationId;

    int findOrderIndex(const string& itemName);

public:
    OrderManager();

    // Set current customer for new orders
    void setCustomer(string customer);
    void setReservationId(int resId);

    void addOrder(MenuItem* item, int quantity);
    void editOrder(int index, int quantity);
    bool deleteOrder(int index);
    void displayOrders() const;
    void searchOrder(string name);
    void sortOrdersByPrice();
    float getTotal() const;
    bool hasOrders() const;
    LinkedList<OrderItem> getOrders() const;
    OrderItem* getOrderAt(int index);
    int getOrderCount() const;
    void saveAllOrders();
    void clearOrders();

    // Menu display functions
    void displayFoodMenu(const LinkedList<Food>& menu) const;
    void displayBeverageMenu(const LinkedList<Beverage>& menu) const;
};

#endif
