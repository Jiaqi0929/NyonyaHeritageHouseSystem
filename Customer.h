#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "Enums.h"
#include "User.h"
#include <string>
using namespace std;

// ==================== CUSTOMER CLASS ====================
// Represents a customer making reservations
class Customer {
    string name, contact, date, time, remarks;
    string username;
    int num_people;
    ReservationStatus status;
    int reservationId;

public:
    // Default constructor
    Customer();

    // Constructor with user information
    Customer(User* user);

    // Input booking details from user
    void inputBookingDetails(User* user = nullptr);

    // Display booking details
    void displayBooking() const;

    // Convert to string format for file storage
    string getBookingData() const;

    // Getters
    string getUsername() const;
    int getReservationId() const;
    string getName() const;
    string getContact() const;
    string getDate() const;
    string getTime() const;
    int getNumPeople() const;
    string getRemarks() const;
    ReservationStatus getStatus() const;

    // Setters
    void setReservationId(int id);
    void setName(const string& n);
    void setContact(const string& c);
    void setDate(const string& d);
    void setTime(const string& t);
    void setNumPeople(int np);
    void setRemarks(const string& r);
    void setStatus(ReservationStatus s);
    void setStatusFromInt(int s);
};

#endif
