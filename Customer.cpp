#include "Customer.h"
#include "Utilities.h"
#include "Enums.h"
#include <iostream>
#include <iomanip>
#include <limits>
using namespace std;

// Default constructor
Customer::Customer() : status(PENDING), reservationId(0) {}

// Constructor with user information
Customer::Customer(User* user) : status(PENDING), reservationId(0) {
    if (user) {
        username = user->getUsername();
        name = user->getFullName();
        contact = user->getPhone();
    }
}

// Input booking details from user
void Customer::inputBookingDetails(User* user) {
    cin.ignore();

    // Pre-fill with user info if available
    if (user && !user->getFullName().empty()) {
        name = user->getFullName();
        cout << "\t\tName: " << name << endl;
    } else {
        cout << "\n\t\tName: ";
        getline(cin, name);
    }

    // Contact with validation
    if (user && !user->getPhone().empty()) {
        contact = user->getPhone();
        cout << "\t\tContact number: " << contact << endl;
    } else {
        cout << "\t\tContact number: ";
        getline(cin, contact);
        while (!isValidPhone(contact)) {
            cout << "\t\tInvalid phone number! Must be 10-12 digits starting with 01\n";
            cout << "\t\tEnter valid contact number: ";
            getline(cin, contact);
        }
    }

    // Date with validation
    cout << "\t\tBooking date (ddmmyy): ";
    cin >> date;
    while (!isValidDate(date)) {
        cout << "\t\tEnter valid date as ddmmyy: ";
        cin >> date;
    }

    // Time with validation
    cout << "\t\tBooking time (HHMM): ";
    cin >> time;
    while (!isValidOperatingTime(time)) {
        cout << "\t\tEnter valid time as HHMM: ";
        cin >> time;
    }

    // Number of people with validation
    cout << "\t\tNumber of people (1-25): ";
    string peopleInput;
    while (true) {
        cin >> peopleInput;

        // Check if input contains only digits
        bool isNumeric = true;
        for (char c : peopleInput) {
            if (!isdigit(c)) {
                isNumeric = false;
                break;
            }
        }

        if (!isNumeric) {
            cout << "\t\tInvalid input! Please enter a number between 1-25.\n";
            cout << "\t\tNumber of people (1-25): ";
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
            continue;
        }

        try {
            num_people = stoi(peopleInput);

            // Check range (1-25 as requested)
            if (num_people >= 1 && num_people <= 25) {
                break; // Valid input, exit loop
            } else {
                cout << "\t\tNumber must be between 1 and 25. Please try again!\n";
                cout << "\t\tNumber of people (1-25): ";
            }
        } catch (...) {
            cout << "\t\tInvalid input! Please enter a number between 1-25.\n";
            cout << "\t\tNumber of people (1-25): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // Optional remarks
    cin.ignore();
    cout << "\t\tRemarks: ";
    getline(cin, remarks);

    status = PENDING;

    // Generate unique reservation ID if not set
    if (reservationId == 0) {
        reservationId = rand() % 10000 + 1000;
    }
}

// Display booking details
void Customer::displayBooking() const {
    cout << "\n\t\tBooking Details:\n";
    cout << "\t\t" << string(40, '-') << "\n";
    cout << left << setw(15) << "\t\tReservation ID:" << reservationId << endl;
    cout << left << setw(15) << "\t\tName:" << name << endl;
    cout << left << setw(15) << "\t\tContact:" << contact << endl;
    cout << left << setw(15) << "\t\tDate:" << date << endl;
    cout << left << setw(15) << "\t\tTime:" << time << endl;
    cout << left << setw(15) << "\t\tPeople:" << num_people << endl;
    cout << left << setw(15) << "\t\tStatus:" << statusToString(status) << endl;
    cout << left << setw(15) << "\t\tRemarks:" << remarks << endl;
    cout << "\t\t" << string(40, '-') << "\n";
}

// Convert to string format for file storage
string Customer::getBookingData() const {
    return to_string(reservationId) + "|" + username + "|" + name + "|" + contact + "|" + date + "|" +
           time + "|" + to_string(num_people) + "|" + to_string((int)status) + "|" + remarks;
}

// ==================== GETTER METHODS ====================
string Customer::getUsername() const { return username; }
int Customer::getReservationId() const { return reservationId; }
string Customer::getName() const { return name; }
string Customer::getContact() const { return contact; }
string Customer::getDate() const { return date; }
string Customer::getTime() const { return time; }
int Customer::getNumPeople() const { return num_people; }
string Customer::getRemarks() const { return remarks; }
ReservationStatus Customer::getStatus() const { return status; }

// ==================== SETTER METHODS ====================
void Customer::setReservationId(int id) { reservationId = id; }
void Customer::setName(const string& n) { name = n; }
void Customer::setContact(const string& c) { contact = c; }
void Customer::setDate(const string& d) { date = d; }
void Customer::setTime(const string& t) { time = t; }
void Customer::setNumPeople(int np) { num_people = np; }
void Customer::setRemarks(const string& r) { remarks = r; }
void Customer::setStatus(ReservationStatus s) { status = s; }
void Customer::setStatusFromInt(int s) { status = intToStatus(s); }
