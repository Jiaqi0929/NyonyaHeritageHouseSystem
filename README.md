# Nyonya Heritage House Food Ordering & Reservation System

This project is a restaurant management system for Nyonya Heritage House that allows customers to place food orders, make reservations, and enables staff to manage orders, reservations, and customer information efficiently. The system is built using C++ and demonstrates the use of file handling, data structures, and object-oriented programming.

## ✨ Features

### Customer Panel:
- Register/Login to account
- Browse menu items
- Place food orders
- Make table reservations

### Admin Panel:
- Secure admin login
- Add, update, or remove menu items
- View all customer orders
- Manage reservations

## 🛠️ Technologies Used
- **Language:** C++
- **Concepts:** OOP (Classes, Inheritance, Polymorphism)
- **Data Structures:** Linked Lists, Queues
- **Algorithms:** Bubble Sort, Linear Search
- **Data Storage:** File handling (text files)

## 📁 Project Structure

| File / Component | Description |
|------------------|-------------|
| main.cpp | Program entry point |
| Customer.cpp / .h | Customer account management |
| OrderManager.cpp / .h | Order processing logic |
| Reservation.cpp / .h | Reservation handling |
| MenuItem.cpp / .h | Menu management |
| utils.cpp / .h | Helper functions |
| LinkedList / Queue | Data structures used in the system |
| customers.txt | Customer database |
| menu.txt | Menu items |
| orders.txt | Order records |
| reservations.txt | Reservation records |

## 🔧 How to Run

1. **Software Requirement:** Code::Blocks IDE with a C++ compiler (MinGW / GNU GCC).
2. Extract the project folder if it is in a .zip file.
3. Open Code::Blocks.
4. Click **File → Open** and navigate to the project folder.
5. Open the project file: `Lab1B_Group05.cbp` (this will load all source files).
6. Press `F9` or click **Build → Build and Run**.
7. Ensure all `.txt` data files (`customers.txt`, `menu.txt`, `orders.txt`, `reservations.txt`) remain in the same folder as the executable.
8. When orders are completed, receipt files (`receipt_*.txt`) will be generated in the project folder.

## 💡 What I Learned
- Implementing sorting and searching algorithms (bubble sort, linear search) in a real-world application
- Designing separate interfaces for different user types (customer vs admin)
- Managing data persistence through file handling
- Building a complete system from scratch using OOP principles
- Using linked lists and queues to manage data efficiently

## 📝 Note
- The program uses file handling to store and retrieve system data.
- All source code is available in this repository.
- Receipts are generated automatically upon order completion.
