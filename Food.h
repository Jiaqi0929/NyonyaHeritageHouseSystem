#ifndef FOOD_H
#define FOOD_H

#include "MenuItem.h"

// ==================== FOOD ITEM ====================
// Derived class for food items
class Food : public MenuItem {
public:
    Food(string n = "", float p = 0.0) : MenuItem(n, p) {}

    // Display food item details
    void display() const override;
    string getType() const override { return "Food"; }
};

#endif
