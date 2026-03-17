#include "Utilities.h"
#include "User.h"
#include "Food.h"
#include "Beverage.h"
#include "OrderItem.h"
#include "OrderManager.h"
#include "Queue.h"
#include "UserManager.h"
#include "Customer.h"
#include "Enums.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <limits>
#include <cctype>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

// ==================== CROSS-PLATFORM PAUSE FUNCTION ====================
// Waits for Enter key press to continue execution
void waitForEnter() {
    cout << "\n\t\tPress Enter to continue...";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// ==================== STRING SPLITTING FUNCTION ====================
// Splits a string by delimiter and returns tokens as LinkedList
LinkedList<string> splitString(const string& str, char delimiter) {
    LinkedList<string> tokens;
    if (str.empty()) return tokens;

    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {
            tokens.insertAtEnd(token);
        }
    }
    return tokens;
}

// ==================== CASE-INSENSITIVE SEARCH HELPER ====================
// Convert string to lowercase for case-insensitive comparison
string toLowercase(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Check if string contains substring
bool caseInsensitiveContains(const string& haystack, const string& needle) {
    string haystackLower = toLowercase(haystack);
    string needleLower = toLowercase(needle);
    return haystackLower.find(needleLower) != string::npos;
}

// ==================== VALIDATION FUNCTIONS ====================
// Validate email format
bool isValidEmail(const string& email) {
    if (email.empty()) return false;
    size_t atPos = email.find('@');
    if (atPos == string::npos || atPos == 0) return false;
    size_t dotPos = email.find('.', atPos);
    if (dotPos == string::npos || dotPos <= atPos + 1) return false;
    if (dotPos == email.length() - 1) return false;
    return true;
}

// Validate phone number format
bool isValidPhone(const string& phone) {
    if (phone.length() < 10 || phone.length() > 12) return false;
    for (char c : phone) {
        if (!isdigit(c)) return false;
    }
    if (phone.substr(0, 2) != "01") return false;
    return true;
}

// Validate password strength
bool isValidPassword(const string& password) {
    if (password.length() < 6) {
        cout << "\t\tPassword must be at least 6 characters long.\n";
        cout << "\t\tPlease try again with a stronger password.\n";
        return false;
    }

    bool hasDigit = false, hasLetter = false;
    for (char c : password) {
        if (isdigit(c)) hasDigit = true;
        if (isalpha(c)) hasLetter = true;
    }

    if (!hasDigit || !hasLetter) {
        cout << "\t\tPassword must contain both letters and numbers.\n";
        cout << "\t\tPlease try again with a stronger password.\n";
        return false;
    }
    return true;
}

// Validate username format and uniqueness
bool isValidUsername(const string& username) {
    if (username.length() < 3 || username.length() > 20) {
        cout << "\t\tUsername must be 3-20 characters.\n";
        return false;
    }

    // Only allow alphanumeric and underscores
    for (char c : username) {
        if (!isalnum(c) && c != '_') {
            cout << "\t\tUsername can only contain letters, numbers, and underscores.\n";
            return false;
        }
    }

    // Check for reserved usernames
    string usernameLower = toLowercase(username);
    if (usernameLower == "admin" ||
        usernameLower == "system" ||
        usernameLower == "root" ||
        usernameLower == "guest") {
        cout << "\t\tThis username is reserved. Please choose another.\n";
        return false;
    }
    return true;
}

// Validate time is within restaurant operating hours
bool isValidOperatingTime(const string& time) {
    // Check basic format first (HHMM)
    if (time.length() != 4) {
        cout << "\t\tTime must be 4 digits (HHMM format).\n";
        return false;
    }

    // Must be all digits
    for (char c : time) {
        if (!isdigit(c)) {
            cout << "\t\tTime must contain only digits.\n";
            return false;
        }
    }

    // Extract hour and minute
    int hour = stoi(time.substr(0, 2));
    int minute = stoi(time.substr(2, 2));

    // Check valid time values
    if (hour < 0 || hour > 23) {
        cout << "\t\tHour must be between 00 and 23.\n";
        return false;
    }

    if (minute < 0 || minute > 59) {
        cout << "\t\tMinute must be between 00 and 59.\n";
        return false;
    }

    // Convert to minutes since midnight for easier comparison
    int totalMinutes = hour * 60 + minute;

    // Restaurant operates from 11:00 AM (1100) to 9:00 PM (2100)
    int openTime = 11 * 60;          // 11:00 AM = 660 minutes
    int closeTime = 21 * 60;         // 9:00 PM = 1260 minutes

    // Check if time is within operating hours
    if (totalMinutes < openTime || totalMinutes > closeTime) {
        cout << "\t\tInvalid time! Restaurant operates from 11:00 AM to 9:00 PM.\n";
        return false;
    }

    return true;
}

// Validate date format (DDMMYY)
bool isValidDate(const string& date) {
    if (date.length() != 6) return false;
    for (char c : date) {
        if (!isdigit(c)) return false;
    }
    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(2, 2));
    int year = stoi(date.substr(4, 2)) + 2000;

    // Days in each month
    if (month < 1 || month > 12) return false;
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Handle leap year for February
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (isLeap) daysInMonth[1] = 29;
    }

    return (day >= 1 && day <= daysInMonth[month - 1]);
}

// Validate time format (HHMM)
bool isValidTime(const string& time) {
    if (time.length() != 4) return false;
    for (char c : time) {
        if (!isdigit(c)) return false;
    }
    int hour = stoi(time.substr(0, 2));
    int minute = stoi(time.substr(2, 2));
    return (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59);
}

// Validate number of people for reservation
bool isValidNumPeople(const string& input) {
    if (input.empty()) return false;

    for (char c : input) {
        if (!isdigit(c)) return false;
    }

    int num = stoi(input);
    if (num < 1 || num > 25) return false;

    return true;
}

// Validate pin number
bool isValidPIN(const string& pin) {
    if (pin.length() != 6) {
        cout << "\t\tPIN must be 6 digits.\n";
        return false;
    }

    for (char c : pin) {
        if (!isdigit(c)) {
            cout << "\t\tPIN must contain only digits.\n";
            return false;
        }
    }
    return true;
}

// Validate the uniqueness of the reservation id
bool isReservationIdUnique(int reservationId) {
    LinkedList<string> allReservations = loadReservations();
    auto it = allReservations.begin();
    while (it.hasNext()) {
        string reservation = it.next();
        LinkedList<string> tokens = splitString(reservation, '|');
        if (tokens.getSize() >= 1) {
            try {
                int existingId = stoi(*tokens.get(0));
                if (existingId == reservationId) {
                    return false;
                }
            } catch (...) {}
        }
    }
    return true;
}

//Function to check whether a string contains only alphabet letters and spaces
bool isAlphabetOnly(const string& str) {
    for (char c : str) {
        if (!isalpha(c) && c != ' ') {
            return false;
        }
    }
    return true;
}

//Function to convert all characters in a string to lwoercase
string toLowerString(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

//Function to remove leading and trailing spaces or tab characters from a string
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

// ==================== PASSWORD MASKING ====================
// Get password input with asterisk masking
string getPassword() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!password.empty()) {
                cout << "\b \b";
                password.pop_back();
            }
        } else if (ch >= 32 && ch <= 126) {
            cout << '*';
            password += ch;
        }
    }
    cout << endl;
    return password;
}

// ==================== BLINKING FUNCTION ====================
// Visual effect for important messages
void blinkMessage(string message) {
    const int blinkDuration = 500;
    int blinkCount = 2;
    for (int i = 0; i < blinkCount; i++) {
        system("cls");
        Sleep(blinkDuration);
        cout << message << endl;
        Sleep(blinkDuration);
    }
}

// ==================== CONVERSION FUNCTIONS IMPLEMENTATION ====================
// Convert enum value to human-readable string
string statusToString(ReservationStatus status) {
    switch (status) {
        case PENDING: return "Pending";
        case CONFIRMED: return "Confirmed";
        case SEATED: return "Seated";
        case COMPLETED: return "Completed";
        case CANCELLED: return "Cancelled";
        default: return "Unknown";
    }
}

//Function to convert an OrderStatus enum value to a readable string
string orderStatusToString(OrderStatus status) {
    switch (status) {
        case ORDER_PENDING: return "Pending";
        case ORDER_PREPARING: return "Preparing";
        case ORDER_READY: return "Ready";
        case ORDER_SERVED: return "Served";
        case ORDER_CANCELLED: return "Cancelled";
        default: return "Unknown";
    }
}

// Convert integer to enum value
ReservationStatus intToStatus(int status) {
    switch (status) {
        case 0: return PENDING;
        case 1: return CONFIRMED;
        case 2: return SEATED;
        case 3: return COMPLETED;
        case 4: return CANCELLED;
        default: return PENDING;
    }
}


// ==================== FILE HANDLING FUNCTIONS ====================
// Save customer data to file
void saveCustomer(const string& data) {
    ofstream file(CUSTOMERS_FILE, ios::app);
    if (file.is_open()) {
        file << data << endl;
        file.close();
    }
}

// Save reservation data to file
void saveReservation(const string& data) {
    ofstream file(RESERVATIONS_FILE, ios::app);
    if (file.is_open()) {
        file << data << endl;
        file.close();
    }
}

// Save order data to file
void saveOrder(const string& data) {
    ofstream file(ORDERS_FILE, ios::app);
    if (file.is_open()) {
        file << data << endl;
        file.close();
    }
}

// Load all customers from file into LinkedList
LinkedList<string> loadCustomers() {
    LinkedList<string> customers;
    ifstream file(CUSTOMERS_FILE);
    if (!file.is_open()) {
        return customers;
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            customers.insertAtEnd(line);
        }
    }
    file.close();
    return customers;
}

// Load all menu from file into LinkedList
void loadMenuFromFile(LinkedList<Food>& foodMenu, LinkedList<Beverage>& bevMenu) {
    ifstream file(MENU_FILE);
    if (!file.is_open()) return;

    string line;
    string section = "";

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (line == "[FOOD]") {
            section = "FOOD";
            continue;
        } else if (line == "[BEVERAGE]") {
            section = "BEVERAGE";
            continue;
        }

        LinkedList<string> tokens = splitString(line, '|');

        if (section == "FOOD" && tokens.getSize() >= 3) {
            try {
                string name = *tokens.get(0);
                float price = stof(*tokens.get(1));
                foodMenu.insertAtEnd(Food(name, price));
            } catch (...) {
            }
        } else if (section == "BEVERAGE" && tokens.getSize() >= 4) {
            try {
                string name = *tokens.get(0);
                float price = stof(*tokens.get(1));
                char type = (*tokens.get(2))[0];
                bevMenu.insertAtEnd(Beverage(name, price, type));
            } catch (...) {
            }
        }
    }
    file.close();
}

// Load all reservations from file into LinkedList
LinkedList<string> loadReservations() {
    LinkedList<string> reservations;
    ifstream file(RESERVATIONS_FILE);
    if (!file.is_open()) {
        return reservations;
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            reservations.insertAtEnd(line);
        }
    }
    file.close();
    return reservations;
}

// Load all orders from file into LinkedList
LinkedList<string> loadOrders() {
    LinkedList<string> orders;
    ifstream file(ORDERS_FILE);
    if (!file.is_open()) {
        return orders;
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            orders.insertAtEnd(line);
        }
    }
    file.close();
    return orders;
}

// ==================== FILE CLEARING FUNCTIONS ====================
// Clear reservation file
void clearReservationFile() {
    ofstream file(RESERVATIONS_FILE, ios::trunc);
    file.close();
}

// Clear order file
void clearOrderFile() {
    ofstream file(ORDERS_FILE, ios::trunc);
    file.close();
}

// Clear customer file
void clearCustomerFile() {
    ofstream file(CUSTOMERS_FILE, ios::trunc);
    file.close();
}

void saveMenuToFile(const LinkedList<Food>& foodMenu, const LinkedList<Beverage>& bevMenu) {
    ofstream file(MENU_FILE);
    if (file.is_open()) {
        file << "[FOOD]" << endl;
        auto foodIt = foodMenu.begin();
        while (foodIt.hasNext()) {
            const Food& food = foodIt.next();
            file << food.getName() << "|" << fixed << setprecision(2) << food.getPrice() << "|F" << endl;
        }

        file << "[BEVERAGE]" << endl;
        auto bevIt = bevMenu.begin();
        while (bevIt.hasNext()) {
            const Beverage& bev = bevIt.next();
            file << bev.getName() << "|" << fixed << setprecision(2) << bev.getPrice()
                 << "|" << bev.getBeverageType() << "|B" << endl;
        }
        file.close();
    }
}

// ==================== LOGIN/SIGNUP FUNCTIONS ====================
// Login function with optional admin check
User* login(bool isAdmin) {
    string u, p;

    cout << "\t\tUsername: ";
    getline(cin, u);
    cout << "\t\tPassword: ";
    p = getPassword();

    // Linear search for matching user
    auto users = getGlobalUsers();
    auto it = users->begin();
    while (it.hasNext()) {
        User& user = it.next();
        if (user.getUsername() == u && user.checkPassword(p)) {
            if (isAdmin && !user.getIsAdmin()) {
                cout << "\t\tAccess denied! This account is not an admin.\n";
                return nullptr;
            }
            cout << "\t\tLogin successful!\n";
            return &user;
        }
    }
    cout << "\t\tInvalid username or password.\n";
    return nullptr;
}

// Sign up function for new users
bool signUp(bool makeAdmin) {
    string u, p, name, email, phone;

    auto users = getGlobalUsers();

    // Get and validate username
    bool validUsername = false;
    do {
        cout << "\t\tEnter new username: ";
        getline(cin, u);

        // Check length
        if (u.length() < 3 || u.length() > 20) {
            cout << "\t\tUsername must be 3-20 characters.\n";
            continue;
        }

        // Check if contains only letters, numbers, and underscores
        bool validChars = true;
        for (char c : u) {
            if (!isalnum(c) && c != '_') {
                validChars = false;
                break;
            }
        }
        if (!validChars) {
            cout << "\t\tUsername can only contain letters, numbers, and underscores.\n";
            continue;
        }

        // Check if starts with letter
        if (!isalpha(u[0])) {
            cout << "\t\tUsername must start with a letter.\n";
            continue;
        }

        // Check if reserved username
        if (toLowercase(u) == "admin") {
            cout << "\t\tThis username is reserved. Please choose another.\n";
            continue;
        }

        // Check if username already exists
        auto it = users->begin();
        bool usernameExists = false;
        while (it.hasNext()) {
            User& user = it.next();
            if (toLowercase(user.getUsername()) == toLowercase(u)) {
                usernameExists = true;
                break;
            }
        }

        if (usernameExists) {
            cout << "\t\tUsername already exists!\n";
        } else {
            validUsername = true;
        }
    } while (!validUsername);

    // Get and validate password
    bool validPassword = false;
    do {
        cout << "\t\tEnter new password: ";
        p = getPassword();

        if (p.empty()) {
            cout << "\t\tPassword cannot be empty!\n";
            continue;
        }

        if (isValidPassword(p)) {
            validPassword = true;
        }
    } while (!validPassword);

    // Profile Information
    if (!makeAdmin) {
        cout << "\t\tEnter full name: ";
        getline(cin, name);
        if (name.empty()) {
            cout << "\t\tFull name cannot be empty!\n";
            return false;
        }

        // Email validation loop
        do {
            cout << "\t\tEnter email: ";
            getline(cin, email);
            if (!isValidEmail(email)) {
                cout << "\t\tInvalid email format! Must contain '@' and '.' (e.g., user@example.com)\n";
            }
        } while (!isValidEmail(email));

        // Phone validation loop
        do {
            cout << "\t\tEnter phone (10-12 digits, starting with 01): ";
            getline(cin, phone);
            if (!isValidPhone(phone)) {
                cout << "\t\tInvalid phone number! Must be 10-12 digits starting with 01\n";
            }
        } while (!isValidPhone(phone));
    }

    // Create and add new user
    User newUser(u, p, makeAdmin, name, email, phone, true);
    users->insertAtEnd(newUser);

    // Save to file if not admin
    if (!makeAdmin) {
        string customerData = u + "|" + p + "|" + name + "|" + email + "|" + phone + "|0|1";
        saveCustomer(customerData);
        cout << "\t\tCustomer data saved to file.\n";
    } else {
        cout << "\t\tAdmin account created in memory only.\n";
    }

    cout << "\t\tSign up successful! You can now login.\n";
    return true;
}

