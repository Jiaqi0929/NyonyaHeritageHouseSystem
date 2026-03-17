#ifndef BEVERAGE_H
#define BEVERAGE_H

#include "MenuItem.h"

// ==================== BEVERAGE ITEM ====================
// Derived class for beverage items with temperature type
class Beverage : public MenuItem {
    char type;
public:
    Beverage(string n = "", float p = 0.0, char t = 'H') : MenuItem(n, p), type(t) {}

    // Display beverage item with type indicator
    void display() const override;
    string getType() const override { return "Beverage"; }

    // Beverage-specific methods
    char getBeverageType() const { return type; }
    void setBeverageType(char t) { type = t; }
};

#endif
