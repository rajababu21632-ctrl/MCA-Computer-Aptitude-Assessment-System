#include "Admin.h"
#include "Utils.h"
#include <iostream>

Admin::Admin() : User("", "", "", "ADMIN"), employeeId("FAC-MCA-01") {}

Admin::Admin(const std::string& u, const std::string& p, const std::string& name, const std::string& empId)
    : User(u, p, name, "ADMIN"), employeeId(empId) {}

std::string Admin::serialize() const {
    // Format: username|password|fullName|role|employeeId
    return username + "|" + password + "|" + fullName + "|" + role + "|" + employeeId;
}

void Admin::displayMenu() const {
    std::cout << Utils::BOLD << Utils::MAGENTA << "=== FACULTY / ADMIN DASHBOARD ===" << Utils::RESET << "\n";
    std::cout << "Faculty: " << Utils::BOLD << fullName << Utils::RESET << " | ID: " << Utils::CYAN << employeeId << Utils::RESET << "\n\n";
    std::cout << " [1] View Question Bank (All Questions & Answers)\n";
    std::cout << " [2] Add New Question to Bank\n";
    std::cout << " [3] Delete Question from Bank\n";
    std::cout << " [4] View All Students' Test Submissions\n";
    std::cout << " [5] View Performance Analytics (Mean, Highest, Lowest, Pass %)\n";
    std::cout << " [6] Search Individual Student Record\n";
    std::cout << " [7] View Overall Merit Leaderboard\n";
    std::cout << " [8] Logout\n";
}