// ==================== VIEW CUSTOMER RESERVATIONS ====================
void viewCustomerReservations(const string& username) {
    system("cls");
    cout << "\n\t\t" << string(60, '=') << "\n";
    cout << "\t\t                     YOUR RESERVATIONS\n";
    cout << "\t\t" << string(60, '=') << "\n";

    //load all reservations from file
    LinkedList<string> allReservations = loadReservations();
    bool found = false;

    //HashMap to store the latest reservation for each reservation ID
    HashMap<int, string> latestReservations;

    //Fist pass: Identify reservations that belong to the logged in user
    auto it = allReservations.begin();
    while (it.hasNext()) {
        string reservation = it.next();
        LinkedList<string> tokens = splitString(reservation, '|');

        if (tokens.getSize() >= 8 && *tokens.get(1) == username) {
            found = true;
            try {
                int resId = stoi(*tokens.get(0));
                latestReservations.insert(resId, reservation);
            } catch (...) {}
        }
    }

    //If no reservations are found for the user
    if (!found) {
        cout << "\n\t\tNo reservations found for your account.\n";
        cout << "\t\tYou can make a reservation from the main menu.\n";
    } else {
        int count = 1;

        //List to track reservation IDs already displayed
        LinkedList<int> seenIds;

        //Second pass: Display unique reservations for the user
        auto displayIt = allReservations.begin();
        while (displayIt.hasNext()) {
            string reservation = displayIt.next();
            LinkedList<string> tokens = splitString(reservation, '|');

            //Check if reservation belongs to the user
            if (tokens.getSize() >= 8 && *tokens.get(1) == username) {
                try {
                    int resId = stoi(*tokens.get(0));

                    //check if this reservation ID has already been displayed
                    bool alreadySeen = false;
                    auto idIt = seenIds.begin();
                    while (idIt.hasNext()) {
                        if (idIt.next() == resId) {
                            alreadySeen = true;
                            break;
                        }
                    }

                    //Display reservation if it has not been shown before
                    if (!alreadySeen) {
                        seenIds.insertAtEnd(resId);

                        cout << "\n\t\t\tReservation " << count++ << ":\n";
                        cout << "\t\t\t" << string(40, '-') << "\n";
                        cout << left << setw(15) << "\t\t\tReservation ID:" << *tokens.get(0) << endl;
                        cout << left << setw(15) << "\t\t\tName:" << *tokens.get(2) << endl;
                        cout << left << setw(15) << "\t\t\tContact:" << *tokens.get(3) << endl;
                        cout << left << setw(15) << "\t\t\tDate:" << *tokens.get(4) << endl;
                        cout << left << setw(15) << "\t\t\tTime:" << *tokens.get(5) << endl;
                        cout << left << setw(15) << "\t\t\tPeople:" << *tokens.get(6) << endl;

                        //Convert reservation status from integer to readable string
                        try {
                            int statusInt = stoi(*tokens.get(7));
                            cout << left << setw(15) << "\t\t\tStatus:" << statusToString(intToStatus(statusInt)) << endl;
                        } catch (...) {
                            cout << left << setw(15) << "\t\t\tStatus:" << "Unknown" << endl;
                        }

                        //Display remarks if available
                        if (tokens.getSize() > 8) {
                            cout << left << setw(15) << "\t\t\tRemarks:" << *tokens.get(8) << endl;
                        }
                        cout << "\t\t\t" << string(40, '-') << "\n";
                    }
                } catch (...) {}
            }
        }
    }

    cout << "\n\t\t" <<  string(60, '=') << "\n";
    waitForEnter();
}

// ==================== CUSTOMER PROFILE ====================
void viewCustomerProfile(User* customer) {
    system("cls");
    cout << "\n\t\t" << string(60, '=') << "\n";
    cout << "\t\t                    CUSTOMER PROFILE\n";
    cout << "\t\t" << string(60, '=') << "\n";
    cout << left << setw(15) << "\t\tUsername     :" << customer->getUsername() << endl;
    cout << left << setw(15) << "\t\tFull Name    :" << (customer->getFullName().empty() ? "Not set" : customer->getFullName()) << endl;
    cout << left << setw(15) << "\t\tEmail        :" << (customer->getEmail().empty() ? "Not set" : customer->getEmail()) << endl;
    cout << left << setw(15) << "\t\tPhone        :" << (customer->getPhone().empty() ? "Not set" : customer->getPhone()) << endl;
    cout << left << setw(15) << "\t\tAccount Type :" << (customer->getIsAdmin() ? "Admin" : "Customer") << endl;
    cout << "\t\t" << string(60, '=') << "\n";

    //Load all orders from file
    LinkedList<string> orders = loadOrders();
    int customerOrderCount = 0;
    float totalSpent = 0;

    ////Loop through all orders to find those belonging to this customer
    auto orderIt = orders.begin();
    while (orderIt.hasNext()) {
        string order = orderIt.next();
        LinkedList<string> tokens = splitString(order, '|');
        if (tokens.getSize() >= 6 && *tokens.get(4) == customer->getUsername()) {
            customerOrderCount++;
            try {
                float price = stof(*tokens.get(1));
                int qty = stoi(*tokens.get(2));
                totalSpent += price * qty;
            } catch (...) {}
        }
    }

    cout << left << setw(15) << "\t\tTotal Orders:" << customerOrderCount << endl;
    cout << left << setw(15) << "\t\tTotal Spent:" << "RM " << fixed << setprecision(2) << totalSpent << endl;

    cout << "\t\t" << string(60, '=') << "\n";

    cout << "\n\t\tNote: You can view your reservations in the 'View My ";
    cout << "\n\t\t      Reservations' option from the main menu.\n";

    waitForEnter();
}

// Get current timestamp as formatted string
string getCurrentTimestamp() {
    time_t now = time(0);
    char buffer[80];
    struct tm timeinfo;

    // Use localtime_s for Windows (safe version)
    localtime_s(&timeinfo, &now);

    // Format: YYYY-MM-DD HH:MM:SS
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return string(buffer);
}

// Get current date only
string getCurrentDate() {
    time_t now = time(0);
    char buffer[80];
    struct tm timeinfo;

    localtime_s(&timeinfo, &now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);
    return string(buffer);
}

// Get current time only
string getCurrentTime() {
    time_t now = time(0);
    char buffer[80];
    struct tm timeinfo;

    localtime_s(&timeinfo, &now);
    strftime(buffer, sizeof(buffer), "%H:%M:%S", &timeinfo);
    return string(buffer);
}
// ==================== ORDER MANAGEMENT ====================
void orderManagement(OrderManager& manager, LinkedList<Food>& foodMenuVec, LinkedList<Beverage>& bevMenuVec) {
    char c;

    //store order notifications (persists between calls)
    static Queue<string> orderNotifications;

    //loop until user choose to go back to main menu
    do {
        system("cls");
        cout << "\n\t\t" << string(40, '=') << "\n";
        cout << "\t\t           ORDER MANAGEMENT\n";
        cout << "\t\t      Notifications in queue: " << orderNotifications.getSize() << "\n";
        cout << "\t\t" << string(40, '=') << "\n";
        cout << "\t\t1. Add Order\n\t\t2. Edit Order\n\t\t3. Delete Order\n\t\t4. View Orders\n";
        cout << "\t\t5. Search Order\n\t\t6. Sort Orders\n\t\t7. View Notifications\n\t\t0. Back to Main Menu\n";
        cout << "\t\t" << string(40, '=') << "\n";
        cout << "\t\tEnter option: ";
        cin >> c;

        switch (c) {
            case '1': {
                system("cls");
                int typeChoice, itemNum, qty;
                cout << "\n\t\t" << string(40, '=') << "\n";
                cout << "\t\t          ADD NEW ORDER\n";
                cout << "\t\t" << string(40, '=') << "\n";
                cout << "\t\t1. Food\n\t\t2. Beverage\n";
                cout << "\t\t" << string(40, '=') << "\n";
                cout << "\n\t\tChoose type: ";
                cin >> typeChoice;

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                system("cls");

                //Food order
                if (typeChoice == 1) {

                    //check if food menu is empty
                    if (foodMenuVec.isEmpty()) {
                        cout << "\t\tNo food items available!\n";
                        break;
                    }

                    //Display food menu
                    manager.displayFoodMenu(foodMenuVec);
                    cout << "\t\tSelect food item (1-" << foodMenuVec.getSize() << "): ";

                    string itemInput;
                    getline(cin, itemInput);

                    //validate numeric input
                    if (itemInput.empty() || !all_of(itemInput.begin(), itemInput.end(), ::isdigit)) {
                        cout << "\n\t\tInvalid input! Please enter a number.\n";
                        waitForEnter();
                        break;
                    }
                    itemNum = stoi(itemInput);

                    //validate item selection
                    if (itemNum >= 1 && itemNum <= foodMenuVec.getSize()) {
                        Food* foodItem = foodMenuVec.get(itemNum - 1);
                        if (foodItem) {
                            do {
                            cout << "\t\tQuantity (1-100): ";

                            string qtyInput;
                            getline(cin, qtyInput);

                            // Check if input is empty or contains non-digits
                            if (qtyInput.empty() || !all_of(qtyInput.begin(), qtyInput.end(), ::isdigit)) {
                                cout << "\n\t\tInvalid quantity! Please enter a number between 1 and 100.\n";
                                qty = -1; // Force re-prompt
                            continue;
                            }

                            try {
                                qty = stoi(qtyInput);
                            } catch (...) {
                                cout << "\n\t\tInvalid quantity! Please enter a number between 1 and 100.\n";
                                qty = -1;
                                continue;
                            }

                            if (qty < 1 || qty > 100) {
                                cout << "\n\t\tQuantity must be between 1 and 100!\n";
                            }
                            } while (qty < 1 || qty > 100);

                                manager.addOrder(foodItem, qty);

                                //create and enqueue notification
                                string notification = "Order added: " + foodItem->getName() +
                                                    " x" + to_string(qty) +
                                                    " at " + to_string(time(0) % 10000);
                                orderNotifications.enqueue(notification);

                                cout << "\t\tAdded to notification queue (Position: "
                                     << orderNotifications.getSize() << ")\n";
                            } else {
                                cout << "\n\t\tInvalid selection!\n";
                            }
                        } else {
                            cout << "\n\t\tInvalid selection!\n";
                        }

                //Beverage order
                } else if (typeChoice == 2) {
                    if (bevMenuVec.isEmpty()) {
                        cout << "\t\tNo beverage items available!\n";
                        break;
                    }
                    manager.displayBeverageMenu(bevMenuVec);
                    cout << "\n\t\tSelect beverage item (1-" << bevMenuVec.getSize() << "): ";

                    string itemInput;
                    getline(cin, itemInput);
                    if (itemInput.empty() || !all_of(itemInput.begin(), itemInput.end(), ::isdigit)) {
                    cout << "\n\t\tInvalid input! Please enter a number.\n";
                        waitForEnter();
                        break;
                    }
                    itemNum = stoi(itemInput);

                    //Validate numeric input
                    if (itemNum >= 1 && itemNum <= bevMenuVec.getSize()) {
                        Beverage* bevItem = bevMenuVec.get(itemNum - 1);
                        if (bevItem) {
                            do {
                                cout << "\t\tQuantity (1-100): ";

                                string qtyInput;
                                getline(cin, qtyInput);

                                // Check if input is empty or contains non-digits
                                if (qtyInput.empty() || !all_of(qtyInput.begin(), qtyInput.end(), ::isdigit)) {
                                    cout << "\n\t\tInvalid quantity! Please enter a number between 1 and 100.\n";
                                    qty = -1;
                                    continue;
                                }

                                try {
                                    qty = stoi(qtyInput);
                                } catch (...) {
                                    cout << "\n\t\tInvalid quantity! Please enter a number between 1 and 100.\n";
                                    qty = -1;
                                    continue;
                                }

                                if (qty < 1 || qty > 100) {
                                    cout << "\n\t\tQuantity must be between 1 and 100!\n";
                                }
                            } while (qty < 1 || qty > 100);

                                manager.addOrder(bevItem, qty);


                                string notification = "Order added: " + bevItem->getName() +
                                                    " [" + string(1, bevItem->getBeverageType()) + "]" +
                                                    " x" + to_string(qty) +
                                                    " at " + to_string(time(0) % 10000);
                                orderNotifications.enqueue(notification);

                                cout << "\t\tAdded to notification queue (Position: "
                                     << orderNotifications.getSize() << ")\n";
                            }
                        } else {
                            cout << "\n\t\tInvalid selection!\n";
                        }
                    } else {
                        cout << "\n\t\tInvalid choice!\n";
                }
                break;
            }

            //edit order
            case '2': {

                if (!manager.hasOrders()) {
                    cout << "\n\t\tNo orders to edit!\n";
                    break;
                }

                //clear input buffer
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                manager.displayOrders();
                int orderNum, newQty;
                cout << "\n\t\tEnter order number to edit (1-" << manager.getOrderCount() << "): ";

                string orderInput;
                getline(cin, orderInput);
                if (orderInput.empty() || !all_of(orderInput.begin(), orderInput.end(), ::isdigit)) {
                    cout << "\n\t\tInvalid input! Please enter a number.\n";
                    waitForEnter();
                    break;
                }

                try {
                    orderNum = stoi(orderInput);
                } catch (...) {
                    cout << "\n\t\tInvalid order number!\n";
                    waitForEnter();
                    break;
                }

                //check valid order range
                if (orderNum >= 1 && orderNum <= manager.getOrderCount()) {
                    do {
                        cout << "\t\tEnter new quantity: ";
                        string qtyInput;
                        getline(cin, qtyInput);

                        // Check if input is empty or contains non-digits
                        if (qtyInput.empty() || !all_of(qtyInput.begin(), qtyInput.end(), ::isdigit)) {
                            cout << "\n\t\tInvalid quantity! Please enter a number between 1 and 100.\n";
                            newQty = -1;
                            continue;
                        }

                        try {
                            newQty = stoi(qtyInput);
                        } catch (...) {
                            cout << "\n\t\tInvalid quantity! Please enter a number between 1 and 100.\n";
                            newQty = -1;
                            continue;
                        }

                        if (newQty < 1 || newQty > 100) {
                            cout << "\n\t\tQuantity must be between 1 and 100!\n";
                        }
                    } while (newQty < 1 || newQty > 100);

                    if (newQty > 0) {
                        manager.editOrder(orderNum - 1, newQty);
                        cout << "\n\t\tOrder updated successfully!\n";

                        //add notification
                        OrderItem* order = manager.getOrderAt(orderNum - 1);
                        if (order) {
                            string notification = "Order edited: " + order->getName() +
                                                " x" + to_string(newQty) +
                                                " at " + to_string(time(0) % 10000);
                            orderNotifications.enqueue(notification);
                        }
                    } else {
                        cout << "\n\t\tQuantity must be greater than 0!\n";
                    }
                } else {
                    cout << "\n\t\tInvalid order number!\n";
                }
                break;
            }

            //Delete order
            case '3': {
                if (!manager.hasOrders()) {
                    cout << "\n\t\tNo orders to delete!\n";
                    break;
                }

                manager.displayOrders();
                int orderNum;
                cout << "\n\t\tEnter order number to delete (1-" << manager.getOrderCount() << "): ";
                cin >> orderNum;

                //validate order number
                if (orderNum >= 1 && orderNum <= manager.getOrderCount()) {
                    OrderItem* order = manager.getOrderAt(orderNum - 1);
                    string orderName = order ? order->getName() : "";

                    //delete order
                    if (manager.deleteOrder(orderNum - 1)) {
                        cout << "\n\t\tOrder deleted successfully!\n";

                        //Add delete notification
                        if (!orderName.empty()) {
                            string notification = "Order deleted: " + orderName +
                                                " at " + to_string(time(0) % 10000);
                            orderNotifications.enqueue(notification);
                        }
                    } else {
                        cout << "\n\t\tFailed to delete order!\n";
                    }
                } else {
                    cout << "\n\t\tInvalid order number!\n";
                }
                break;
            }

            //View orders
            case '4': {
                manager.displayOrders();
                if (manager.hasOrders()) {
                    float total = manager.getTotal();
                    cout << "\n\t\tTOTAL: RM " << fixed << setprecision(2) << total << endl;
                }
                break;
            }

            //Search Order
            case '5': {
                if (!manager.hasOrders()) {
                    cout << "\n\t\tNo orders to search!\n";
                    break;
                }

                string searchTerm;
                cin.ignore();
                cout << "\t\tEnter item name to search: ";
                getline(cin, searchTerm);
                manager.searchOrder(searchTerm);
                break;
            }

            //Sort orders
            case '6': {
                manager.sortOrdersByPrice();
                break;
            }

            //View notifications (Queue-FIFO)
            case '7': {
                system("cls");
                cout << "\n\t\t" << string(50, '=') << "\n";
                cout << "\t\t           ORDER NOTIFICATIONS QUEUE\n";
                cout << "\t\t" << string(50, '=') << "\n";

                if (orderNotifications.isEmpty()) {
                    cout << "\t\tNo notifications in queue.\n";
                } else {
                    cout << "\t\tNotifications (" << orderNotifications.getSize() << "):\n";
                    cout << "\t\t" << string(50, '-') << "\n";

                    Queue<string> tempQueue;
                    int count = 1;

                    //display notifications without removing them permanently
                    while (!orderNotifications.isEmpty()) {
                        string notif = orderNotifications.dequeue();
                        cout << "\t\t" << count++ << ". " << notif << "\n";
                        tempQueue.enqueue(notif);
                    }

                    //Restore queue
                    while (!tempQueue.isEmpty()) {
                        orderNotifications.enqueue(tempQueue.dequeue());
                    }

                    cout << "\t\t" << string(50, '-') << "\n";
                    cout << "\n\t\tOptions:\n";
                    cout << "\t\t1. Clear all notifications\n";
                    cout << "\t\t2. Process oldest notification\n";
                    cout << "\t\t3. Back\n";
                    cout << "\t\t" << string(50, '=') << "\n";
                    cout << "\n\t\tChoice: ";

                    char queueChoice;
                    cin >> queueChoice;

                    if (queueChoice != '1' && queueChoice != '2' && queueChoice != '3') {
                        cout << "\n\t\tInvalid choice! Please enter 1, 2, or 3.\n";
                        waitForEnter();
                        break;
                    }

                    if (queueChoice == '1') {
                        orderNotifications.clear();
                        cout << "\n\t\tAll notifications cleared!\n";
                    } else if (queueChoice == '2' && !orderNotifications.isEmpty()) {
                        string processed = orderNotifications.dequeue();
                        cout << "\n\t\tProcessed: " << processed << "\n";
                        cout << "\t\t" << orderNotifications.getSize() << " notifications remaining\n";
                    }
                }
                break;
            }

            case '0':
                break;

            default:
                cout << "\n\t\tInvalid option! Please enter 0-7.\n";
        }

        if (c != '0') {
            waitForEnter();
        }

    } while (c != '0');
}

