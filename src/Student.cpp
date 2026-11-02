#include "Student.h"
#include "Utils.h"
#include <iostream>

Student::Student() : User("", "", "", "STUDENT"), uid("") {}

Student::Student(const std::string& u, const std::string& p, const std::string& name, const std::string& studentUid)
    : User(u, p, name, "STUDENT"), uid(studentUid) {}

std::string Student::serialize() const {
    // Format: username|password|fullName|role|uid
    return username + "|" + password + "|" + fullName + "|" + role + "|" + uid;
}

void Student::displayMenu() const {
    std::cout << Utils::BOLD << Utils::GREEN << "=== STUDENT DASHBOARD ===" << Utils::RESET << "\n";
    std::cout << "Student: " << Utils::BOLD << fullName << Utils::RESET << " | UID: " << Utils::CYAN << uid << Utils::RESET << "\n\n";
    std::cout << " [1] Take Full Computer Aptitude Assessment Test (All Modules)\n";
    std::cout << " [2] Take Module-Wise Practice Test (Topic Specific)\n";
    std::cout << " [3] View My Previous Exam Results & Scorecard\n";
    std::cout << " [4] View Global Merit List / Leaderboard\n";
    std::cout << " [5] View Assessment Syllabus & Scoring Criteria\n";
    std::cout << " [6] Logout\n";
}
