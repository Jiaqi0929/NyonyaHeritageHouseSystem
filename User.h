#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

// ==================== USER CLASS ====================
// Represents a user of the system (customer or admin)
class User {
    string username;
    string password;
    bool isAdmin;
    string fullName;
    string email;
    string phone;
    bool activeStatus;

public:
    // Constructor with default values
    User(string u = "", string p = "", bool admin = false, string name = "",
         string e = "", string ph = "", bool active = true)
        : username(u), password(p), isAdmin(admin), fullName(name),
          email(e), phone(ph), activeStatus(active) {}

    // ==================== GETTER METHODS ====================
    string getFullName() const { return fullName; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    string getUsername() const { return username; }

    // Verify password and account status
    bool checkPassword(string p) const {
        return password == p && activeStatus;
    }

    bool getIsAdmin() const { return isAdmin; }

    // ==================== SETTER METHODS ====================
    void setFullName(string name) { fullName = name; }
    void setEmail(string e) { email = e; }
    void setPhone(string ph) { phone = ph; }
    void setPassword(string p) { password = p; }
    void setIsAdmin(bool admin) { isAdmin = admin; }

    // Account status methods
    bool isActive() const { return activeStatus; }
    void setActive(bool status) { activeStatus = status; }
};

#endif