// ==================== CHECKOUT FUNCTION ====================
//Handles final checkout process including receipt display, tax calculation,
//Reservation confirmation, payment processing, and order clearing
void checkout(OrderManager& manager, Customer& customer, bool hasReservation, string customerUsername) {
    system("cls");

    if (!manager.hasOrders()) {
        cout << "\n\t\t" << string(60, '=') << "\n";
        cout << "\t\t                   NO ORDERS\n";
        cout << "\t\t" << string(60, '=') << "\n";
        cout << "\t\tYou have no orders to checkout!\n";
        waitForEnter();
        return;
    }

    //Reservation confirmation
    //If customer has an active reservation, confirm it
    if (hasReservation) {
        cout << "\n\t\t" << string(60, '=') << "\n";
        cout << "\t\t                RESERVATION CONFIRMATION\n";
        cout << "\t\t" << string(60, '=') << "\n";
        customer.displayBooking();
        customer.setStatus(CONFIRMED);
        cout << "\n\t\tReservation confirmed!\n";
        cout << "\t\t" << string(60, '=') << "\n\n";
    }

    //final receipt
    cout << "\n\n\t   "<< string(70, '*') << "\n";
    cout << "\t                               FINAL RECEIPT\n";
    cout << "\t   "<< string(70, '*') << "\n";
    manager.displayOrders();

    //calculate totals
    float total = manager.getTotal();
    float tax = total * SERVICE_TAX;
    float finalTotal = total + tax;

    //Display receipt summary
    cout << "\n\t   "<< string(70, '-') << "\n";
    cout << left << setw(37) << "\t\tSubtotal:" << "RM " << fixed << setprecision(2) << right << setw(13) << total << endl;
    cout << left << setw(37) << "\t\tService Tax (10%):" << "RM " << fixed << setprecision(2) << right << setw(13) << tax << endl;
    cout << left << setw(37) << "\t\tTOTAL AMOUNT:" << "RM " << fixed << setprecision(2) << right << setw(13) << finalTotal << endl;
    cout << "\t   "<< string(70, '-') << "\n";

    //Display Linked reservation ID if applicable
    if (hasReservation) {
        cout << "\n\t\t\tLinked to Reservation ID: " << customer.getReservationId() << endl;
    }

    //Save all completed order to file/database
    manager.saveAllOrders();

    //payment section
    cout << "\n\t   "<< string(70, '*') << "\n";
    cout << "\t                             PAYMENT OPTIONS\n";
    cout << "\t   "<< string(70, '*') << "\n";

   bool paymentSuccess = processPayment(total, customerUsername);

    if (paymentSuccess) {
        manager.clearOrders();
        cout << "\n\t\tCheckout completed successfully!\n";
        cout << "\t\tThank you for your order!\n";
    } else {
        cout << "\n\t\tCheckout cancelled. Orders are not cleared.\n";
    }
    waitForEnter();
}

// ==================== PAYMENT FUNCTIONS ====================
// handles online payment processing including method selection,
// PIN verification, confirmation, and simulated bank approval
// Get current time only
bool processOnlinePayment(float amount, const string& customerName) {

    system("cls");
    cout << "\n\t\t" << string(60, '=') << "\n";
    cout << "\t\t                 ONLINE PAYMENT PROCESSING\n";
    cout << "\t\t" << string(60, '=') << "\n";

    cout << "\t\tAmount: RM " << fixed << setprecision(2) << amount << endl;
    cout << "\t\t" << string(60, '-') << "\n";

    cout << "\n\t\tSelect Online Payment Method:\n";
    cout << "\t\t1. FPX (Internet Banking)\n";
    cout << "\t\t2. e-Wallet (Touch 'n Go, GrabPay, etc.)\n";
    cout << "\t\t3. Online Banking Transfer\n";
    cout << "\n\t\tChoice: ";

    int onlineChoice;
    string input;
    getline(cin, input);

    // Validate online payment method choice
    if (input.empty() || input.length() != 1 || !isdigit(input[0])) {
        cout << "\n\t\tInvalid choice. Payment cancelled.\n";
        return false;
    }

    //Convert character input to integer
    onlineChoice = input[0] - '0';

    //validate choice range
    if (onlineChoice < 1 || onlineChoice > 3) {
        cout << "\n\t\tInvalid choice. Payment cancelled.\n";
        return false;
    }

    //Display message based on selected payment method
    switch(onlineChoice) {
        case 1:
            cout << "\n\t\tYou will be redirected to your bank's login page...\n";
            break;
        case 2:
            cout << "\n\t\tPlease open your e-Wallet app to confirm payment...\n";
            break;
        case 3:
            cout << "\n\t\tPlease complete the transfer within 15 minutes...\n";
            break;
    }
    //stimulate redirection/loading delay
    Sleep(2000);

    // PIN Verification for all online payment methods
    system("cls");
    cout << "\n\t\t" << string(50, '=') << "\n";
    cout << "\t\t               SECURITY VERIFICATION\n";
    cout << "\t\t" << string(50, '=') << "\n";

    //prompt user to enter 6-digit PIN (hidden input)
    string pin;
    cout << "\t\tEnter your 6-digit PIN for verification: ";
    pin = getPassword();

    while (!isValidPIN(pin)) {
        cout << "\t\tEnter your 6-digit PIN: ";
        pin = getPassword();
    }

    //Ask user to confirm payment
    char confirm;
    cout << "\n\t\tConfirm payment of RM " << fixed << setprecision(2) << amount << "? (y/n): ";
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear input buffer

    //cancel payment if user does not confirm
    if (tolower(confirm) != 'y') {
        cout << "\n\t\tPayment cancelled by user.\n";
        return false;
    }

    //payment processing
    //stimulate payment processing animation
    cout << "\n\t\tProcessing payment";
    for (int i = 0; i < 5; i++) {
        cout << ".";
        Sleep(400);
    }

    //Stimulate bank PIN verification
    cout << "\n\t\tVerifying PIN with bank";
    for (int i = 0; i < 3; i++) {
        cout << ".";
        Sleep(600);
    }

    // Random success/failure for demonstration (80% success rate)
    srand(time(0));
    bool paymentApproved = (rand() % 100) < 80;

    if (paymentApproved) {
        cout << "\n\n\t\tPIN verified successfully!\n";
        cout << "\t\tPayment of RM " << fixed << setprecision(2) << amount << " approved!\n\n";

        // Show transaction details
        cout << "\t\t" << string(40, '=') << "\n";
        cout << "\t\t        TRANSACTION DETAILS\n";
        cout << "\t\t" << string(40, '=') << "\n";
        cout << "\t\tAmount:      RM " << fixed << setprecision(2) << amount << endl;
        cout << "\t\tMethod:      ";

        //Display payment method used
        switch(onlineChoice) {
            case 1: cout <<       "FPX (Internet Banking)"; break;
            case 2: cout <<       "e-Wallet"; break;
            case 3: cout <<       "Online Banking Transfer"; break;
        }
        cout << endl;
        cout << "\t\tStatus:      COMPLETED\n";
        cout << "\t\tReference:   ONL" << (10000 + rand() % 90000) << endl;
        cout << "\t\tTime:        " << getCurrentTime() << endl;
        cout << "\t\tDate:        " << getCurrentDate() << endl;
        cout << "\t\t" << string(40, '=') << "\n";

        return true; //payment successful
    } else {
        cout << "\n\n\t\tPayment failed!\n";
        cout << "\t\tIncorrect PIN or insufficient funds.\n";
        cout << "\t\tPlease try again with a different payment method.\n";
        return false;
    }
}

// ==================== PAYMENT PROCESS FUNCTION ====================
// Handles payment selection, validation, processing (cash/card/online),
// and receipt generation after successful payment
bool processPayment(float totalAmount, string customerName){

    // Calculate service tax and grand total
    float serviceTax = totalAmount * SERVICE_TAX;
    float grandTotal = totalAmount + serviceTax;

    // Display available payment methods
    cout << "\n\t\tPayment Methods:\n";
    cout << "\t\t1. Cash\n\t\t2. Credit/Debit Card\n\t\t3. Online Payment\n";
    cout << "\n\t   "<< string(70, '*') << "\n";

    int choice;
    bool validInput = false;

    // Clear any previous input errors and ignore leftover newline
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    //payment method selection
    //loop until a valid payment is selected
    do {
        cout << "\n\t\tSelect payment method (1-3): ";
        string input;
        getline(cin, input);

        // Check if input is empty
        if (input.empty()) {
            cout << "\t\t    Invalid choice! Please enter 1, 2, or 3.\n";
            continue;
        }

        // Check if input is exactly one character and a digit
        if (input.length() != 1 || !isdigit(input[0])) {
            cout << "\t\t    Invalid choice! Please enter 1, 2, or 3.\n";
            continue;
        }

        // Convert to integer
        choice = input[0] - '0';  // Convert char to int

        // Check if it's a valid choice
        if (choice < 1 || choice > 3) {
            cout << "\t\t    Invalid choice! Please enter 1, 2, or 3.\n";
            continue;
        }

        validInput = true;

    } while (!validInput);

    //Stored selected payment method and result
    string paymentMethod;
    bool paymentSuccess = false;

    //handle payment method
    switch(choice) {

        //cash payment
        case 1: {
            paymentMethod = "Cash";
            system("cls");

            // Display cash payment information
            cout << "\n\t\t" << string(49, '=') << "\n";
            cout << "\t\t                  CASH PAYMENT\n";
            cout << "\t\t" << string(49, '=') << "\n";
            cout << "\t\tAmount: RM " << fixed << setprecision(2) << grandTotal << endl;
            cout << "\t\t" << string(49, '-') << "\n\n";

            cout << "\t\tPlease prepare RM " << fixed << setprecision(2) << grandTotal << " in cash.\n";
            cout << "\t\tStaff will assist you with the payment.\n\n";

            char confirm;
            cout << "\t\tReady to proceed? (y/n): ";
            cin >> confirm;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear input buffer

            // Simulate payment processing
            if (tolower(confirm) == 'y') {
                cout << "\n\t\tProcessing cash payment";
                for (int i = 0; i < 5; i++) {
                    cout << ".";
                    Sleep(400);
                }

                cout << "\n\n\t\tCash payment accepted!\n";
                cout << "\t\tThank you for your payment.\n";

                // Show transaction details
                cout << "\n\t\t" << string(40, '=') << "\n";
                cout << "\t\t          TRANSACTION RECEIPT\n";
                cout << "\t\t" << string(40, '=') << "\n";
                cout << "\t\tAmount:      RM " << fixed << setprecision(2) << grandTotal << endl;
                cout << "\t\tMethod:      Cash Payment\n";
                cout << "\t\tStatus:      COMPLETED\n";
                cout << "\t\tReference:   CASH" << (1000 + rand() % 9000) << endl;
                cout << "\t\tTime:        " << getCurrentTime() << endl;
                cout << "\t\tDate:        " << getCurrentDate() << endl;
                cout << "\t\t" << string(40, '=') << "\n";

                Sleep(2000);
                paymentSuccess = true;
            } else {
                cout << "\n\t\tPayment cancelled by user.\n";
                Sleep(1500);
                paymentSuccess = false;
            }
            break;
        }

        //card payment
        case 2: {
            paymentMethod = "Credit/Debit Card";
            system("cls");

            // Display card payment header
            cout << "\n\t\t" << string(49, '=') << "\n";
            cout << "\t\t                  CARD PAYMENT\n";
            cout << "\t\t" << string(49, '=') << "\n";
            cout << "\t\tAmount: RM " << fixed << setprecision(2) << grandTotal << endl;
            cout << "\t\t" << string(49, '-') << "\n\n";

            string pin;
            int pinAttempts = 0;
            const int maxPinAttempts = 3;
            bool validPinEntered = false;

            // Allow up to 3 PIN attempts
            while (pinAttempts < maxPinAttempts && !validPinEntered) {
                cout << "\t\tEnter your 6-digit PIN (Attempt " << (pinAttempts + 1) << "/" << maxPinAttempts << "): ";
                pin = getPassword();

                if (isValidPIN(pin)) {
                    validPinEntered = true;
                } else {
                    pinAttempts++;
                    if (pinAttempts < maxPinAttempts) {
                        cout << "\n\t\tInvalid PIN. Please try again.\n";
                        Sleep(1000);
                        // Clear and show header again
                        system("cls");
                        cout << "\n\t\t" << string(55, '=') << "\n";
                        cout << "\t\t        CARD PAYMENT\n";
                        cout << "\t\t" << string(55, '=') << "\n";
                        cout << "\t\tAmount: RM " << fixed << setprecision(2) << grandTotal << endl;
                        cout << "\t\t" << string(30, '-') << "\n\n";
                    }
                }
            }

            // Cancel if PIN attempts exceeded
            if (!validPinEntered) {
                cout << "\n\t\tToo many incorrect PIN attempts.\n";
                cout << "\t\tPayment cancelled for security reasons.\n";
                Sleep(3000);
                paymentSuccess = false;
                break;
            }

            char confirm;
            cout << "\n\t\tConfirm payment of RM " << fixed << setprecision(2) << grandTotal << "? (y/n): ";
            cin >> confirm;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear input buffer

          //stimulate bank processing
            if (tolower(confirm) == 'y') {
                cout << "\n\t\tProcessing payment";
                for (int i = 0; i < 5; i++) {
                    cout << ".";
                    Sleep(400);
                }

                cout << "\n\t\tAuthorizing with bank";
                for (int i = 0; i < 3; i++) {
                    cout << ".";
                    Sleep(600);
                }

                // 95% success rate for cards (higher than online)
                srand(time(0));
                bool cardApproved = (rand() % 100) < 95;

                if (cardApproved) {
                    cout << "\n\n\t\tPIN verified successfully!\n";
                    cout << "\t\tPayment of RM " << fixed << setprecision(2) << grandTotal << " approved!\n";

                    // Show transaction details
                    cout << "\n\t\t" << string(40, '=') << "\n";
                    cout << "\t\t          TRANSACTION RECEIPT\n";
                    cout << "\t\t" << string(40, '=') << "\n";
                    cout << "\t\tAmount:      RM " << fixed << setprecision(2) << grandTotal << endl;
                    cout << "\t\tMethod:      Credit/Debit Card\n";
                    cout << "\t\tStatus:      COMPLETED\n";
                    cout << "\t\tReference:   CARD" << (10000 + rand() % 90000) << endl;
                    cout << "\t\tTime:        " << getCurrentTime() << endl;
                    cout << "\t\tDate:        " << getCurrentDate() << endl;
                    cout << "\t\t" << string(40, '=') << "\n";

                    Sleep(2000);
                    paymentSuccess = true;
                } else {
                    cout << "\n\n\t\tPayment declined by bank!\n";
                    cout << "\t\tPlease try a different card or payment method.\n";
                    Sleep(2000);
                    paymentSuccess = false;
                }
            } else {
                cout << "\n\t\tPayment cancelled by user.\n";
                Sleep(1500);
                paymentSuccess = false;
            }
            break;
        }

        //online payment
        case 3: {
            paymentMethod = "Online Payment";
            paymentSuccess = processOnlinePayment(grandTotal, customerName);
            break;
        }
    }

    //receipe file generation
    if (paymentSuccess) {
        cout << "\n\n\t\tPayment successful!\n";
        cout << "\t\tReceipt has been generated.\n";

        //generate receipt file
        string filename = "receipt_" + customerName + "_" + to_string(time(0)) + ".txt";
        ofstream receipt(filename);
        if (receipt.is_open()) {
            receipt << "\t\t======== NYONYA HERITAGE HOUSE RECEIPT ========\n";
            receipt << "\t\tCustomer: " << customerName << endl;
            receipt << "\t\tDate: " << getCurrentDate() << " Time: " << getCurrentTime() << endl;
            receipt << "\t\t" << string(50, '-') << endl;
            receipt << "\t\tSubtotal: RM " << fixed << setprecision(2) << totalAmount << endl;
            receipt << "\t\tService Tax (10%): RM " << serviceTax << endl;
            receipt << "\t\tGrand Total: RM " << grandTotal << endl;
            receipt << "\t\tPayment Method: " << paymentMethod << endl;

            // Add transaction reference based on payment method
            if (paymentMethod == "Cash") {
                receipt << "\t\tReference: CASH" << (1000 + rand() % 9000) << endl;
            } else if (paymentMethod == "Credit/Debit Card") {
                receipt << "\t\tReference: CARD" << (10000 + rand() % 90000) << endl;
            } else if (paymentMethod == "Online Payment") {
                receipt << "\t\tReference: ONL" << (10000 + rand() % 90000) << endl;
            }

            receipt << "\t\tPayment Status: PAID\n";
            receipt << "\t\t" << string(50, '=') << endl;
            receipt << "\t\tThank you for dining with us!\n";
            receipt << "\t\tWe hope to serve you again soon!\n";
            receipt.close();

            cout << "\t\tReceipt saved as '" << filename << "'\n";
        } else {
            cout << "\t\tCould not save receipt to file.\n";
        }
    } else {
        cout << "\n\t\tPayment failed or was cancelled.\n";
        cout << "\t\tPlease try again or choose a different payment method.\n";
    }
    return paymentSuccess;
}


