#ifndef ORDERITEM_H
#define ORDERITEM_H

#include "Enums.h"
#include "Utilities.h"
#include "MenuItem.h"
#include <ctime>

// ==================== ORDER ITEM ====================
// Represents a single item in an order with quantity and status
class OrderItem {
    MenuItem* item;
    int quantity;
    string timestamp;
    string customerUsername;
    OrderStatus status;
    int reservationId;

public:
    // Constructor with validation
    OrderItem(MenuItem* i = nullptr, int q = 0, string customer = "", int resId = 0);

    // ==================== GETTER METHODS ====================
    string getName() const;
    float getPrice() const;
    int getQuantity() const;
    string getTimestamp() const;
    string getCustomerUsername() const;
    OrderStatus getOrderStatus() const;
    int getReservationId() const;

    // ==================== SETTER METHODS ====================
    void setQuantity(int q);
    void setOrderStatus(OrderStatus s);
    void setReservationId(int id);

    // Display order item details
    void display() const;

    // Convert to string format for file storage
    string getOrderData() const;
};

#endif
