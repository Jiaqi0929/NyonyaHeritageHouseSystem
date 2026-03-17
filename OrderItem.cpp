#include "OrderItem.h"
#include "Enums.h"

// Constructor with validation
OrderItem::OrderItem(MenuItem* i, int q, string customer, int resId)
    : item(i), quantity(q), customerUsername(customer), status(ORDER_PENDING), reservationId(resId) {
    // Validate quantity range
    if (q < 1) quantity = 1;
    if (q > 100) quantity = 100;

    // Generate timestamp
    time_t now = time(0);
    char buffer[80];
    ctime_s(buffer, sizeof(buffer), &now);
    timestamp = buffer;
    if (!timestamp.empty() && timestamp.back() == '\n') {
        timestamp.pop_back();
    }
}

// ==================== GETTER METHODS ====================
string OrderItem::getName() const { return item ? item->getName() : ""; }
float OrderItem::getPrice() const { return item ? item->getPrice() * quantity : 0; }
int OrderItem::getQuantity() const { return quantity; }
string OrderItem::getTimestamp() const { return timestamp; }
string OrderItem::getCustomerUsername() const { return customerUsername; }
OrderStatus OrderItem::getOrderStatus() const { return status; }
int OrderItem::getReservationId() const { return reservationId; }

// ==================== SETTER METHODS ====================
void OrderItem::setQuantity(int q) { quantity = q; }
void OrderItem::setOrderStatus(OrderStatus s) { status = s; }
void OrderItem::setReservationId(int id) { reservationId = id; }

// Display order item details
void OrderItem::display() const {
    if (item) {
        cout << left << setw(3) << quantity << " x " << left << setw(25) << item->getName()
            << "RM " << fixed << setprecision(2) << right << setw(8) << getPrice()
            << " [" << timestamp << "]" << endl;
    }
}

// Convert to string format for file storage
string OrderItem::getOrderData() const {
    if (item) {
        return item->getName() + "|" + to_string(item->getPrice()) + "|" +
            to_string(quantity) + "|" + timestamp + "|" +
            customerUsername + "|" + to_string((int)status) + "|" +
            to_string(reservationId);
    }
    return "";
}
