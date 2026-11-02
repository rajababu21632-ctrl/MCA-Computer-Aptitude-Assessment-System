# MCA Computer Aptitude Assessment & Analytics System (CU)
> **Course:** Master of Computer Applications (MCA General)  
> **Institution:** Chandigarh University (CU)  
> **Subject:** Computer Aptitude / C++ Object-Oriented Software Engineering  
> **Author / Student:** Raja Babu  
> **Language & Standard:** C++17  

---

## 📌 1. Project Overview & Objective

The **MCA Computer Aptitude Assessment & Analytics System** is a modular, high-performance console-based assessment and evaluation platform designed specifically for the **Computer Aptitude** curriculum in the MCA program at Chandigarh University.

It bridges core computer science theory with practical Object-Oriented Programming (OOP) in C++. The platform features separate role-based workflows for **Students** and **Faculty/Administrators**, comprehensive test modules, instant diagnostic feedback, performance analytics, and file persistence.

---

## 🎯 2. Assessment Modules Covered

The question repository contains questions covering the core domains of the CU MCA Computer Aptitude syllabus:
1. **Computer Fundamentals & Digital Logic:**
   - 2's Complement and binary arithmetic
   - CPU Registers (PC, AC, IR, MDR) & Von Neumann architecture
   - Cache memory mapping (Direct, Associative, Set-Associative)
   - Boolean algebra simplification & logic gates
2. **C++ & Object-Oriented Programming (OOP):**
   - Virtual destructors and memory safety
   - `dynamic_cast` vs `static_cast` & RTTI
   - Constructor/destructor inheritance and operator overloading
   - Exception handling & stack unwinding
3. **Data Structures & Algorithms:**
   - Time & space complexity analysis (Big-O notation)
   - Binary Search Tree (BST) worst-case vs balanced traversal
   - Stack applications (nested parsing, bracket matching)
   - Sorting algorithm stability and comparison bounds
4. **Operating Systems & System Software:**
   - Coffman deadlock conditions & prevention
   - Virtual memory, paging, and thrashing
   - User-Level Threads (ULT) vs Kernel-Level Threads (KLT)
   - CPU scheduling algorithms & Convoy effect
5. **Database Management Systems (DBMS):**
   - 1NF, 2NF, 3NF, and BCNF normalization
   - ACID transaction properties
   - DDL vs DML commands (`DROP` vs `TRUNCATE`)
   - B+ Tree indices and query performance

---

## 🏛️ 3. Software Architecture & OOP Concepts

| OOP / C++ Concept | Implementation in Project |
| :--- | :--- |
| **Abstraction** | Abstract base class `User` hiding sensitive credential logic and providing a clean API interface. |
| **Inheritance** | Derived classes `Student : public User` and `Admin : public User`. |
| **Polymorphism** | Pure virtual function `virtual void displayMenu() const = 0` overridden dynamically by derived classes (`dynamic_pointer_cast`). |
| **Encapsulation** | Private attributes in `Question`, `Student`, `Admin`, `ResultRecord` with validated getters and setters. |
| **File I/O Persistence** | Dedicated text-based storage engines (`data/questions.txt`, `data/users.txt`, `data/results.txt`) with custom delimiters (`|`). |
| **Standard Template Library (STL)** | `std::vector`, `std::map`, `std::shared_ptr`, `std::sort` with custom lambdas, `std::istringstream`, `std::ostringstream`. |
| **Robust Input Sanitization** | `Utils::getSafeInt`, `Utils::getSafeChar`, `Utils::getSafeString` preventing infinite input stream buffer overflow. |

---

## 📂 4. Project Directory Structure

```text
Aptitude_Testing_System_MCA/
├── build.bat               # Windows one-click compilation script
├── run.bat                 # Windows one-click execution script
├── README.md               # Complete Project Report & Viva Guide
├── data/                   # Persistent Data Storage
│   ├── questions.txt       # Question bank with technical explanations
│   ├── results.txt         # Student submissions & assessment logs
│   └── users.txt           # Registered student & admin accounts
├── include/                # Header Files (.h)
│   ├── Admin.h             # Faculty/Admin role class definition
│   ├── AuthManager.h       # Authentication & user registration manager
│   ├── ExamEngine.h        # Test generation, quiz session & grading
│   ├── Question.h          # Question model & serialization
│   ├── ResultManager.h     # Scorecard generation, analytics & leaderboard
│   ├── Student.h           # Student role class definition
│   ├── User.h              # Base User abstract class
│   └── Utils.h             # UI formatting, ANSI colors & input helpers
└── src/                    # Source Files (.cpp)
    ├── Admin.cpp
    ├── AuthManager.cpp
    ├── ExamEngine.cpp
    ├── Question.cpp
    ├── ResultManager.cpp
    ├── Student.cpp
    ├── User.cpp
    ├── Utils.cpp
    └── main.cpp            # Application entry point & main routing loop
```

---

## 🚀 5. How to Build & Run

### Prerequisites
- Operating System: Windows 10 / 11
- Compiler: MinGW-w64 / GCC (`g++`) with C++17 support

### Method 1: Using Batch Scripts (Easiest)
1. Double-click `build.bat` to compile the source code into `exam_system.exe`.
2. Double-click `run.bat` to start the program in your terminal.

### Method 2: Manual Compilation via Command Prompt / PowerShell
Open terminal in the project directory and run:
```bash
g++ -std=c++17 src/*.cpp -Iinclude -o exam_system.exe
.\exam_system.exe
```

---

## 🔑 6. Default Credentials for Viva & Demonstration

| Role | Username | Password | Notes / Identifiers |
| :--- | :--- | :--- | :--- |
| **Faculty / Admin** | `admin` | `admin123` | ID: `CU-MCA-FAC01` (Prof. R. K. Sharma) |
| **Student (Demo 1)** | `student` | `pass123` | UID: `24MCA10250` (Rahul Sharma) |
| **Student (Demo 2)** | `priya` | `pass123` | UID: `24MCA10284` (Priya Verma) |

*You can also register new student accounts on-the-fly using Option [2] in the main menu.*

---

## 🎓 7. Common Viva Voce Questions & Answers

**Q1: What are pure virtual functions and why are they used in this project?**  
> *Ans:* A pure virtual function is defined with `= 0` in the base class (e.g., `virtual void displayMenu() const = 0` in `User.h`). It makes `User` an Abstract Base Class, preventing direct instantiation and forcing every derived class (`Student`, `Admin`) to implement its own customized dashboard menu.

**Q2: How is data persisted when the application closes?**  
> *Ans:* The application uses C++ File Streams (`std::ifstream` and `std::ofstream`). Question records, user credentials, and assessment results are serialized into delimited string formats (`|` delimiter) and written to the `data/` directory.

**Q3: How does the system prevent input validation crashes?**  
> *Ans:* Standard `cin >> variable` fails if a user inputs characters instead of numbers. We created helper functions in `Utils::getSafeInt` that check `std::cin.fail()`, clear the error state using `std::cin.clear()`, flush remaining buffer characters with `std::cin.ignore()`, and loop until valid input is received.

**Q4: How does the Leaderboard ranking algorithm work?**  
> *Ans:* In `ResultManager::displayLeaderboard()`, `std::sort` is invoked on the vector of `ResultRecord` structures using a custom C++ lambda function that compares candidate percentages in descending order, with ties broken by raw score.

---
*Created for Chandigarh University MCA Academic Project Submission.*