// ==================== MENU DISPLAY FUNCTIONS ====================
//Display the food menu in a formatted table layout
void displayFoodMenu(const LinkedList<Food>& menu) {

    //Display menu header
    cout << "\n\t\t" << string(50, '=') << "\n";
    cout << "\t\t                   FOOD MENU\n";
    cout << "\t\t" << string(50, '=') << "\n";

    //Display column titles
    cout << left << setw(7) << "\t\tNO" << left << setw(31) << "ITEM NAME" << left << setw(10) << "PRICE" << endl;

    //Display Divider
    cout << "\t\t" << string(50, '-') << "\n";


    // Iterate through food menu items and display them
    int i = 1;
    auto it = menu.begin();
    while (it.hasNext()) {
        cout << left << "\t\t" << setw(5) << i++;
        it.next().display();
    }
    cout << "\t\t" << string(50, '=') << "\n\n";
}

// ==================== Beverage Menu Functios =====================
void displayBeverageMenu(const LinkedList<Beverage>& menu) {
    cout << "\n\t\t" << string(50, '=') << "\n";
    cout << "\t\t                 BEVERAGE MENU\n";
    cout << "\t\t" << string(50, '=') << "\n";
    cout << left << setw(7) << "\t\tNO" << left << setw(25) << "ITEM NAME"
         << left << setw(12) << "TYPE" << left << setw(20) << "PRICE" << endl;
    cout << "\t\t" << string(50, '-') << "\n";

    int i = 1;
    auto it = menu.begin();
    while (it.hasNext()) {
        cout << left << "\t\t" << setw(5) << i++;
        it.next().display();
    }
    cout << "\t\t" << string(50, '=') << "\n";
}

// ==================== ADMIN MENU ====================
void adminMenu(OrderManager& manager, LinkedList<Food>& foodMenuVec, LinkedList<Beverage>& bevMenuVec) {
    char choice;
    do {
        system("cls");
        cout << "\n\t\t" << string(50, '=') << "\n";
        cout << "\t\t                   ADMIN PANEL\n";
        cout << "\t\t" << string(50, '=') << "\n";
        cout << "\t\t1. Manage Orders\n";
        cout << "\t\t2. Manage Reservations\n";
        cout << "\t\t3. Manage Menu\n";
        cout << "\t\t4. View Sales Report\n";
        cout << "\t\t5. Manage Customers\n";
        cout << "\t\t0. Logout\n";
        cout << "\t\t" << string(50, '=') << "\n";
        cout << "\n\t\tEnter choice: ";
        string input;
        getline(cin,input);


        // If we get an empty string immediately, it means there was a leftover newline
        // Let's get fresh input in that case
        if (input.empty()) {
            // Clear the buffer again
            cin.clear();
            getline(cin, input);
        }

        // Check if input is still empty
        if (input.empty()) {
            cout << "\n\t\tInput cannot be empty! Please enter 0-5.\n";
            waitForEnter();
            continue;
        }

        // Trim whitespace
        input.erase(0, input.find_first_not_of(" \t\n\r"));
        input.erase(input.find_last_not_of(" \t\n\r") + 1);

        // Check if input is exactly one character
        if (input.length() != 1) {
            cout << "\n\t\tInvalid input! Please enter a single digit (0-5).\n";
            waitForEnter();
            continue;
        }

        // Get the single character
        choice = input[0];

        // Check if it's one of the valid choices (0-5)
        if (!(choice >= '0' && choice <= '5')) {
            cout << "\n\t\tInvalid choice! Please enter 0-5.\n";
            waitForEnter();
            continue;
        }

        switch (choice) {
            case '1':
                manageOrdersAdmin();
                break;
            case '2':
                manageReservationsAdmin();
                break;
            case '3':
                manageMenuAdmin(foodMenuVec, bevMenuVec);
                break;
            case '4':
                viewSalesReport(manager);
                break;
            case '5':
                manageCustomersAdmin();
                break;
            case '0':
                cout << "\n\t\tLogging out...\n";
                waitForEnter();
                return;
            default:
                cout << "\n\t\tInvalid choice! Please enter 0-5.\n";
                waitForEnter();
        }
    } while (true);
}

