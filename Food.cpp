#include "Food.h"

// Displays food name and formatted price aligned in columns
void Food::display() const {
    cout << left << setw(25) << name << "RM " << fixed << setprecision(2) << right << setw(8) << price << endl;
}
