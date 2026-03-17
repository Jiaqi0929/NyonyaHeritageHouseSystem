#include "Constants.h"
#include "Utilities.h"
#include "UserManager.h"
#include "LinkedList.h"
#include "User.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <cctype>
#include <cstdlib>
#include <limits>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <ctime>
#include <conio.h>
#include <utility>
using namespace std;

int main() {
    // Seed random number generator (used for reservation ID)
    srand(static_cast<unsigned int>(time(0)));

    // Change console color and show loading animation
    system("Color E4");
    blinkMessage("Loading Nyonya Heritage House System...");

    // Load existing data
    loadUsersFromFile();
    initializeAdminUser();

    // Load menu from file
    // Create linked lists for food and beverage menu
    LinkedList<Food> foodMenuVec;
    LinkedList<Beverage> bevMenuVec;
    loadMenuFromFile(foodMenuVec, bevMenuVec);

    // If file is empty, initialize with default menu
    if (foodMenuVec.isEmpty() && bevMenuVec.isEmpty()) {
        foodMenuVec.insertAtEnd(Food("Ayam Pongteh", 25.00));
        foodMenuVec.insertAtEnd(Food("Nyonya Laksa", 18.00));
        foodMenuVec.insertAtEnd(Food("Chap Chye", 15.00));
        foodMenuVec.insertAtEnd(Food("Sambal Belacan", 20.00));
        foodMenuVec.insertAtEnd(Food("Otak-Otak", 23.00));
        foodMenuVec.insertAtEnd(Food("Ayam Buah Keluak", 25.00));
        foodMenuVec.insertAtEnd(Food("Udang Lemak Nanas", 30.00));
        foodMenuVec.insertAtEnd(Food("Nyonya Popiah", 10.00));
        foodMenuVec.insertAtEnd(Food("Asam Pedas", 23.00));
        foodMenuVec.insertAtEnd(Food("Nyonya Chicken Curry", 23.00));

        bevMenuVec.insertAtEnd(Beverage("Nyonya kopi", 3.50, 'H'));
        bevMenuVec.insertAtEnd(Beverage("Sirap bandung", 4.00, 'C'));
        bevMenuVec.insertAtEnd(Beverage("Cendol", 9.00, 'C'));
        bevMenuVec.insertAtEnd(Beverage("Bubur Cha Cha", 7.00, 'H'));
        bevMenuVec.insertAtEnd(Beverage("Nyonya teh", 3.50, 'H'));
        bevMenuVec.insertAtEnd(Beverage("Kedondong Asam Boi", 6.50, 'C'));
        bevMenuVec.insertAtEnd(Beverage("Bubur Pulut Hitam", 7.00, 'H'));
        bevMenuVec.insertAtEnd(Beverage("Limau ais", 5.00, 'C'));
        bevMenuVec.insertAtEnd(Beverage("Air kelapa", 8.00, 'C'));
        bevMenuVec.insertAtEnd(Beverage("Barli ais", 5.00, 'C'));

         // Insert default food and beverage items
        saveMenuToFile(foodMenuVec, bevMenuVec);
    }

    // Main program loop
    while (true) {
        system("cls");
        cout << "\n\t\t" << string(60, '=') << "\n";
        cout << "\t\t          WELCOME TO NYONYA HERITAGE HOUSE SYSTEM !\n";
        cout << "\t\t               Operating Hours: 11 AM - 9 PM \n";
        cout << "\t\t" << string(60, '=') << "\n";
        cout << "\t\tSelect your role:\n";
        cout << "\t\t1. Customer\n";
        cout << "\t\t2. Admin\n";
        cout << "\t\t3. Exit Program\n";
        cout << "\t\t" << string(60, '=') << "\n";
        cout << "\t\tEnter choice (1-3): ";

        string roleChoice;
        getline(cin, roleChoice);

        // Validate input
        if (roleChoice != "1" && roleChoice != "2" && roleChoice != "3") {
            cout << "\n\t\tInvalid choice! Please enter 1, 2, or 3.\n";
            waitForEnter();
            continue;
        }

        if (roleChoice == "3") {
            system("cls");
            cout << "\n\t\t" << string(60, '=') << "\n";
            cout << "\t\t    Thank you for using Nyonya Heritage House System!\n";
            cout << "\t\t               Program terminated successfully.\n";
            cout << "\t\t" << string(60, '=') << "\n\n";
            break;
        }

        // ================= ADMIN FLOW =================
        if (roleChoice == "2") {
            User* loggedInUser = nullptr;
            int loginAttempts = 0;

            do {
                system("cls");
                cout << "\n\t\t" << string(40, '=') << "\n";
                cout << "\t\t           ADMIN LOGIN\n";
                cout << "\t\t" << string(40, '=') << "\n";
                cout << "\t\tAttempt " << loginAttempts + 1 << " of 3\n";
                cout << "\t\t" << string(40, '-') << "\n";

                loggedInUser = login(true);
                if (loggedInUser) {
                    break;
                } else {
                    loginAttempts++;
                    if (loginAttempts < 3) {
                        cout << "\n\t\tPress any key to try again...";
                        cin.get();
                    } else {
                        cout << "\n\t\tToo many failed attempts! Returning to main menu...\n";
                        waitForEnter();
                        break;
                    }
                }
            } while (loginAttempts < 3);

            if (loggedInUser && loggedInUser->getIsAdmin()) {
                OrderManager manager;
                adminMenu(manager, foodMenuVec, bevMenuVec);
            }
            continue;

          // ================= CUSTOMER FLOW =================
        } else if (roleChoice == "1") {
            char loginChoice;
            bool loggedIn = false;
            User* loggedInUser = nullptr;

            // Customer login/signup loop
            do {
                system("cls");
                cout << "\n\t\t" << string(60, '=') << "\n";
                cout << "\t\t                    CUSTOMER PORTAL\n";
                cout << "\t\t" <<string(60, '=') << "\n";
                cout << "\t\t1. Login\n\t\t2. Sign Up\n\t\t3. Back to Role Selection\n";
                cout << "\t\t" << string(60, '=') << "\n";
                cout << "\t\tEnter choice: ";

                // Clear any previous input errors
                cin.clear();

                // Read the entire line
                string input;
                getline(cin, input);

                // Validate input
                if (input.length() != 1) {
                    cout << "\n\t\tInvalid option. Please enter only 1, 2, or 3.\n";
                    waitForEnter();
                    continue;
                }

                loginChoice = input[0];

                // Validate the single character
                if (loginChoice != '1' && loginChoice != '2' && loginChoice != '3') {
                    cout << "\n\t\tInvalid option. Please enter only 1, 2, or 3.\n";
                    waitForEnter();
                    continue;
                }

                if (loginChoice == '1') {
                    system("cls");
                    cout << "\n\t\t" << string(40, '=') << "\n";
                    cout << "\t\t           CUSTOMER LOGIN\n";
                    cout << "\t\t" << string(40, '=') << "\n";
                    loggedInUser = login();
                    if (loggedInUser) {
                        if (loggedInUser->getIsAdmin()) {
                            cout << "\n\t\tPlease login through admin portal.\n";
                            waitForEnter();
                        } else {
                            loggedIn = true;
                        }
                    } else {
                        cout << "\n\t\tPress any key to try again...";
                        cin.get();
                    }
                } else if (loginChoice == '2') {
                    system("cls");
                    cout << "\n\t\t" << string(60, '=') << "\n";
                    cout << "\t\t                        CUSTOMER SIGN UP\n";
                    cout << "\t\t" << string(60, '=') << "\n";
                    if (signUp(false)) {
                        cout << "\n\t\tPress any key to login with your new account...";
                        cin.get();
                    } else {
                        waitForEnter();
                    }
                } else if (loginChoice == '3') {
                    break;
                } else {
                    cout << "\n\t\tInvalid option.\n";
                    waitForEnter();
                }

            } while (!loggedIn);

            if (loginChoice == '3') continue;

            OrderManager manager;
            manager.setCustomer(loggedInUser->getUsername());
            Customer customer;
            bool hasReservation = false;

            char choice;
            do {
                system("cls");
                cout << "\n\t\t" << string(50, '=') << "\n";
                cout << "\t\t                    MAIN MENU\n";
                cout << "\t\t                Welcome, " << loggedInUser->getUsername() << "!\n";
                cout << "\t\t" << string(50, '=') << "\n";
                cout << "\t\t1. View Menu\n";
                cout << "\t\t2. Order Food\n";
                cout << "\t\t3. Make Reservation\n";
                cout << "\t\t4. View My Reservations\n";
                cout << "\t\t5. Checkout\n";
                cout << "\t\t6. View Profile\n";
                cout << "\t\t0. Logout\n";
                cout << "\t\t" << string(50, '=') << "\n";
                cout << "\t\tEnter choice: ";
                string input;
                getline(cin, input);

                if (input.empty() || input.length() != 1 || !isdigit(input[0])) {
                    cout << "\n\t\tInvalid choice!\n";
                    waitForEnter();
                    continue;
                }

                choice = input[0];

                switch (choice) {
                    case '1':
                        system("cls");
                        displayFoodMenu(foodMenuVec);
                        displayBeverageMenu(bevMenuVec);
                        waitForEnter();
                        break;

                    case '2':
                        orderManagement(manager, foodMenuVec, bevMenuVec);
                        break;

                    case '3': {
                        // Create new reservation
                        system("cls");
                        cout << "\n\t\t" << string(40, '=') << "\n";
                        cout << "\t\t            MAKE RESERVATION\n";
                        cout << "\t\t" << string(40, '=') << "\n";

                        Customer newCustomer(loggedInUser);
                        newCustomer.inputBookingDetails(loggedInUser);

                        // Generate unique reservation ID
                        int newResId;
                        do {
                            newResId = rand() % 9000 + 1000;
                        } while (!isReservationIdUnique(newResId));
                        newCustomer.setReservationId(newResId);

                        customer = newCustomer;
                        hasReservation = true;
                        manager.setReservationId(newResId);

                        // Save reservation to file
                        saveReservation(customer.getBookingData());
                        cout << "\t\t" << string(40, '=') << "\n";
                        cout << "\n\t\tReservation created successfully!\n";
                        cout << "\t\tReservation ID: " << newResId << endl;

                        waitForEnter();
                        break;
                    }

                    case '4':
                        viewCustomerReservations(loggedInUser->getUsername());
                        break;

                    case '5':
                        checkout(manager, customer, hasReservation, loggedInUser->getUsername());
                        break;

                    case '6':
                        viewCustomerProfile(loggedInUser);
                        break;

                    case '0':
                        cout << "\n\t\tLogging out...\n";
                        waitForEnter();
                        break;

                    default:
                        cout << "\n\t\tInvalid choice!\n";
                        waitForEnter();
                }

            } while (choice != '0');
        } else {
            cout << "\n\t\tInvalid choice! Please enter 1, 2, or 3.\n";
            waitForEnter();
        }
    }

    // Free memory and cleanup before exit
    UserManager::cleanup();

    return 0;
}