// ==================== ADMIN FUNCTIONS ====================
void manageOrdersAdmin() {
    char choice;

    do {
        // Load fresh data each time
        LinkedList<string> ordersList = loadOrders();

        // Display orders
        system("cls");
        cout << "\n\t\t" << string(100, '=') << "\n";
        cout << "\t\t                                         MANAGE ORDERS\n";
        cout << "\t\t" << string(100, '=') << "\n";

        if (ordersList.isEmpty()) {
            cout << "\n\t\tNo orders found in database.\n";
        } else {
            cout << left << setw(7) << "\t\tNO" << left << setw(25) << "ITEM NAME"
                 << left << setw(12) << "PRICE" << left << setw(10) << "QTY"
                 << left << setw(12) << "SUBTOTAL" << left << setw(20) << "CUSTOMER"
                 << left << setw(15) << "STATUS" << endl;
            cout << "\t\t" << string(100, '-') << "\n";

            int i = 1;
            auto it = ordersList.begin();
            while (it.hasNext()) {
                string order = it.next();
                LinkedList<string> tokens = splitString(order, '|');

                if (tokens.getSize() >= 6) {
                    string statusStr;
                    try {
                        int statusInt = stoi(*tokens.get(5));
                        statusStr = orderStatusToString(static_cast<OrderStatus>(statusInt));
                    } catch (...) {
                        statusStr = "Unknown";
                    }

                    // Calculate subtotal
                    float subtotal = 0.0;
                    try {
                        float price = stof(*tokens.get(1));
                        int qty = stoi(*tokens.get(2));
                        subtotal = price * qty;
                    } catch (...) {}

                    string displayName = (*tokens.get(0)).length() > 23 ?
                                         (*tokens.get(0)).substr(0, 23) + ".." : *tokens.get(0);
                    string displayCustomer = (*tokens.get(4)).length() > 18 ?
                                             (*tokens.get(4)).substr(0, 18) + ".." : *tokens.get(4);

                    cout << left << "\t\t" << setw(5) << i++
                         << left << setw(25) << displayName
                         << "RM " << left << setw(9) << fixed << setprecision(2) << stof(*tokens.get(1))
                         << left << setw(10) << *tokens.get(2)
                         << "RM " << left << setw(9) << fixed << setprecision(2) << subtotal
                         << left << setw(20) << displayCustomer
                         << left << setw(15) << statusStr << endl;
                }
            }
        }

        cout << "\n\t\t" << string(100, '=') << "\n";
        cout << "\t\tOptions:\n";
        cout << "\t\t1. Update Order Status\n";
        cout << "\t\t2. Search Orders (Linear Search)\n";
        cout << "\t\t3. Sort Orders (Bubble Sort)\n";
        cout << "\t\t4. Delete Order\n";
        cout << "\t\t0. Back\n";
        cout << "\t\t" << string(100, '=') << "\n";
        cout << "\n\t\tChoice: ";
        string input;
        getline(cin, input);

        if (input.empty()) {
            cout << "\n\t\tError: Choice cannot be empty!\n";
            waitForEnter();
            continue;
        }

        if (input.length() != 1 || !isdigit(input[0])) {
            cout << "\n\t\tError: Please enter a number between 0 and 4!\n";
            waitForEnter();
            continue;
        }

        choice = input[0];

        if (choice < '0' || choice > '4') {
            cout << "\n\t\tError: Invalid choice! Please enter 0 4.\n";
            waitForEnter();
            continue;
        }

        if (choice == '1') {
            // UPDATE ORDER STATUS
            LinkedList<string> currentOrders = loadOrders();
            if (currentOrders.isEmpty()) {
                cout << "\n\t\tNo orders to update!\n";
                waitForEnter();
                continue;
            }

          cout << "\n\t\tEnter order number to update status (1-" << currentOrders.getSize() << "): ";
          getline(cin, input);
        //Validate order number input
        if (input.empty()) {
            cout << "\n\t\tError: Selection cannot be empty!\n";
            waitForEnter();
            continue;
        }


        int idx;
        try {
            idx = stoi(input);
        } catch (...) {
            cout << "\n\t\tError: Please enter a valid number!\n";
            waitForEnter();
            continue;
        }

        if (idx < 1 || idx > currentOrders.getSize()) {
            cout << "\n\t\tError: Invalid order number!\n";
            waitForEnter();
            continue;
        }
        string orderStr = *currentOrders.get(idx - 1);
        LinkedList<string> tokens = splitString(orderStr, '|');

        //validate order record
        if (tokens.getSize() < 6) {
            cout << "\n\t\tError: Corrupted order data!\n";
            waitForEnter();
            continue;
        }
        //Get current status safely
        int currentStatus;
            try {
                currentStatus = stoi(*tokens.get(5));
            } catch (...) {
                currentStatus = 0;
            }
            //Display current status
            cout << "\n\t\tCurrent Status: "
                 << orderStatusToString(static_cast<OrderStatus>(currentStatus))
                 << endl;

            cout << "\t\tSelect new status:\n";
            cout << "\t\t1. Pending\n\t\t2. Preparing\n\t\t3. Ready\n\t\t4. Served\n\t\t5. Cancelled\n";
            cout << "\t\tChoice: ";
            getline(cin, input);

                    if (input.empty()) {
                                cout << "\n\t\tError: Status choice cannot be empty!\n";
                                waitForEnter();
                                continue;
                            }

                            int statusChoice;
                            try {
                                statusChoice = stoi(input);
                            } catch (...) {
                                cout << "\n\t\tError: Invalid status input!\n";
                                waitForEnter();
                                continue;
                            }
                    //Validate status range
                     if (statusChoice < 1 || statusChoice > 5) {
                                cout << "\n\t\tError: Invalid status choice!\n";
                                waitForEnter();
                                continue;
                            }

                            // Update status token (status stored as 0 4)
                            *tokens.get(5) = to_string(statusChoice - 1);

                            // Reconstruct updated order string
                            string updatedOrder;
                            for (int i = 0; i < tokens.getSize(); i++) {
                                updatedOrder += *tokens.get(i);
                                if (i < tokens.getSize() - 1)
                                    updatedOrder += "|";
                            }

                            // Update order in list
                            *currentOrders.get(idx - 1) = updatedOrder;

                            // Save update
                            *currentOrders.get(idx - 1) = updatedOrder;
                            clearOrderFile();

                            auto saveIt = currentOrders.begin();
                            while (saveIt.hasNext()) {
                                string orderToSave = saveIt.next();
                                if (!orderToSave.empty()) {
                                    saveOrder(orderToSave);
                                }
                            }

                cout << "\n\t\tOrder status updated successfully!\n";
                waitForEnter();
                continue;
        }

        else if (choice == '2') {
            // LINEAR SEARCH Implementation
            LinkedList<string> currentOrders = loadOrders();
            if (currentOrders.isEmpty()) {
                cout << "\n\t\tNo orders to search!\n";
                waitForEnter();
                continue;
            }

            int searchChoice;
            cout << "\n\t\tSearch by:\n";
            cout << "\t\t1. Customer Name\n";
            cout << "\t\t2. Item Name\n";
            cout << "\t\t3. Order Status\n";
            cout << "\t\t4. Back to Main List\n";
            cout << "\n\t\tChoice: ";
            string input;
            getline(cin, input);

            //Validate search menu choice
            if (input.empty()) {
                cout << "\n\t\tError: Choice cannot be empty!\n";
                waitForEnter();
                continue;
            }

            try {
                searchChoice = stoi(input);
            } catch (...) {
                cout << "\n\t\tError: Please enter a valid number!\n";
                waitForEnter();
                continue;
            }

            if (searchChoice < 1 || searchChoice > 4) {
                cout << "\n\t\tError: Invalid choice! Please enter 1-4.\n";
                waitForEnter();
                continue;
            }

            //Back to main menu
            if (searchChoice == 4) {
                continue;
            }

            //Get search term
            string searchTerm;
            cout << "\t\tEnter search term: ";
            getline(cin, searchTerm);


            // Empty input check
            if (searchTerm.empty()) {
                cout << "\n\t\tError: Search term cannot be empty!\n";
                waitForEnter();
                continue;
            }

            // Alphabet-only validation
            if (!isAlphabetOnly(searchTerm)) {
                cout << "\n\t\tError: Only alphabetic characters are allowed!\n";
                waitForEnter();
                continue;
            }

            // Convert to lowercase for comparison
            transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), ::tolower);

            LinkedList<string> searchResults;
            auto it = currentOrders.begin();

            while (it.hasNext()) {
                string order = it.next();
                LinkedList<string> tokens = splitString(order, '|');

                if (tokens.getSize() >= 6) {
                    bool match = false;

                    //Customer Name
                    if (searchChoice == 1 && tokens.getSize() > 4) {
                        string customerLower = *tokens.get(4);
                        transform(customerLower.begin(), customerLower.end(), customerLower.begin(), ::tolower);
                        if (customerLower.find(searchTerm) != string::npos) match = true;
                    }

                    //Item Name
                    else if (searchChoice == 2 && tokens.getSize() > 0) {
                        string itemLower = *tokens.get(0);
                        transform(itemLower.begin(), itemLower.end(), itemLower.begin(), ::tolower);
                        if (itemLower.find(searchTerm) != string::npos) match = true;
                    }
                    //Order Status
                    else if (searchChoice == 3 && tokens.getSize() > 5) {
                        int statusInt;
                        try {
                            statusInt = stoi(*tokens.get(5));
                        } catch (...) {
                            statusInt = 0;
                        }
                        string statusStr = orderStatusToString(static_cast<OrderStatus>(statusInt));
                        transform(statusStr.begin(), statusStr.end(), statusStr.begin(), ::tolower);
                        if (statusStr.find(searchTerm) != string::npos) match = true;
                    }

                    if (match) {
                        searchResults.insertAtEnd(order);
                    }
                }
            }

            // Display search results
            system("cls");
            cout << "\n\t\t" << string(100, '=') << "\n";
            cout << "\t\t                                 LINEAR SEARCH RESULTS (" << searchResults.getSize() << " found)\n";
            cout << "\t\t" << string(100, '=') << "\n";

            if (searchResults.isEmpty()) {
                cout << "\n\t\tNo matching orders found.\n";
            } else {
                cout << left << setw(7) << "\t\tNO" << left << setw(25) << "ITEM NAME"
                     << left << setw(12) << "PRICE" << left << setw(10) << "QTY"
                     << left << setw(20) << "CUSTOMER" << left << setw(15) << "STATUS" << endl;
                cout << "\t\t" << string(100, '-') << "\n";

                int i = 1;
                auto resultIt = searchResults.begin();
                while (resultIt.hasNext()) {
                    string order = resultIt.next();
                    LinkedList<string> tokens = splitString(order, '|');

                    if (tokens.getSize() >= 6) {
                        string statusStr;
                        try {
                            int statusInt = stoi(*tokens.get(5));
                            statusStr = orderStatusToString(static_cast<OrderStatus>(statusInt));
                        } catch (...) {
                            statusStr = "Unknown";
                        }

                        string displayName = (*tokens.get(0)).length() > 23 ?
                                             (*tokens.get(0)).substr(0, 23) + ".." : *tokens.get(0);
                        string displayCustomer = (*tokens.get(4)).length() > 18 ?
                                                 (*tokens.get(4)).substr(0, 18) + ".." : *tokens.get(4);

                        cout << left << "\t\t" << setw(5) << i++
                             << left << setw(25) << displayName
                             << "RM " << left << setw(9) << fixed << setprecision(2) << stof(*tokens.get(1))
                             << left << setw(10) << *tokens.get(2)
                             << left << setw(20) << displayCustomer
                             << left << setw(15) << statusStr << endl;
                    }
                }
            }

            cout << "\t\t" << string(100, '=') << "\n";
            waitForEnter();
        }

        else if (choice == '3') {
            // BUBBLE SORT Implementation - FIXED
            LinkedList<string> currentOrders = loadOrders();
            if (currentOrders.isEmpty()) {
                cout << "\nNo orders to sort!\n";
                waitForEnter();
                continue;
            }

            int sortChoice;
            cout << "\n\t\tSort orders by:\n";
            cout << "\t\t1. Customer Name (A-Z)\n";
            cout << "\t\t2. Item Name (A-Z)\n";
            cout << "\t\t3. Status (Pending -> Served)\n";
            cout << "\t\t4. Price (Low to High)\n";
            cout << "\t\t5. Cancel\n";
            cout << "\n\t\tChoice: ";
            string input;
            getline(cin, input);

            //validate sort menu choice
            if (input.empty()) {
                cout << "\n\t\tError: Choice cannot be empty!\n";
                waitForEnter();
                continue;
            }

            try {
                sortChoice = stoi(input);
            } catch (...) {
                cout << "\n\t\tError: Please enter a valid number (0-5)!\n";
                waitForEnter();
                continue;
            }

            if (sortChoice < 1 || sortChoice > 5) {
                cout << "\n\t\tError: Invalid choice! Please enter 1-5.\n";
                waitForEnter();
                continue;
            }

            //Cancel sorting
            if (sortChoice == 5){
                continue;
            }

            // Perform Bubble Sort
            if (sortChoice == 1) {
                // Sort by Customer Name
                bool swapped;
                int n = currentOrders.getSize();
                for (int i = 0; i < n - 1; i++) {
                    swapped = false;
                    for (int j = 0; j < n - i - 1; j++) {
                        string* order1 = currentOrders.get(j);
                        string* order2 = currentOrders.get(j + 1);

                        if (order1 && order2) {
                            LinkedList<string> tokens1 = splitString(*order1, '|');
                            LinkedList<string> tokens2 = splitString(*order2, '|');

                            if (tokens1.getSize() >= 5 && tokens2.getSize() >= 5) {
                                string customer1 = *tokens1.get(4);
                                string customer2 = *tokens2.get(4);

                                if (customer1 > customer2) {
                                    // Swap orders
                                    string temp = *order1;
                                    *order1 = *order2;
                                    *order2 = temp;
                                    swapped = true;
                                }
                            }
                        }
                    }
                    if (!swapped) break;
                }
            }
            else if (sortChoice == 2) {
                // Sort by Item Name
                bool swapped;
                int n = currentOrders.getSize();
                for (int i = 0; i < n - 1; i++) {
                    swapped = false;
                    for (int j = 0; j < n - i - 1; j++) {
                        string* order1 = currentOrders.get(j);
                        string* order2 = currentOrders.get(j + 1);

                        if (order1 && order2) {
                            LinkedList<string> tokens1 = splitString(*order1, '|');
                            LinkedList<string> tokens2 = splitString(*order2, '|');

                            if (tokens1.getSize() >= 1 && tokens2.getSize() >= 1) {
                                string item1 = *tokens1.get(0);
                                string item2 = *tokens2.get(0);

                                if (item1 > item2) {
                                    // Swap orders
                                    string temp = *order1;
                                    *order1 = *order2;
                                    *order2 = temp;
                                    swapped = true;
                                }
                            }
                        }
                    }
                    if (!swapped) break;
                }
            }
            else if (sortChoice == 3) {
                // Sort by Status
                bool swapped;
                int n = currentOrders.getSize();
                for (int i = 0; i < n - 1; i++) {
                    swapped = false;
                    for (int j = 0; j < n - i - 1; j++) {
                        string* order1 = currentOrders.get(j);
                        string* order2 = currentOrders.get(j + 1);

                        if (order1 && order2) {
                            LinkedList<string> tokens1 = splitString(*order1, '|');
                            LinkedList<string> tokens2 = splitString(*order2, '|');

                            if (tokens1.getSize() >= 6 && tokens2.getSize() >= 6) {
                                int status1, status2;
                                try {
                                    status1 = stoi(*tokens1.get(5));
                                    status2 = stoi(*tokens2.get(5));
                                } catch (...) {
                                    status1 = status2 = 0;
                                }

                                if (status1 > status2) {
                                    // Swap orders
                                    string temp = *order1;
                                    *order1 = *order2;
                                    *order2 = temp;
                                    swapped = true;
                                }
                            }
                        }
                    }
                    if (!swapped) break;
                }
            }
            else if (sortChoice == 4) {
                // Sort by Price
                bool swapped;
                int n = currentOrders.getSize();
                for (int i = 0; i < n - 1; i++) {
                    swapped = false;
                    for (int j = 0; j < n - i - 1; j++) {
                        string* order1 = currentOrders.get(j);
                        string* order2 = currentOrders.get(j + 1);

                        if (order1 && order2) {
                            LinkedList<string> tokens1 = splitString(*order1, '|');
                            LinkedList<string> tokens2 = splitString(*order2, '|');

                            if (tokens1.getSize() >= 2 && tokens2.getSize() >= 2) {
                                float price1, price2;
                                try {
                                    price1 = stof(*tokens1.get(1));
                                    price2 = stof(*tokens2.get(1));
                                } catch (...) {
                                    price1 = price2 = 0;
                                }

                                if (price1 > price2) {
                                    // Swap orders
                                    string temp = *order1;
                                    *order1 = *order2;
                                    *order2 = temp;
                                    swapped = true;
                                }
                            }
                        }
                    }
                    if (!swapped) break;
                }
            }

            // Save sorted orders back to file
            clearOrderFile();
            auto saveIt = currentOrders.begin();
            while (saveIt.hasNext()) {
                string order = saveIt.next();
                if (!order.empty()) {
                    saveOrder(order);
                }
            }

            system("cls");
                cout << "\n\t\t" << string(100, '=') << "\n";
                cout << "\t\t                                      SORTED ORDER LIST\n";
                cout << "\t\t" << string(100, '=') << "\n";

                cout << left << setw(7) << "\t\tNO"
                     << left << setw(25) << "ITEM NAME"
                     << left << setw(12) << "PRICE"
                     << left << setw(10) << "QTY"
                     << left << setw(12) << "SUBTOTAL"
                     << left << setw(20) << "CUSTOMER"
                     << left << setw(15) << "STATUS" << endl;

                cout << "\t\t" << string(100, '-') << "\n";

                int idx = 1;
                auto showIt = currentOrders.begin();
                while (showIt.hasNext()) {
                    string order = showIt.next();
                    LinkedList<string> tokens = splitString(order, '|');

                    if (tokens.getSize() >= 6) {
                        float price = stof(*tokens.get(1));
                        int qty = stoi(*tokens.get(2));
                        float subtotal = price * qty;
                        int statusInt = stoi(*tokens.get(5));

                        cout << left << "\t\t" << setw(5) << idx++
                             << left << setw(25) << trim(*tokens.get(0))
                             << "RM " << left << setw(9) << fixed << setprecision(2) << price
                             << left << setw(10) << qty
                             << "RM " << left << setw(9) << fixed << setprecision(2) << subtotal
                             << left << setw(20) << trim(*tokens.get(4))
                             << left << setw(15)
                             << orderStatusToString(static_cast<OrderStatus>(statusInt))
                             << endl;
                    }
                }
                cout << "\t\t" << string(100, '=') << "\n";
                cout << "\n\t\tOrders sorted successfully using Bubble Sort!\n";
            waitForEnter();
        }


        else if (choice == '4') {
            // DELETE ORDER
            LinkedList<string> currentOrders = loadOrders();

            //Check empty list
            if (currentOrders.isEmpty()) {
                cout << "\n\t\tNo orders to delete!\n";
                waitForEnter();
                continue;
            }

            string input;
            int idx;
            cout << "\t\tEnter order number to delete (1-" << currentOrders.getSize() << "): ";
            getline(cin,input);

            //Empty input
            if (input.empty()) {
                cout << "\n\t\tError: Order number cannot be empty!\n";
                waitForEnter();
                continue;
            }

            //non-numeric input
            try {
                idx = stoi(input);
            } catch (...) {
                cout << "\n\t\tError: Please enter a valid number!\n";
                waitForEnter();
                continue;
            }

            //Range validation
             if (idx < 1 || idx > currentOrders.getSize()) {
                cout << "\n\t\tError: Invalid order number!\n";
                waitForEnter();
                continue;
            }

            //Confirmation
            string confirm;
            cout << "\t\tAre you sure you want to delete this order? (y/n): ";
            getline(cin,confirm);

             if (confirm.empty()) {
                cout << "\n\t\tError: Confirmation cannot be empty!\n";
                waitForEnter();
                continue;
            }

            char c = tolower(confirm[0]);

            if (c != 'y' && c != 'n') {
                cout << "\n\t\tError: Please enter 'y' or 'n' only!\n";
                waitForEnter();
                continue;
            }

            if (c == 'n') {
                cout << "\n\t\tOperation cancelled.\n";
                waitForEnter();
                continue;
            }

            // Perform deletion
            currentOrders.deleteNode(idx - 1);

            //Save updated list
                clearOrderFile();
                auto saveIt = currentOrders.begin();
                while (saveIt.hasNext()) {
                    string order = saveIt.next();
                    if (!order.empty()) {
                        saveOrder(order);
                    }
                }

                cout << "\n\t\tOrder deleted successfully!\n";
                waitForEnter();
            }
        else if (choice != '0') {
            cout << "\n\t\tInvalid choice!\n";
            waitForEnter();
        }

    } while (choice != '0');
}

