#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "LinkedList.h"
#include "User.h"
#include <string>

// File paths
#define CUSTOMERS_FILE "customers.txt"

// Declare GlobalUsers as external
extern LinkedList<User>* GlobalUsers;

// ==================== SINGLETON USER LIST ====================
// Singleton pattern to ensure only one UserManager instance exists
class UserManager {
private:
    static LinkedList<User>* users;
    UserManager() {}
    UserManager(const UserManager&) = delete;
    UserManager& operator=(const UserManager&) = delete;

public:
    static LinkedList<User>* getInstance();
    static void cleanup();
};

// Function declarations
LinkedList<User>* getGlobalUsers();
void initializeAdminUser();
void loadUsersFromFile();

#endif
