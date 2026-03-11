# Nyonya Heritage House Food Ordering and Reservation System

## 1. Software Requirement
- **Code::Blocks IDE**
- **C++ Compiler (MinGW / GNU GCC)**

Make sure Code::Blocks is installed with a working C++ compiler.

---

## 2. How to Open the Project

1. Extract the project folder if it is in a `.zip` file.
2. Open **Code::Blocks**.
3. Click **File → Open**.
4. Navigate to the project folder.
5. Open the project file: Lab1B_Group05.cbp

This file will automatically load all source files into Code::Blocks.

---

## 3. How to Run the Program

1. After opening the `.cbp` project file, click **Build and Run**.

   **OR**

2. Press the shortcut: F9

Code::Blocks will:
- Compile all `.cpp` files
- Build the program
- Run the system in the console.

---

## 4. Required Data Files

The following text files must remain in the **same folder as the project** because the program reads data from them:
- customers.txt
- menu.txt
- orders.txt
-reservations.txt

These files store system data such as customers, menu items, orders, and reservations.

---

## 5. Program Output

- The system runs in the **console window**.
- When orders are completed, **receipt files** may be generated automatically such as: receipt_*.txt

These receipts will appear in the project folder.

---

## 6. Project Structure

Important files in the project:

| File / Component | Description |
|------------------|-------------|
| `main.cpp` | Main program execution |
| Customer / User Classes | Manage customer accounts |
| OrderManager | Handle food orders |
| LinkedList / Queue | Data structures used in the system |
| Utilities | Helper functions |

---

## Notes
- Ensure all required `.txt` files are present before running the program.
- The program uses **file handling** to store and retrieve system data.