// ==================== Manage Reservations Admin ====================
void manageReservationsAdmin() {
    char choice ;

    do {
        // Load fresh data each time
        LinkedList<string> reservationsList = loadReservations();

        // Display reservations
        system("cls");
        cout << "\n\t\t" << string(90, '=') << "\n";
        cout << "\t\t                          MANAGE RESERVATIONS\n";
        cout << "\t\t" << string(90, '=') << "\n";

    if (reservationsList.isEmpty()) {
        cout << "\n\t\tNo reservations found.\n";
    } else {
        // Store latest reservation by ID using HashMap
        HashMap<int, string> latestReservations;

        // Collect latest reservations
        auto tempIt = reservationsList.begin();
        while (tempIt.hasNext()) {
            string reservation = tempIt.next();
            LinkedList<string> tokens = splitString(reservation, '|');
            if (tokens.getSize() >= 1) {
                try {
                    int resId = stoi(*tokens.get(0));
                    // Use HashMap insert method
                    latestReservations.insert(resId, reservation);
                } catch (...) {}
            }
        }

        cout << left << setw(7) << "\t\tNO" << left << setw(15) << "RESERVATION ID"
             << left << setw(20) << "NAME" << left << setw(15) << "DATE"
             << left << setw(10) << "TIME" << left << setw(10) << "PEOPLE"
             << left << setw(15) << "STATUS" << endl;
        cout << "\t\t" << string(90, '-') << "\n";

        LinkedList<int> seenIds;
        int i = 1;

        auto it = reservationsList.begin();
        while (it.hasNext()) {
            string reservation = it.next();
            LinkedList<string> tokens = splitString(reservation, '|');
            if (tokens.getSize() >= 8) {
                try {
                    int resId = stoi(*tokens.get(0));

                    // Check if we've seen this ID before (keep only latest)
                    bool seen = false;
                    auto idIt = seenIds.begin();
                    while (idIt.hasNext()) {
                        if (idIt.next() == resId) {
                            seen = true;
                            break;
                        }
                    }

                    if (!seen) {
                        seenIds.insertAtEnd(resId);

                        int statusInt;
                        try {
                            statusInt = stoi(*tokens.get(7));
                        } catch (...) {
                            statusInt = 0;
                        }
                        string statusStr = statusToString(intToStatus(statusInt));

                        string displayName = (*tokens.get(2)).length() > 18 ?
                                             (*tokens.get(2)).substr(0, 18) + ".." : *tokens.get(2);

                        // Format date as DD/MM/YY
                        string date = *tokens.get(4);
                        string formattedDate = date;
                        if (date.length() == 6) {
                            formattedDate = date.substr(0, 2) + "/" +
                                           date.substr(2, 2) + "/" +
                                           date.substr(4, 2);
                        }

                        // Format time as HH:MM
                        string time = *tokens.get(5);
                        string formattedTime = time;
                        if (time.length() == 4) {
                            formattedTime = time.substr(0, 2) + ":" + time.substr(2, 2);
                        }

                        cout << left << "\t\t" << setw(5) << i++
                             << left << setw(15) << *tokens.get(0)
                             << left << setw(20) << displayName
                             << left << setw(15) << formattedDate
                             << left << setw(10) << formattedTime
                             << left << setw(10) << *tokens.get(6)
                             << left << setw(15) << statusStr << endl;
                    }
                } catch (...) {}
            }
        }
    }

        cout << "\n\t\t" << string(90, '=') << "\n";
        cout << "\t\tOptions:\n";
        cout << "\t\t1. Update Status\n";
        cout << "\t\t2. Search Reservations (Linear Search)\n";
        cout << "\t\t3. Sort Reservations (Bubble Sort)\n";
        cout << "\t\t4. Delete Reservation\n";
        cout << "\t\t0. Back\n";
        cout << "\t\t" << string(90, '=') << "\n";
        cout << "\n\t\tChoice: ";

        string input;
        getline(cin,input);
        if (input.empty() || input.length() != 1 || !isdigit(input[0])) {
        cout << "\n\t\tError: Enter a number from 0 to 4 only.\n";
        waitForEnter();
        continue;
    }

    choice = input[0];

        if (choice == '1') {
            // UPDATE STATUS
            LinkedList<string> currentReservations = loadReservations();
            if (currentReservations.isEmpty()) {
                cout << "\n\t\tNo reservations to update!\n";
                waitForEnter();
                continue;
            }
            string input;
            int idx;
            cout << "\t\tEnter reservation number to update (1-" << currentReservations.getSize() << "): ";
            getline(cin,input);

            // Empty input
            if (input.empty()) {
                cout << "\n\t\tError: Reservation number cannot be empty!\n";
                waitForEnter();
                continue;
            }

            // Must be numeric
            try {
                idx = stoi(input);
            } catch (...) {
                cout << "\n\t\tError: Please enter a valid number!\n";
                waitForEnter();
                continue;
            }

            // Range check
            if (idx < 1 || idx > currentReservations.getSize()) {
                cout << "\n\t\tError: Invalid reservation number!\n";
                waitForEnter();
                continue;
            }

            //Get selected reservation
            string reservationStr = *currentReservations.get(idx - 1);
            LinkedList<string> tokens = splitString(reservationStr, '|');

              if (tokens.getSize() < 8) {
                    cout << "\n\t\tError: Corrupted reservation data!\n";
                    waitForEnter();
                    continue;
                }

                // Get current status safely
                int currentStatus = 0;
                try {
                    currentStatus = stoi(*tokens.get(7));
                } catch (...) {}

                    cout << "\n\t\tCurrent Status: " << statusToString(intToStatus(currentStatus)) << endl;
                    cout << "\t\tSelect new status:\n";
                    cout << "\t\t1. Pending\n\t\t2. Confirmed\n\t\t3. Seated\n\t\t4. Completed\n\t\t5. Cancelled\n";
                    cout << "\n\t\tChoice: ";
                    getline(cin,input);
                     // Empty status input
                    if (input.empty()) {
                        cout << "\n\t\tError: Status choice cannot be empty!\n";
                        waitForEnter();
                        continue;
                    }

                    int statusChoice;
                    try {
                        statusChoice = stoi(input);
                    } catch (...) {
                        cout << "\n\t\tError: Invalid status input!\n";
                        waitForEnter();
                        continue;
                    }

                    //validate status range
                    if (statusChoice < 1 || statusChoice > 5) {
                        cout << "\n\t\tError: Status must be between 1 and 5!\n";
                        waitForEnter();
                        continue;
                    }

                        // Update status (stored as 0-4)
                       *tokens.get(7) = to_string(statusChoice - 1);

                        // Rebuild reservation string
                        string updatedRes;
                        for (int i = 0; i < tokens.getSize(); i++) {
                            updatedRes += *tokens.get(i);
                            if (i < tokens.getSize() - 1) updatedRes += "|";
                        }

                        // Update in list
                        *currentReservations.get(idx - 1) = updatedRes;

                        // Save to file
                        clearReservationFile();
                        auto saveIt = currentReservations.begin();
                        while (saveIt.hasNext()) {
                            string res = saveIt.next();
                            if (!res.empty()) {
                                saveReservation(res);
                            }
                        }

                        cout << "\n\t\tReservation status updated successfully!\n";
            waitForEnter();
        }
         else if (choice == '2') {
            // LINEAR SEARCH for reservations
            LinkedList<string> currentReservations = loadReservations();
            if (currentReservations.isEmpty()) {
                cout << "\n\t\tNo reservations to search!\n";
                waitForEnter();
                continue;
            }

            int searchChoice;
            cout << "\n\t\tSearch by:\n";
            cout << "\t\t1. Customer Name\n";
            cout << "\t\t2. Date\n";
            cout << "\t\t3. Status\n";
            cout << "\t\t4. Reservation ID\n";
            cout << "\t\t5. Back to Main List\n";
            cout << "\n\t\tChoice: ";

        if (!(cin >> searchChoice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\t\tError: Please enter a number!\n";
            waitForEnter();
            continue;
        }

        if (searchChoice < 1 || searchChoice > 5) {
            cout << "\n\t\tError: Invalid choice! Please enter 1 5.\n";
            waitForEnter();
            continue;
        }

        if (searchChoice == 5) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

            string searchTerm;
            cout << "\t\tEnter search term: ";
            getline(cin, searchTerm);

            // Empty search term
            if (searchTerm.empty()) {
                cout << "\n\t\tError: Search term cannot be empty!\n";
                waitForEnter();
                continue;
            }

            // Validate search term by type
            if (searchChoice == 1 || searchChoice == 3) {
                // Alphabet only
                for (char c : searchTerm) {
                    if (!isalpha(c) && c != ' ') {
                        cout << "\n\t\tError: Only alphabet characters are allowed!\n";
                        waitForEnter();
                        continue;
                    }
                }
            }
            else if (searchChoice == 2 || searchChoice == 4) {
                // Numeric only
                for (char c : searchTerm) {
                    if (!isdigit(c)) {
                        cout << "\n\t\tError: Only numeric characters are allowed!\n";
                        waitForEnter();
                        continue;
                    }
                }
            }

            transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), ::tolower);

            LinkedList<string> searchResults;
            auto it = currentReservations.begin();

            while (it.hasNext()) {
                string reservation = it.next();
                LinkedList<string> tokens = splitString(reservation, '|');

                if (tokens.getSize() >= 8) {
                    bool match = false;

                    //Customer name
                    if (searchChoice == 1 && tokens.getSize() > 2) {
                        string nameLower = *tokens.get(2);
                        transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
                        if (nameLower.find(searchTerm) != string::npos) match = true;
                    }
                    //Date
                    else if (searchChoice == 2 && tokens.getSize() > 4) {
                        string dateLower = *tokens.get(4);
                        if (dateLower.find(searchTerm) != string::npos) match = true;
                    }
                    //Status
                    else if (searchChoice == 3 && tokens.getSize() > 7) {
                        int statusInt;
                        try {
                            statusInt = stoi(*tokens.get(7));
                        } catch (...) {
                            statusInt = 0;
                        }
                        string statusStr = statusToString(intToStatus(statusInt));
                        transform(statusStr.begin(), statusStr.end(), statusStr.begin(), ::tolower);
                        if (statusStr.find(searchTerm) != string::npos) match = true;
                    }
                    //Reservation ID
                    else if (searchChoice == 4 && tokens.getSize() > 0) {
                        string idLower = *tokens.get(0);
                        if (idLower.find(searchTerm) != string::npos) match = true;
                    }

                    if (match) {
                        searchResults.insertAtEnd(reservation);
                    }
                }
            }

            // Display search results
            system("cls");
            cout << "\n\t\t" << string(90, '=') << "\n";
            cout << "\t\t                             LINEAR SEARCH RESULTS (" << searchResults.getSize() << " found)\n";
            cout << "\t\t" << string(90, '=') << "\n";

            if (searchResults.isEmpty()) {
                cout << "\n\t\tNo matching reservations found.\n";
            } else {
                cout << left << setw(7) << "\t\tNO" << left << setw(15) << "RESERVATION ID"
                     << left << setw(20) << "NAME" << left << setw(15) << "DATE"
                     << left << setw(10) << "TIME" << left << setw(10) << "PEOPLE"
                     << left << setw(15) << "STATUS" << endl;
                cout << "\t\t" << string(90, '-') << "\n";

                int i = 1;
                auto resultIt = searchResults.begin();
                while (resultIt.hasNext()) {
                    string reservation = resultIt.next();
                    LinkedList<string> tokens = splitString(reservation, '|');

                    int statusInt = 0;
                    try { statusInt = stoi(*tokens.get(7)); } catch (...) {}

                    cout << left << "\t\t" << setw(5) << i++
                         << left << setw(15) << *tokens.get(0)
                         << left << setw(20) << *tokens.get(2)
                         << left << setw(15) << *tokens.get(4)
                         << left << setw(10) << *tokens.get(5)
                         << left << setw(10) << *tokens.get(6)
                         << left << setw(15)
                         << statusToString(intToStatus(statusInt)) << endl;
                }
            }

            cout << "\t\t" << string(90, '=') << "\n";
            waitForEnter();
        }

        else if (choice == '3') {
            // BUBBLE SORT for reservations - FIXED
            LinkedList<string> currentReservations = loadReservations();
            if (currentReservations.isEmpty()) {
                cout << "\nNo reservations to sort!\n";
                waitForEnter();
                continue;
            }
            int sortChoice;
            cout << "\n\t\tSort reservations by:\n";
            cout << "\t\t1. Customer Name (A-Z)\n";
            cout << "\t\t2. Date (Oldest First)\n";
            cout << "\t\t3. Status (Pending -> Completed)\n";
            cout << "\t\t4. Number of People (Low to High)\n";
            cout << "\t\t5. Cancel\n";
            cout << "\n\t\tChoice: ";
            string input;
            getline(cin, input);

            //empty input
            if (input.empty()) {
                cout << "\n\t\tError: Choice cannot be empty!\n";
                waitForEnter();
                continue;
            }

            //must be numeric
            try {
                sortChoice = stoi(input);
            } catch (...) {
                cout << "\n\t\tError: Please enter a valid number!\n";
                waitForEnter();
                continue;
            }

            //range check
            if (sortChoice < 1 || sortChoice > 5) {
                cout << "\n\t\tError: Invalid choice! Please enter 1-5.\n";
                waitForEnter();
                continue;
            }

            //Cancel
            if (sortChoice == 5) {
                continue;
            }

            // Perform Bubble Sort
            bool swapped;
            int n = currentReservations.getSize();

            for (int i = 0; i < n - 1; i++) {
                swapped = false;
                for (int j = 0; j < n - i - 1; j++) {
                    string* res1 = currentReservations.get(j);
                    string* res2 = currentReservations.get(j + 1);

                    if (res1 && res2) {
                        LinkedList<string> tokens1 = splitString(*res1, '|');
                        LinkedList<string> tokens2 = splitString(*res2, '|');

                        if (tokens1.getSize() >= 8 && tokens2.getSize() >= 8) {
                            bool shouldSwap = false;

                            if (sortChoice == 1) {
                                // Sort by Customer Name
                                string name1 = *tokens1.get(2);
                                string name2 = *tokens2.get(2);
                                shouldSwap = (name1 > name2);
                            }
                            else if (sortChoice == 2) {
                                // Sort by Date
                                string date1 = *tokens1.get(4);
                                string date2 = *tokens2.get(4);
                                shouldSwap = (date1 > date2);
                            }
                            else if (sortChoice == 3) {
                                // Sort by Status
                                int status1, status2;
                                try {
                                    status1 = stoi(*tokens1.get(7));
                                    status2 = stoi(*tokens2.get(7));
                                } catch (...) {
                                    status1 = status2 = 0;
                                }
                                shouldSwap = (status1 > status2);
                            }
                            else if (sortChoice == 4) {
                                // Sort by Number of People
                                int people1, people2;
                                try {
                                    people1 = stoi(*tokens1.get(6));
                                    people2 = stoi(*tokens2.get(6));
                                } catch (...) {
                                    people1 = people2 = 0;
                                }
                                shouldSwap = (people1 > people2);
                            }

                            if (shouldSwap) {
                                // Swap reservations
                                string temp = *res1;
                                *res1 = *res2;
                                *res2 = temp;
                                swapped = true;
                            }
                        }
                    }
                }
                if (!swapped) break;
            }

            clearReservationFile();
            auto saveIt = currentReservations.begin();
            while (saveIt.hasNext()) {
                string res = saveIt.next();
                if (!res.empty()) {
                    saveReservation(res);
                }
            }

            cout << "\n\t\tReservations sorted successfully using Bubble Sort!\n";
            waitForEnter();
        }
        else if (choice == '4') {
            // DELETE RESERVATION
            LinkedList<string> currentReservations = loadReservations();
            if (currentReservations.isEmpty()) {
                cout << "\n\t\tNo reservations to delete!\n";
                waitForEnter();
                continue;
            }

            int idx;
            string input;
            cout << "\t\tEnter reservation number to delete (1-" << currentReservations.getSize() << "): ";
            getline(cin,input);

            // empty check
            if (input.empty()) {
                cout << "\n\t\tError: Reservation number cannot be empty!\n";
                waitForEnter();
                continue;
            }

            // numeric check
            try {
                idx = stoi(input);
            } catch (...) {
                cout << "\n\t\tError: Please enter a valid number!\n";
                waitForEnter();
                continue;
            }

            // range check
            if (idx < 1 || idx > currentReservations.getSize()) {
                cout << "\n\t\tError: Invalid reservation number!\n";
                waitForEnter();
                continue;
            }

            //confirmation
                cout << "\t\tAre you sure you want to delete this reservation? (y/n): ";
               getline(cin,input);

               //Empty confirm
                 if (input.empty()) {
                    cout << "\n\t\tError: Confirmation cannot be empty!\n";
                    waitForEnter();
                    continue;
                }
                    char confirm =tolower(input[0]);
                //Invalid confirm input
                    if (confirm != 'y' && confirm != 'n') {
                        cout << "\n\t\tError: Please enter 'y' or 'n' only!\n";
                        waitForEnter();
                        continue;
                    }

                    if (tolower(confirm) == 'y') {
                    currentReservations.deleteNode(idx - 1);

                    clearReservationFile();
                    auto saveIt = currentReservations.begin();
                    while (saveIt.hasNext()) {
                        string res = saveIt.next();
                        if (!res.empty()) {
                            saveReservation(res);
                        }
                    }

                    cout << "\n\t\tReservation deleted successfully!\n";
                } else {
                    cout << "\n\t\tOperation cancelled.\n";
                }
            waitForEnter();
        }
        else if (choice != '0') {
            cout << "\n\t\tInvalid choice!\n";
            waitForEnter();
        }

    } while (choice != '0');
}

