#ifndef MENUITEM_H
#define MENUITEM_H

#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

// ==================== ABSTRACT MENU ITEM ====================
// Abstract base class for all menu items
class MenuItem {
protected:
    string name;
    float price;

public:
    MenuItem(string n = "", float p = 0.0) : name(n), price(p) {}

    // Pure virtual function
    virtual void display() const = 0;

    // Common getters
    string getName() const { return name; }
    float getPrice() const { return price; }
    virtual string getType() const = 0;

    // Setters
    void setName(string n) { name = n; }
    void setPrice(float p) { price = p; }
    virtual ~MenuItem() {}
};

#endif
