#include "Beverage.h"

// Displays beverage name and formatted price aligned in columns.
void Beverage::display() const {
    cout << left << setw(25) << name << "[" << type << "]  RM " << fixed << setprecision(2) << right << setw(8) << price << endl;
}