// ==================== ADMIN MENU MANAGEMENT ====================
// Allows admin to add, update, delete, and view food and beverage menu items
// Uses LinkedList for dynamic menu storage and includes full input validation
void manageMenuAdmin(LinkedList<Food>& foodMenu, LinkedList<Beverage>& bevMenu) {
    string input;
    char choice;

    // Loop until admin chooses to go back
    do {
        system("cls");

        // Display admin menu options
        cout << "\n\t\t" << string(60, '=') << "\n";
        cout << "\t\t                     MANAGE MENU\n";
        cout << "\t\t" << string(60, '=') << "\n";
        cout << "\t\t1. Add Food Item\n\t\t2. Update Food Item\n\t\t3. Delete Food Item\n";
        cout << "\t\t4. Add Beverage Item\n\t\t5. Update Beverage Item\n\t\t6. Delete Beverage Item\n";
        cout << "\t\t7. View Current Menu\n\t\t0. Back\n";
        cout << "\t\t" << string(60, '=') << "\n";
        cout << "\n\t\tChoice: ";

        cin.clear();
        getline(cin, input);

        if (input.empty()) {
            continue;
        }

         // Trim leading and trailing whitespace
        input.erase(0, input.find_first_not_of(" \t\n\r"));
        input.erase(input.find_last_not_of(" \t\n\r") + 1);

         // Validate menu choice (must be a single digit 0–7)
        if (input.length() != 1 || input[0] < '0' || input[0] > '7') {
            cout << "\n\t\tInvalid choice! Please enter 0-7.\n";
            waitForEnter();
            continue;
        }

        //Handle admin menu options
        choice = input[0];

        switch (choice) {

            //Add food item
            case '1': {
                string name;
                float price;

                cout << "\t\tFood name: ";
                getline(cin, name);

                if (name.empty()) {
                    cout << "\n\t\tError: Food name cannot be empty!\n";
                    waitForEnter();
                    break;
                }

                if (!isalpha(name[0])) {
                    cout << "\n\t\tError: Food name must start with a letter (A-Z).\n";
                    waitForEnter();
                    break;
                }

                // Check for duplicate food names (case-insensitive)
                bool foodExists = false;
                auto foodIt = foodMenu.begin();
                while (foodIt.hasNext()) {
                    Food& food = foodIt.next();
                    if (toLowercase(food.getName()) == toLowercase(name)) {
                        foodExists = true;
                        break;
                    }
                }

                if (foodExists) {
                    cout << "\n\t\tError: Food item '" << name << "' already exists!\n";
                    waitForEnter();
                    break;
                }

                cout << "\t\tPrice: RM ";
                getline(cin, input);
                try {
                    price = stof(input);
                } catch (...) {
                    cout << "\n\t\tError: Invalid price input!\n";
                    waitForEnter();
                    break;
                }

                if (price < 1 || price > 100) {
                    cout << "\n\t\tError: Food price must be between RM1 and RM100!\n";
                    waitForEnter();
                    break;
                }

                // Add food to linked list and save
                foodMenu.insertAtEnd(Food(name, price));
                saveMenuToFile(foodMenu, bevMenu);
                cout << "\n\t\tFood item added successfully!\n";
                waitForEnter();
                break;
            }

            // UPDATE FOOD ITEM
            case '2': {
                if (foodMenu.getSize() == 0) {
                    cout << "\n\t\tNo food items available to update!\n";
                    waitForEnter();
                    break;
                }

                displayFoodMenu(foodMenu);
                int idx;
                cout << "\n\t\tSelect food item to update (1-" << foodMenu.getSize() << "): ";
                cin >> idx;

                //validate selection
                if (cin.fail() || idx < 1 || idx > foodMenu.getSize()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\n\t\tError: Invalid food selection!\n";
                    waitForEnter();
                    break;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                string name;
                float price;

                cout << "\t\tNew name: ";
                getline(cin, name);
                if (name.empty()) {
                    cout << "\n\t\tError: Food name cannot be empty!\n";
                    waitForEnter();
                    break;
                }
                if (!isalpha(name[0])) {
                    cout << "\n\t\tError: Food name must start with an alphabet!\n";
                    waitForEnter();
                    break;
                }

                //Check duplicate names excluding current item
                bool duplicate = false;
                auto it = foodMenu.begin();
                int currentIndex = 0;

                while (it.hasNext()) {
                    Food& food = it.next();
                    if (currentIndex != (idx - 1) &&
                        toLowercase(food.getName()) == toLowercase(name)) {
                        duplicate = true;
                        break;
                    }
                    currentIndex++;
                }

                if (duplicate) {
                    cout << "\n\t\tError: Food name already exists!\n";
                    waitForEnter();
                    break;
                }

                cout << "\t\tNew price: ";
                getline(cin,input);

                if (input.empty()) {
                    cout << "\n\t\tError: Price cannot be empty!\n";
                    waitForEnter();
                    break;
                }

                try {
                    price = stof(input);
                } catch (...) {
                    cout << "\n\t\tError: Invalid price input!\n";
                    waitForEnter();
                    break;
                }

                if (price < 0 || price > 100) {
                    cout << "\n\t\tError: Price must be between RM0 and RM100!\n";
                    waitForEnter();
                    break;
                }

                if (cin.fail() || price < 1 || price > 100) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\n\t\tError: Price must be between RM1 and RM100!\n";
                    waitForEnter();
                    break;
                }

                 // Update food item
                Food* foodItem = foodMenu.get(idx - 1);
                if (foodItem) {
                    *foodItem = Food(name, price);
                    saveMenuToFile(foodMenu, bevMenu);
                    cout << "\n\t\tFood item updated successfully!\n";
                }

                waitForEnter();
                break;
            }

            //Delete food item
            case '3': {
                if (foodMenu.getSize() == 0) {
                    cout << "\n\t\tNo food items available to delete!\n";
                    waitForEnter();
                    break;
                }

                displayFoodMenu(foodMenu);
                cout << "\n\t\tSelect food item to delete (1-" << foodMenu.getSize() << "): ";
                getline(cin, input);

                if (input.empty()) {
                    cout << "\n\t\tError: Selection cannot be empty!\n";
                    waitForEnter();
                    break;
                }

                int idx;
                try {
                    idx = stoi(input);
                } catch (...) {
                    cout << "\n\t\tError: Please enter a valid number!\n";
                    waitForEnter();
                    break;
                }

                if (idx < 1 || idx > foodMenu.getSize()) {
                    cout << "\n\t\tError: Invalid food selection!\n";
                    waitForEnter();
                    break;
                }

                foodMenu.deleteNode(idx - 1);
                saveMenuToFile(foodMenu, bevMenu);
                cout << "\n\t\tFood item deleted successfully!\n";
                waitForEnter();
                break;
            }

            //Add beverage item
            case '4': {
                string name;
                float price;
                char type;

                cout << "\t\tBeverage name: ";
                getline(cin, name);

                if (name.empty()) {
                    cout << "\n\t\tError: Beverage name cannot be empty!\n";
                    waitForEnter();
                    break;
                }

                if (!isalpha(name[0])) {
                    cout << "\n\t\tError: Beverage name must start with an alphabet!\n";
                    waitForEnter();
                    break;
                }

                // Check for duplicate beverage name (case-insensitive)
                bool exists = false;
                auto it = bevMenu.begin();
                while (it.hasNext()) {
                    Beverage& bev = it.next();
                    if (toLowercase(bev.getName()) == toLowercase(name)) {
                        exists = true;
                        break;
                    }
                }

                if (exists) {
                    cout << "\n\t\tError: Beverage already exists!\n";
                    waitForEnter();
                    break;
                }

                cout << "\t\tPrice: RM ";
                getline(cin, input);

                if (input.empty()) {
                    cout << "\n\t\tError: Price cannot be empty!\n";
                    waitForEnter();
                    break;
                }

                try {
                    price = stof(input);
                } catch (...) {
                    cout << "\n\t\tError: Invalid price input!\n";
                    waitForEnter();
                    break;
                }

                if (price < 1 || price > 100) {
                    cout << "\n\t\tError: Price must be between RM1 and RM100!\n";
                    waitForEnter();
                    break;
                }

                cout << "\t\tType (H for Hot, C for Cold): ";
                getline(cin, input);
                if (input.length() != 1) {
                    cout << "\n\t\tError: Beverage type must be H or C!\n";
                    waitForEnter();
                    break;
                }

                type = toupper(input[0]);
                if (type != 'H' && type != 'C') {
                    cout << "\n\t\tError: Beverage type must be H or C!\n";
                    waitForEnter();
                    break;
                }

                bevMenu.insertAtEnd(Beverage(name, price, type));
                saveMenuToFile(foodMenu, bevMenu);
                cout << "\n\t\tBeverage item added successfully!\n";
                waitForEnter();
                break;
            }

            //Update beverage item
            case '5': {
                if (bevMenu.getSize() == 0) {
                    cout << "\n\t\tNo beverage items available to update!\n";
                    waitForEnter();
                    break;
                }

                displayBeverageMenu(bevMenu);
                cout << "\n\t\tSelect beverage item to update (1-" << bevMenu.getSize() << "): ";
                getline(cin, input);

                if (input.empty()) {
                    cout << "\n\t\tError: Selection cannot be empty!\n";
                    waitForEnter();
                    break;
                }

                int idx;
                try {
                    idx = stoi(input);
                } catch (...) {
                    cout << "\n\t\tError: Please enter a valid number!\n";
                    waitForEnter();
                    break;
                }

                if (idx < 1 || idx > bevMenu.getSize()) {
                    cout << "\n\t\tError: Invalid beverage selection!\n";
                    waitForEnter();
                    break;
                }

                string name;
                float price;
                char type;

                cout << "\t\tNew name: ";
                getline(cin, name);
                if (name.empty()) {
                    cout << "\n\t\tError: Beverage name cannot be empty!\n";
                    waitForEnter();
                    break;
                }

                if (!isalpha(name[0])) {
                    cout << "\n\t\tError: Beverage name must start with an alphabet!\n";
                    waitForEnter();
                    break;
                }

                // Check duplicate name excluding current item
                bool duplicate = false;
                auto it = bevMenu.begin();
                int currentIndex = 0;

                while (it.hasNext()) {
                    Beverage& bev = it.next();
                    if (currentIndex != (idx - 1) &&
                        toLowercase(bev.getName()) == toLowercase(name)) {
                        duplicate = true;
                        break;
                    }
                    currentIndex++;
                }
                if (duplicate) {
                    cout << "\n\t\tError: Beverage name already exists!\n";
                    waitForEnter();
                    break;
                }

                cout << "\t\tNew price: RM ";
                getline(cin, input);
                if (input.empty()) {
                    cout << "\n\t\tError: Price cannot be empty!\n";
                    waitForEnter();
                    break;
                }

                try {
                    price = stof(input);
                } catch (...) {
                    cout << "\n\t\tError: Invalid price input!\n";
                    waitForEnter();
                    break;
                }

                if (price < 1 || price > 100) {
                    cout << "\n\t\tError: Price must be between RM1 and RM100!\n";
                    waitForEnter();
                    break;
                }

                cout << "\t\tNew type (H for Hot, C for Cold): ";
                getline(cin, input);
                if (input.length() != 1) {
                    cout << "\n\t\tError: Beverage type must be H or C!\n";
                    waitForEnter();
                    break;
                }

                type = toupper(input[0]);
                if (type != 'H' && type != 'C') {
                    cout << "\n\t\tError: Beverage type must be H or C!\n";
                    waitForEnter();
                    break;
                }

                Beverage* bevItem = bevMenu.get(idx - 1);
                if (bevItem) {
                    *bevItem = Beverage(name, price, type);
                    saveMenuToFile(foodMenu, bevMenu);
                    cout << "\n\t\tBeverage item updated successfully!\n";
                }

                waitForEnter();
                break;
            }

            //Delete beverage item
            case '6': {
                if (bevMenu.getSize() == 0) {
                    cout << "\n\t\tNo beverage items available to delete!\n";
                    waitForEnter();
                    break;
                }

                displayBeverageMenu(bevMenu);
                cout << "\n\t\tSelect beverage item to delete (1-" << bevMenu.getSize() << "): ";
                getline(cin, input);

                if (input.empty()) {
                    cout << "\n\t\tError: Selection cannot be empty!\n";
                    waitForEnter();
                    break;
                }

                int idx;
                try {
                    idx = stoi(input);
                } catch (...) {
                    cout << "\n\t\tError: Please enter a valid number!\n";
                    waitForEnter();
                    break;
                }

                if (idx < 1 || idx > bevMenu.getSize()) {
                    cout << "\n\t\tError: Invalid beverage selection!\n";
                    waitForEnter();
                    break;
                }

                bevMenu.deleteNode(idx - 1);
                saveMenuToFile(foodMenu, bevMenu);
                cout << "\n\t\tBeverage item deleted successfully!\n";
                waitForEnter();
                break;
            }

            //View Current Menu
            case '7': {
                system("cls");
                displayFoodMenu(foodMenu);
                displayBeverageMenu(bevMenu);
                waitForEnter();
                break;
            }
            case '0':
                return;
            default:
                cout << "\n\t\tInvalid choice!\n";
                waitForEnter();
                break;
        }

    } while (choice != '0');
}

// ==================== SALES REPORT FUNCTION ====================
// Generates and displays a sales summary including total revenue,
// service tax, and top-selling items based on revenue
void viewSalesReport(OrderManager& manager) {
    system("cls");
    cout << "\n\t\t" << string(70, '=') << "\n";
    cout << "\t\t                        SALES REPORT SUMMARY\n";
    cout << "\t\t" << string(70, '=') << "\n\n";

    // Load all saved orders from file
    LinkedList<string> orders = loadOrders();

    if (orders.isEmpty()) {
        cout << "\t\tNo sales data available.\n";
        cout << string(70, '=') << "\n";
        waitForEnter();
        return;
    }
// LinkedLists to store aggregated quantity and revenue per item
    LinkedList<pair<string, int>> itemQuantityList;  // <Item Name, Total Quantity>
    LinkedList<pair<string, float>> itemRevenueList; // <Item Name, Total Revenue>
    float totalRevenue = 0;
    int totalOrders = 0;

    //aggregate sales data
    auto it = orders.begin();
    while (it.hasNext()) {
        string order = it.next();
        LinkedList<string> tokens = splitString(order, '|');

    //Ensure order has required fields
        if (tokens.getSize() >= 3) {
            try {
                string itemName = *tokens.get(0);
                float unitPrice = stof(*tokens.get(1));
                int qty = stoi(*tokens.get(2));
                float subtotal = unitPrice * qty;

                bool found = false;
                auto qtyIt = itemQuantityList.begin();
                auto revIt = itemRevenueList.begin();
                int index = 0;

                 // Search for existing item in aggregated lists
                while (qtyIt.hasNext() && revIt.hasNext()) {
                    if (qtyIt.next().first == itemName) {
                        itemQuantityList.get(index)->second += qty;
                        itemRevenueList.get(index)->second += subtotal;
                        found = true;
                        break;
                    }
                    index++;
                }

                 // If item does not exist, insert new record
                if (!found) {
                    itemQuantityList.insertAtEnd(make_pair(itemName, qty));
                    itemRevenueList.insertAtEnd(make_pair(itemName, subtotal));
                }

                // Update overall totals
                totalRevenue += subtotal;
                totalOrders++;

            } catch (...) { // Ignore invalid order data
            }
        }
    }

    //Revenue summary
    cout << "\t\t    " << string(60, '*') << "\n";
    cout << "\t\t                        REVENUE SUMMARY\n";
    cout << "\t\t    " << string(60, '*') << "\n";

    float serviceTax = totalRevenue * SERVICE_TAX;
    float grandTotal = totalRevenue + serviceTax;

    // Display revenue statistics
    cout << left << setw(25) << "\t\t    Total Orders:" << totalOrders << endl;
    cout << left << setw(25) << "\t\t    Total Revenue:"
         << "RM " << fixed << setprecision(2) << right << setw(10) << totalRevenue << endl;
    cout << left << setw(25) << "\t\t    Service Tax (10%):"
         << "RM " << fixed << setprecision(2) << right << setw(10) << serviceTax << endl;
    cout << left << setw(25) << "\t\t    GRAND TOTAL:"
         << "RM " << fixed << setprecision(2) << right << setw(10) << grandTotal << endl;
    cout << "\t\t    " << string(60, '*') << "\n\n";

    //top 5 items by revenue
    if (!itemRevenueList.isEmpty()) {
        cout << "\n\t\t    " << string(60, '*') << "\n";
        cout << "\t\t                TOP 5 POPULAR ITEMS (BY REVENUE)\n";
        cout << "\t\t    " << string(60, '*') << "\n";

        cout << left << setw(11) << "\t\t    NO" << left << setw(30) << "ITEM NAME"
             << left << setw(15) << "QUANTITY" << left << setw(15) << "REVENUE" << endl;
        cout << "\t\t    " << string(60, '-') << "\n";

        //sort items by revenue (Bubble Sort)
        bool swapped;
        int n = itemRevenueList.getSize();
        for (int i = 0; i < n - 1; i++) {
            swapped = false;
            for (int j = 0; j < n - i - 1; j++) {
                pair<string, float>* item1 = itemRevenueList.get(j);
                pair<string, float>* item2 = itemRevenueList.get(j + 1);
                pair<string, int>* qty1 = itemQuantityList.get(j);
                pair<string, int>* qty2 = itemQuantityList.get(j + 1);

                //Swap if revenue is in descending order
                if (item1 && item2 && qty1 && qty2 && item1->second < item2->second) {
                    pair<string, float> tempRev = *item1;
                    *item1 = *item2;
                    *item2 = tempRev;

                    pair<string, int> tempQty = *qty1;
                    *qty1 = *qty2;
                    *qty2 = tempQty;

                    swapped = true;
                }
            }
            //Stop early if already sorted
            if (!swapped) break;
        }

        //Display Top 5
        int count = 1;
        auto revIt = itemRevenueList.begin();
        auto qtyIt = itemQuantityList.begin();

        while (revIt.hasNext() && qtyIt.hasNext() && count <= 5) {
            pair<string, float> revenueItem = revIt.next();
            pair<string, int> quantityItem = qtyIt.next();

            // Truncate long item names
            string displayName = revenueItem.first.length() > 28 ?
                                revenueItem.first.substr(0, 28) + ".." : revenueItem.first;

            cout << left << "\t\t    " << setw(5) << count
                 << left << setw(30) << displayName
                 << left << setw(15) << quantityItem.second
                 << "RM " << fixed << setprecision(2) << left << setw(12) << revenueItem.second << endl;
            count++;
        }

        cout << "\t\t    " << string(60, '*') << "\n";
    }

    cout << "\n\t\t" << string(70, '=') << "\n";
    waitForEnter();
}

