#include <iostream>
#include <memory>
#include "Utils.h"
#include "AuthManager.h"
#include "ResultManager.h"
#include "ExamEngine.h"
#include "Student.h"
#include "Admin.h"

void handleStudentSession(std::shared_ptr<Student> student, ExamEngine& examEngine, ResultManager& resultManager) {
    while (true) {
        Utils::printHeader("STUDENT PORTAL - " + student->getFullName());
        student->displayMenu();
        std::cout << "\n";
        int choice = Utils::getSafeInt(1, 6, "Enter option (1-6): ");

        if (choice == 1) {
            // Full Assessment
            examEngine.startExam(*student, "");
        } else if (choice == 2) {
            // Module-wise Practice Test
            auto categories = examEngine.getAvailableCategories();
            if (categories.empty()) {
                std::cout << Utils::RED << " No question categories available.\n" << Utils::RESET;
                Utils::pause();
                continue;
            }

            Utils::printHeader("SELECT ASSESSMENT MODULE");
            std::cout << "Available Computer Aptitude Modules:\n\n";
            for (size_t i = 0; i < categories.size(); ++i) {
                std::cout << " [" << (i + 1) << "] " << categories[i] << "\n";
            }
            std::cout << " [" << (categories.size() + 1) << "] Return to Student Menu\n\n";

            int catChoice = Utils::getSafeInt(1, categories.size() + 1, "Select module: ");
            if (catChoice <= static_cast<int>(categories.size())) {
                examEngine.startExam(*student, categories[catChoice - 1]);
            }
        } else if (choice == 3) {
            // My Results
            resultManager.displayStudentHistory(student->getUsername());
        } else if (choice == 4) {
            // Leaderboard
            resultManager.displayLeaderboard();
        } else if (choice == 5) {
            // Syllabus
            examEngine.showSyllabus();
        } else if (choice == 6) {
            std::cout << Utils::GREEN << "\nLogging out from student session...\n" << Utils::RESET;
            Utils::pause();
            break;
        }
    }
}

void handleAdminSession(std::shared_ptr<Admin> admin, ExamEngine& examEngine, ResultManager& resultManager) {
    while (true) {
        Utils::printHeader("FACULTY & ADMIN EVALUATION PANEL");
        admin->displayMenu();
        std::cout << "\n";
        int choice = Utils::getSafeInt(1, 8, "Enter option (1-8): ");

        if (choice == 1) {
            examEngine.listAllQuestions();
        } else if (choice == 2) {
            examEngine.addQuestionWizard();
        } else if (choice == 3) {
            examEngine.deleteQuestionWizard();
        } else if (choice == 4) {
            resultManager.displayAllSubmissions();
        } else if (choice == 5) {
            resultManager.displayBatchAnalytics();
        } else if (choice == 6) {
            std::string q = Utils::getSafeString("Enter Student UID, Username, or Name to search: ");
            resultManager.searchStudentReport(q);
        } else if (choice == 7) {
            resultManager.displayLeaderboard();
        } else if (choice == 8) {
            std::cout << Utils::GREEN << "\nLogging out from faculty session...\n" << Utils::RESET;
            Utils::pause();
            break;
        }
    }
}

void showAboutProject() {
    Utils::printHeader("PROJECT SPECIFICATION & DEVELOPER PROFILE");
    std::cout << Utils::BOLD << Utils::CYAN << "PROJECT TITLE:\n" << Utils::RESET;
    std::cout << "   Smart Computer Aptitude Testing & Evaluation System (SCATES)\n\n";

    std::cout << Utils::BOLD << Utils::CYAN << "ACADEMIC CONTEXT:\n" << Utils::RESET;
    std::cout << "   Degree  : Master of Computer Applications (MCA General)\n";
    std::cout << "   Subject : Computer Aptitude / C++ Object-Oriented Software Development\n";
    std::cout << "   Campus  : Chandigarh University (CU)\n\n";

    std::cout << Utils::BOLD << Utils::CYAN << "TECHNICAL ARCHITECTURE & CONCEPTS DEMONSTRATED:\n" << Utils::RESET;
    std::cout << "   * Object-Oriented Programming (OOP):\n";
    std::cout << "     - Abstract Base Class 'User' with pure virtual functions\n";
    std::cout << "     - Derived Classes 'Student' and 'Admin' implementing dynamic polymorphism\n";
    std::cout << "     - Encapsulated domain classes: 'Question', 'ResultRecord'\n";
    std::cout << "   * File Handling & Data Persistence:\n";
    std::cout << "     - Safe serialization and deserialization of questions, credentials, and test results\n";
    std::cout << "   * Data Structures & Algorithms:\n";
    std::cout << "     - Dynamic STL Vectors, Hash Maps for category analytics, Lambda sorting for leaderboards\n";
    std::cout << "   * Modern C++ Standards:\n";
    std::cout << "     - std::shared_ptr smart pointers, streams, exception-safe input loops, ANSI UI styling\n\n";

    std::cout << Utils::BOLD << Utils::CYAN << "DEFAULT CREDENTIALS FOR EVALUATION / VIVA:\n" << Utils::RESET;
    std::cout << "   - Faculty / Admin Login: Username: " << Utils::YELLOW << "admin" << Utils::RESET 
              << " | Password: " << Utils::YELLOW << "admin123" << Utils::RESET << "\n";
    std::cout << "   - Student Demo Login   : Username: " << Utils::YELLOW << "student" << Utils::RESET 
              << " | Password: " << Utils::YELLOW << "pass123" << Utils::RESET 
              << " (UID: 24MCA10250)\n";
    std::cout << "   - Alternative Student  : Username: " << Utils::YELLOW << "priya" << Utils::RESET 
              << " | Password: " << Utils::YELLOW << "pass123" << Utils::RESET 
              << " (UID: 24MCA10284)\n";

    Utils::printDivider('=', 74);
    Utils::pause();
}

