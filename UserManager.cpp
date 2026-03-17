#include "UserManager.h"
#include <fstream>
#include <iostream>
#include <string>
#include "Utilities.h"

using namespace std;

// Static member initialization
LinkedList<User>* UserManager::users = nullptr;

// Global variable definition
LinkedList<User>* GlobalUsers = nullptr;

// Get singleton instance
LinkedList<User>* UserManager::getInstance() {
    if (!users) {
        users = new LinkedList<User>();
        // Initialize GlobalUsers
        GlobalUsers = users;
    }
    return users;
}

// Cleanup static instance (call before program exit)
void UserManager::cleanup() {
    if (users) {
        delete users;
        users = nullptr;
        GlobalUsers = nullptr;
    }
}

// Global accessor function for user list
LinkedList<User>* getGlobalUsers() {
    return UserManager::getInstance();
}

// ==================== INITIALIZE ADMIN USER ====================
// Creates default admin account if it doesn't exist
void initializeAdminUser() {
    auto users = getGlobalUsers();
    if (!users) return;

    bool adminExists = false;

    // Check if admin already exists
    auto it = users->begin();
    while (it.hasNext()) {
        User& user = it.next();
        if (user.getUsername() == "admin") {
            adminExists = true;
            break;
        }
    }

    // Create default admin if not exists
    if (!adminExists) {
        users->insertAtEnd(User("admin", "admin123", true, "System Administrator",
                               "admin@restaurant.com", "012-3456789"));
    }
}

// ==================== LOAD USERS FROM FILE ====================
// Loads user data from customers.txt file into memory
void loadUsersFromFile() {
    ifstream file(CUSTOMERS_FILE);
    if (!file.is_open()) {
        return;
    }

    auto users = getGlobalUsers();
    if (!users) return;

    string line;

    // Read each line from file
    while (getline(file, line)) {
        if (line.empty()) continue;

        // Parse line using pipe delimiter
        LinkedList<string> tokens = splitString(line, '|');
        if (tokens.getSize() >= 5) {
            string username = *tokens.get(0);
            string password = *tokens.get(1);
            string name = *tokens.get(2);
            string email = *tokens.get(3);
            string phone = *tokens.get(4);

            bool isAdmin = false;
            bool isActive = true;

            // Parse admin flag if exists
            if (tokens.getSize() >= 6) {
                string adminFlag = *tokens.get(5);
                isAdmin = (adminFlag == "1" || adminFlag == "true" || adminFlag == "admin");
            }

            // Parse active flag if exists
            if (tokens.getSize() >= 7) {
                string activeFlag = *tokens.get(6);
                isActive = (activeFlag == "1" || activeFlag == "true" || activeFlag == "active");
            }

            // Check if user already exists in memory
            bool exists = false;
            auto it = users->begin();
            while (it.hasNext()) {
                User& user = it.next();
                if (user.getUsername() == username) {
                    exists = true;
                    break;
                }
            }

            // Add user if not duplicate
            if (!exists) {
                users->insertAtEnd(User(username, password, isAdmin, name, email, phone, isActive));
            }
        }
    }
    file.close();
}
