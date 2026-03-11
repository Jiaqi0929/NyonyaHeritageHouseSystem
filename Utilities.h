#ifndef UTILITIES_H
#define UTILITIES_H

#include "Enums.h"
#include "LinkedList.h"
#include "HashMap.h"
#include "OrderManager.h"
#include "OrderItem.h"
#include "Customer.h"
#include "Enums.h"
#include <string>

using namespace std;

// File paths
#define CUSTOMERS_FILE "customers.txt"
#define RESERVATIONS_FILE "reservations.txt"
#define ORDERS_FILE "orders.txt"
#define MENU_FILE "menu.txt"

// Constants
#define SERVICE_TAX 0.10

// ==================== FORWARD DECLARATIONS ====================
// Forward declarations for main classes
class User;
class OrderManager;
class Customer;
class Food;
class Beverage;

// ==================== BASIC UTILITY FUNCTIONS ====================
void waitForEnter();
LinkedList<string> splitString(const string& str, char delimiter);
string toLowercase(const string& str);
string toLowerString(string s);
bool caseInsensitiveContains(const string& haystack, const string& needle);
bool isValidEmail(const string& email);
bool isValidPhone(const string& phone);
bool isValidPassword(const string& password);
bool isValidUsername(const string& username);
bool isValidOperatingTime(const string& time);
bool isValidDate(const string& date);
bool isValidTime(const string& time);
bool isValidNumPeople(const string& input);
bool isValidPIN(const string& pin);
bool isReservationIdUnique(int reservationId);
bool isAlphabetOnly(const string& str);
string getPassword();
void blinkMessage(string message);
string trim(const string& s);

// ==================== CONVERSION FUNCTIONS ====================
string statusToString(ReservationStatus status);
string orderStatusToString(OrderStatus status);
ReservationStatus intToStatus(int status);

// ==================== FILE HANDLING FUNCTIONS ====================
void saveCustomer(const string& data);
void saveReservation(const string& data);
void saveOrder(const string& data);
LinkedList<string> loadCustomers();
LinkedList<string> loadReservations();
LinkedList<string> loadOrders();
void clearReservationFile();
void clearOrderFile();
void clearCustomerFile();
void saveMenuToFile(const LinkedList<Food>& foodMenu, const LinkedList<Beverage>& bevMenu);
void loadMenuFromFile(LinkedList<Food>& foodMenu, LinkedList<Beverage>& bevMenu);

// ==================== LOGIN/SIGNUP FUNCTIONS ====================
User* login(bool isAdmin = false);
bool signUp(bool makeAdmin = false);

// ==================== CUSTOMER FUNCTIONS ====================
void viewCustomerReservations(const string& username);
void viewCustomerProfile(User* customer);

// ==================== ORDER MANAGEMENT ====================
void orderManagement(OrderManager& manager, LinkedList<Food>& foodMenuVec, LinkedList<Beverage>& bevMenuVec);
void checkout(OrderManager& manager, Customer& customer, bool hasReservation, string customerUsername);

// ==================== PAYMENT FUNCTIONS ====================
bool processOnlinePayment(float amount, const string& customerName);
bool processPayment(float totalAmount, string customerName);

// ==================== MENU DISPLAY FUNCTIONS ====================
void displayFoodMenu(const LinkedList<Food>& menu);
void displayBeverageMenu(const LinkedList<Beverage>& menu);
void adminMenu(OrderManager& manager,
               LinkedList<Food>& foodMenu,
               LinkedList<Beverage>& bevMenu);

// ==================== ADMIN FUNCTIONS ====================
void manageOrdersAdmin();
void manageReservationsAdmin();
void manageMenuAdmin(LinkedList<Food>& foodMenu, LinkedList<Beverage>& bevMenu);
void viewSalesReport(OrderManager& manager);
void manageCustomersAdmin();

#endif // UTILITIES_H