int main() {
    Utils::enableVirtualTerminal();

    ResultManager resultManager("data/results.txt");
    ExamEngine examEngine(resultManager, "data/questions.txt");
    AuthManager authManager("data/users.txt");

    while (true) {
        Utils::printHeader("WELCOME TO MCA COMPUTER APTITUDE ASSESSMENT PLATFORM");
        std::cout << Utils::BOLD << "Chandigarh University - Department of Computer Applications\n" << Utils::RESET;
        std::cout << "Subject: Computer Aptitude | C++ Semester Project\n\n";

        std::cout << " [1] Student Login (Take Assessment / View Scorecards)\n";
        std::cout << " [2] New Student Registration\n";
        std::cout << " [3] Faculty / Admin Login (Exam & Question Management)\n";
        std::cout << " [4] View Assessment Syllabus & Scoring Scheme\n";
        std::cout << " [5] Project Architecture & Viva Guidelines\n";
        std::cout << " [6] Exit System\n\n";

        int choice = Utils::getSafeInt(1, 6, "Enter option (1-6): ");

        if (choice == 1) {
            // Student Login
            Utils::printHeader("STUDENT AUTHENTICATION");
            std::string u = Utils::getSafeString("Enter Username: ");
            std::string p = Utils::getSafePassword("Enter Password: ");

            auto user = authManager.authenticate(u, p);
            if (user && user->getRole() == "STUDENT") {
                auto student = std::dynamic_pointer_cast<Student>(user);
                std::cout << Utils::GREEN << "\nAuthentication Successful! Welcome, " << student->getFullName() << ".\n" << Utils::RESET;
                Utils::pause();
                handleStudentSession(student, examEngine, resultManager);
            } else {
                std::cout << Utils::RED << "\n [ERROR] Invalid student credentials or unauthorized role!\n" << Utils::RESET;
                Utils::pause();
            }
        } else if (choice == 2) {
            // Student Registration
            Utils::printHeader("NEW CANDIDATE REGISTRATION");
            std::string u = Utils::getSafeString("Choose Username: ");
            if (authManager.usernameExists(u)) {
                std::cout << Utils::RED << " [ERROR] Username already taken! Please choose another.\n" << Utils::RESET;
                Utils::pause();
                continue;
            }
            std::string p = Utils::getSafePassword("Choose Password: ");
            std::string name = Utils::getSafeString("Enter Full Name: ");
            std::string uid = Utils::getSafeString("Enter CU Student UID (e.g. 24MCA10555): ");

            if (authManager.registerStudent(u, p, name, uid)) {
                std::cout << Utils::GREEN << "\n [SUCCESS] Account registered successfully! You can now log in.\n" << Utils::RESET;
            } else {
                std::cout << Utils::RED << "\n [ERROR] Registration failed. Please try again.\n" << Utils::RESET;
            }
            Utils::pause();
        } else if (choice == 3) {
            // Admin Login
            Utils::printHeader("FACULTY & ADMINISTRATOR AUTHENTICATION");
            std::string u = Utils::getSafeString("Enter Faculty Username: ");
            std::string p = Utils::getSafePassword("Enter Password: ");

            auto user = authManager.authenticate(u, p);
            if (user && user->getRole() == "ADMIN") {
                auto admin = std::dynamic_pointer_cast<Admin>(user);
                std::cout << Utils::GREEN << "\nAdmin Authentication Successful! Welcome, " << admin->getFullName() << ".\n" << Utils::RESET;
                Utils::pause();
                handleAdminSession(admin, examEngine, resultManager);
            } else {
                std::cout << Utils::RED << "\n [ERROR] Invalid admin credentials!\n" << Utils::RESET;
                Utils::pause();
            }
        } else if (choice == 4) {
            examEngine.showSyllabus();
        } else if (choice == 5) {
            showAboutProject();
        } else if (choice == 6) {
            Utils::printHeader("THANK YOU");
            std::cout << Utils::BOLD << Utils::GREEN << "Thank you for using the MCA Computer Aptitude Assessment System!\n" << Utils::RESET;
            std::cout << "All session records are securely synchronized to data files.\n\n";
            break;
        }
    }

    return 0;
}