// ==================== MANAGE CUSTOMERS (ADMIN) ====================
// Allows admin to view, search, sort, delete customers,
// and view detailed customer information
void manageCustomersAdmin() {
    char choice;

    // Loop until admin chooses to return
 do {
        // Load fresh data each time
        LinkedList<string> customers = loadCustomers();

        // Display customers list
        system("cls");
        cout << "\n\t\t" << string(90, '=') << "\n";
        cout << "\t\t                               MANAGE CUSTOMERS\n";
        cout << "\t\t" << string(90, '=') << "\n";

        if (customers.isEmpty()) {
            cout << "\n\t\tNo customer records found in the database.\n";
        } else {
            cout << left << setw(7) << "\t\tNO" << left << setw(20) << "USERNAME"
                 << left << setw(25) << "FULL NAME" << left << setw(25) << "EMAIL"
                 << left << setw(15) << "PHONE" << endl;
            cout << "\t\t" << string(90, '-') << "\n";

            //Display each customer
            int i = 1;
            auto it = customers.begin();
            while (it.hasNext()) {
                string customer = it.next();
                LinkedList<string> tokens = splitString(customer, '|');
                if (tokens.getSize() >= 5) {
                    string username = *tokens.get(0);
                    string displayUsername = username.length() > 18 ? username.substr(0, 18) + ".." : username;

                    string name = tokens.getSize() > 2 ? *tokens.get(2) : "N/A";
                    string displayName = name.length() > 23 ? name.substr(0, 23) + ".." : name;

                    string email = tokens.getSize() > 3 ? *tokens.get(3) : "N/A";
                    string displayEmail = email.length() > 23 ? email.substr(0, 23) + ".." : email;

                    string phone = tokens.getSize() > 4 ? *tokens.get(4) : "N/A";

                    cout << left << "\t\t" << setw(5) << i++
                         << left << setw(20) << displayUsername
                         << left << setw(25) << displayName
                         << left << setw(25) << displayEmail
                         << left << setw(15) << phone << endl;
                }
            }
        }

        cout << "\n\t\t" << string(90, '=') << "\n";
        cout << "\t\tOptions:\n";
        cout << "\t\t1. Search Customers (Linear Search)\n";
        cout << "\t\t2. Sort Customers (Bubble Sort)\n";
        cout << "\t\t3. Delete Customer\n";
        cout << "\t\t4. View Customer Details\n";
        cout << "\t\t0. Back to Admin Menu\n";
        cout << "\t\t" << string(90, '=') << "\n";
        cout << "\n\t\tChoice: ";
        string input;
        getline(cin, input);

        //Empty input
        if (input.empty()) {
            cout << "\n\t\tError: Choice cannot be empty!\n";
            waitForEnter();
            continue;
        }

        //Must be exactly one digit
        if (input.length() != 1 || !isdigit(input[0])) {
            cout << "\n\t\tError: Please enter a number from 0 to 4!\n";
            waitForEnter();
            continue;
        }

        //Convert to char
        choice = input[0];

        //Range check
        if (choice < '0' || choice > '4') {
            cout << "\n\t\tError: Invalid choice! Please enter from 0 to 4.\n";
            waitForEnter();
            continue;
        }

        //search (linear search)
        if (choice == '1') {
            LinkedList<string> currentCustomers = loadCustomers();
            if (currentCustomers.isEmpty()) {
                cout << "\n\t\tNo customers to search!\n";
                waitForEnter();
                continue;
            }

            int searchChoice;
            string input;
            cout << "\n\t\tSearch by:\n";
            cout << "\t\t1. Username\n";
            cout << "\t\t2. Full Name\n";
            cout << "\t\t3. Email\n";
            cout << "\t\t4. Phone\n";
            cout << "\t\t5. Back to Main List\n";
            cout << "\n\t\tChoice: ";
            getline(cin,input);
             //Empty check
             if (input.empty()) {
                cout << "\n\t\tError: Choice cannot be empty!\n";
                waitForEnter();
                continue;
            }

            //Numeric only
            try {
                searchChoice = stoi(input);
            } catch (...) {
                cout << "\n\t\tError: Please enter a valid number!\n";
                waitForEnter();
                continue;
            }

            //Range check
            if (searchChoice < 1 || searchChoice > 5) {
                cout << "\n\t\tError: Invalid choice! Please enter from 1 to 5.\n";
                waitForEnter();
                continue;
            }

            if (searchChoice == 5) continue;
            //get search keyword
            string searchTerm;
            cout << "\t\tEnter search term: ";
            getline(cin, searchTerm);

            //Empty search term
            if (searchTerm.empty()) {
                cout << "\n\t\tError: Search term cannot be empty!\n";
                waitForEnter();
                continue;
            }

            transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), ::tolower);

           // Perform linear search
            LinkedList<string> searchResults;
            auto it = currentCustomers.begin();

            while (it.hasNext()) {
                string customer = it.next();
                LinkedList<string> tokens = splitString(customer, '|');

                if (tokens.getSize() >= 5) {
                    bool match = false;

                    if (searchChoice == 1 && tokens.getSize() > 0) {
                        string usernameLower = *tokens.get(0);
                        transform(usernameLower.begin(), usernameLower.end(), usernameLower.begin(), ::tolower);
                        if (usernameLower.find(searchTerm) != string::npos) match = true;
                    }
                    else if (searchChoice == 2 && tokens.getSize() > 2) {
                        string nameLower = *tokens.get(2);
                        transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
                        if (nameLower.find(searchTerm) != string::npos) match = true;
                    }
                    else if (searchChoice == 3 && tokens.getSize() > 3) {
                        string emailLower = *tokens.get(3);
                        transform(emailLower.begin(), emailLower.end(), emailLower.begin(), ::tolower);
                        if (emailLower.find(searchTerm) != string::npos) match = true;
                    }
                    else if (searchChoice == 4 && tokens.getSize() > 4) {
                        string phoneLower = *tokens.get(4);
                        if (phoneLower.find(searchTerm) != string::npos) match = true;
                    }

                    if (match) {
                        searchResults.insertAtEnd(customer);
                    }
                }
            }

            // Display search results
            system("cls");
            cout << "\n\t\t" << string(90, '=') << "\n";
            cout << "\t\t              LINEAR SEARCH RESULTS (" << searchResults.getSize() << " found)\n";
            cout << "\t\t" << string(90, '=') << "\n";

            if (searchResults.isEmpty()) {
                cout << "\n\t\tNo matching customers found.\n";
            } else {
                cout << left << setw(7) << "\t\tNO" << left << setw(20) << "USERNAME"
                     << left << setw(25) << "FULL NAME" << left << setw(25) << "EMAIL"
                     << left << setw(15) << "PHONE" << endl;
                cout << "\t\t" << string(90, '-') << "\n";

                int i = 1;
                auto resultIt = searchResults.begin();
                while (resultIt.hasNext()) {
                    string customer = resultIt.next();
                    LinkedList<string> tokens = splitString(customer, '|');
                    if (tokens.getSize() >= 5) {
                        string username = *tokens.get(0);
                        string displayUsername = username.length() > 18 ? username.substr(0, 18) + ".." : username;

                        string name = *tokens.get(2);
                        string displayName = name.length() > 23 ? name.substr(0, 23) + ".." : name;

                        string email = *tokens.get(3);
                        string displayEmail = email.length() > 23 ? email.substr(0, 23) + ".." : email;

                        string phone = *tokens.get(4);

                        cout << left << "\t\t" << setw(5) << i++
                             << left << setw(20) << displayUsername
                             << left << setw(25) << displayName
                             << left << setw(25) << displayEmail
                             << left << setw(15) << phone << endl;
                    }
                }
            }

            cout << "\t\t" << string(90, '=') << "\n";
            waitForEnter();
        }

        //Sort (Bubble Sort)
        else if (choice == '2') {
            // BUBBLE SORT
            LinkedList<string> currentCustomers = loadCustomers();
            if (currentCustomers.isEmpty()) {
                cout << "\n\t\tNo customers to sort!\n";
                waitForEnter();
                continue;
            }

            int sortChoice;
            cout << "\n\t\tSort customers by:\n";
            cout << "\t\t1. Username (A-Z)\n";
            cout << "\t\t2. Full Name (A-Z)\n";
            cout << "\t\t3. Cancel\n";
            cout << "\n\t\tChoice: ";
           string input;
           getline(cin,input);

            // Empty input
            if (input.empty()) {
                cout << "\n\t\tError: Choice cannot be empty!\n";
                waitForEnter();
                continue;
            }

            // Must be numeric

            try {
                sortChoice = stoi(input);
            } catch (...) {
                cout << "\n\t\tError: Please enter a valid number!\n";
                waitForEnter();
                continue;
            }

            // Range check
            if (sortChoice < 1 || sortChoice > 3) {
                cout << "\n\t\tError: Invalid choice! Please enter from 1 to 3.\n";
                waitForEnter();
                continue;
            }
             // Cancel
                if (sortChoice == 3) {
                    continue;
                }

            // Perform Bubble Sort
            if (sortChoice == 1) {
                // Sort by Username
                bool swapped;
                int n = currentCustomers.getSize();
                for (int i = 0; i < n - 1; i++) {
                    swapped = false;
                    for (int j = 0; j < n - i - 1; j++) {
                        string* cust1 = currentCustomers.get(j);
                        string* cust2 = currentCustomers.get(j + 1);

                        if (cust1 && cust2) {
                            LinkedList<string> tokens1 = splitString(*cust1, '|');
                            LinkedList<string> tokens2 = splitString(*cust2, '|');

                            if (tokens1.getSize() >= 1 && tokens2.getSize() >= 1) {
                                string username1 = *tokens1.get(0);
                                string username2 = *tokens2.get(0);

                                if (username1 > username2) {
                                    // Swap customers
                                    string temp = *cust1;
                                    *cust1 = *cust2;
                                    *cust2 = temp;
                                    swapped = true;
                                }
                            }
                        }
                    }
                    if (!swapped) break;
                }
            }
            else if (sortChoice == 2) {
                // Sort by Full Name
                bool swapped;
                int n = currentCustomers.getSize();
                for (int i = 0; i < n - 1; i++) {
                    swapped = false;
                    for (int j = 0; j < n - i - 1; j++) {
                        string* cust1 = currentCustomers.get(j);
                        string* cust2 = currentCustomers.get(j + 1);

                        if (cust1 && cust2) {
                            LinkedList<string> tokens1 = splitString(*cust1, '|');
                            LinkedList<string> tokens2 = splitString(*cust2, '|');

                            if (tokens1.getSize() >= 3 && tokens2.getSize() >= 3) {
                                string name1 = *tokens1.get(2);
                                string name2 = *tokens2.get(2);

                                if (name1 > name2) {
                                    // Swap customers
                                    string temp = *cust1;
                                    *cust1 = *cust2;
                                    *cust2 = temp;
                                    swapped = true;
                                }
                            }
                        }
                    }
                    if (!swapped) break;
                }
            }

            // Save sorted list back to file
            clearCustomerFile();
            auto saveIt = currentCustomers.begin();
            while (saveIt.hasNext()) {
                string cust = saveIt.next();
                if (!cust.empty()) {
                    saveCustomer(cust);
                }
            }

            cout << "\n\t\tCustomers sorted successfully using Bubble Sort!\n";
            waitForEnter();
        }

        // DELETE CUSTOMER
        else if (choice == '3') {
            LinkedList<string> currentCustomers = loadCustomers();
            if (currentCustomers.isEmpty()) {
                cout << "\nNo customers to delete!\n";
                waitForEnter();
                continue;
            }

            // Display customers for deletion
            system("cls");
            cout << "\n\t\t" << string(90, '=') << "\n";
            cout << "\t\t                                    DELETE CUSTOMER\n";
            cout << "\t\t" << string(90, '=') << "\n";

            cout << left << setw(7) << "\t\tNO" << left << setw(20) << "USERNAME"
                 << left << setw(25) << "FULL NAME" << left << setw(25) << "EMAIL" << endl;
            cout << "\t\t" << string(90, '-') << "\n";

            int displayNum = 1;
            auto dispIt = currentCustomers.begin();
            while (dispIt.hasNext()) {
                string customer = dispIt.next();
                LinkedList<string> tokens = splitString(customer, '|');
                if (tokens.getSize() >= 5) {
                    string username = *tokens.get(0);
                    string displayUsername = username.length() > 18 ? username.substr(0, 18) + ".." : username;

                    string name = *tokens.get(2);
                    string displayName = name.length() > 23 ? name.substr(0, 23) + ".." : name;

                    string email = *tokens.get(3);
                    string displayEmail = email.length() > 23 ? email.substr(0, 23) + ".." : email;

                    cout << left << "\t\t" << setw(5) << displayNum++
                         << left << setw(20) << displayUsername
                         << left << setw(25) << displayName
                         << left << setw(25) << displayEmail << endl;
                }
            }

            string input;
            int idx;
            cout << "\t\t" << string(90, '=') << "\n";
            cout << "\n\t\tEnter customer number to delete (1-" << currentCustomers.getSize() << "): ";
            getline(cin,input);

            // Empty check
            if (input.empty()) {
                cout << "\n\t\tError: Input cannot be empty!\n";
                waitForEnter();
                continue;
            }

            // Numeric check
            try {
                idx = stoi(input);
            } catch (...) {
                cout << "\n\t\tError: Please enter a valid number!\n";
                waitForEnter();
                continue;
            }

            // Range check
            if (idx < 1 || idx > currentCustomers.getSize()) {
                cout << "\n\t\tError: Invalid customer number!\n";
                waitForEnter();
                continue;
            }

            //Get customer
                string customerStr = *currentCustomers.get(idx - 1);
                LinkedList<string> tokens = splitString(customerStr, '|');

                if (tokens.getSize() >= 3) {
                    string username = *tokens.get(0);
                    string name = *tokens.get(2);

                    // Check if it's the admin account
                    if (username == "admin") {
                        cout << "\n\t\tCannot delete the admin account!\n";
                        waitForEnter();
                        continue;
                    }

                    //confirmation
                    char confirm;
                    cout << "\n\t\tWARNING: This will delete customer '" << name << "' (" << username << ") permanently!\n";
                    cout << "\t\tThis action cannot be undone.\n";
                    cout << "\t\tAre you sure? (y/n): ";
                    cin >> confirm;

                    if (tolower(confirm) == 'y') {
                        // Delete from customers list
                        currentCustomers.deleteNode(idx - 1);

                        // Update file
                        clearCustomerFile();
                        auto saveIt = currentCustomers.begin();
                        while (saveIt.hasNext()) {
                            string cust = saveIt.next();
                            if (!cust.empty()) {
                                saveCustomer(cust);
                            }
                        }

                        // Remove from in-memory user list
                        auto users = getGlobalUsers();
                        bool removed = false;
                        for (int i = 0; i < users->getSize(); i++) {
                            User* user = users->get(i);
                            if (user && user->getUsername() == username) {
                                users->deleteNode(i);
                                removed = true;
                                break;
                            }
                        }

                        if (removed) {
                            cout << "\n\t\tCustomer '" << name << "' deleted successfully from both file and memory!\n";
                        } else {
                            cout << "\n\t\tCustomer deleted from file but not found in memory.\n";
                        }
                    } else {
                        cout << "\n\t\tDeletion cancelled.\n";
                    }

                cout << "\n\t\tInvalid customer number!\n";
            }
            waitForEnter();
    }
          // VIEW CUSTOMER DETAILS
        else if (choice == '4') {
            LinkedList<string> currentCustomers = loadCustomers();
            if (currentCustomers.isEmpty()) {
                cout << "\n\t\tNo customers to view!\n";
                waitForEnter();
                continue;
            }

            int idx;
            string input;
            cout << "\n\t\tEnter customer number to view details (1-" << currentCustomers.getSize() << "): ";
             getline(cin, input);

            // Empty input
            if (input.empty()) {
                cout << "\n\t\tError: Customer number cannot be empty!\n";
                waitForEnter();
                continue;
            }

            // Numeric check
            try {
                idx = stoi(input);
            } catch (...) {
                cout << "\n\t\tError: Please enter a valid number!\n";
                waitForEnter();
                continue;
            }

            // Range check
            if (idx < 1 || idx > currentCustomers.getSize()) {
                cout << "\n\t\tError: Invalid customer number!\n";
                waitForEnter();
                continue;
            }

                string customerStr = *currentCustomers.get(idx - 1);
                LinkedList<string> tokens = splitString(customerStr, '|');

                system("cls");
                cout << "\n\t\t" << string(60, '=') << "\n";
                cout << "\t\t                       CUSTOMER DETAILS\n";
                cout << "\t\t" << string(60, '=') << "\n";

                if (tokens.getSize() >= 5) {
                    cout << left << setw(25) << "\t\tUsername:" << *tokens.get(0) << endl;
                    cout << left << setw(25) << "\t\tPassword:" << "[HIDDEN]" << endl;
                    cout << left << setw(25) << "\t\tFull Name:" << *tokens.get(2) << endl;
                    cout << left << setw(25) << "\t\tEmail:" << *tokens.get(3) << endl;
                    cout << left << setw(25) << "\t\tPhone:" << *tokens.get(4) << endl;

                    // Check if admin (field 5)
                    string adminStatus = "Customer";
                    if (tokens.getSize() > 5) {
                        string adminFlag = *tokens.get(5);
                        if (adminFlag == "1" || adminFlag == "true" || adminFlag == "admin") {
                            adminStatus = "Admin";
                        }
                    }
                    cout << left << setw(25) << "\t\tAccount Type:" << adminStatus << endl;

                    // Check if active (field 6)
                    string status = "Active";
                    if (tokens.getSize() > 6) {
                        string activeFlag = *tokens.get(6);
                        if (activeFlag == "0" || activeFlag == "false" || activeFlag == "inactive") {
                            status = "Inactive";
                        }
                    }
                    cout << left << "\t\t" << setw(25) << "Status:" << status << endl;
                }

                // Show customer's orders
                LinkedList<string> orders = loadOrders();
                int orderCount = 0;
                float totalSpent = 0;

                auto orderIt = orders.begin();
                while (orderIt.hasNext()) {
                    string order = orderIt.next();
                    LinkedList<string> orderTokens = splitString(order, '|');
                    if (orderTokens.getSize() >= 5 && *orderTokens.get(4) == *tokens.get(0)) {
                        orderCount++;
                        try {
                            float price = stof(*orderTokens.get(1));
                            int qty = stoi(*orderTokens.get(2));
                            totalSpent += price * qty;
                        } catch (...) {}

                    }
                }

                cout << "\t\t" << string(60, '-') << "\n";
                cout << left << setw(25) << "\t\tTotal Orders:" << orderCount << endl;
                cout << left << setw(25) << "\t\tTotal Spent:" << "RM " << fixed << setprecision(2) << totalSpent << endl;
                cout << "\t\t" << string(60, '=') << "\n";

            waitForEnter();
        }

    } while (choice != '0');
}

