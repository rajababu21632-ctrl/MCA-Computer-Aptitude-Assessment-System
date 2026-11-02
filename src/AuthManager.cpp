#include "AuthManager.h"
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <iostream>

AuthManager::AuthManager(const std::string& path) : usersFilePath(path) {
    loadUsers();
    if (users.empty()) {
        initializeDefaultUsers();
    }
}

void AuthManager::initializeDefaultUsers() {
    users.push_back(std::make_shared<Admin>("admin", "admin123", "Prof. R. K. Sharma", "CU-MCA-FAC01"));
    users.push_back(std::make_shared<Student>("student", "pass123", "Rahul Sharma", "24MCA10250"));
    users.push_back(std::make_shared<Student>("priya", "pass123", "Priya Verma", "24MCA10284"));
    saveUsers();
}

void AuthManager::loadUsers() {
    users.clear();
    std::ifstream inFile(usersFilePath);
    if (!inFile.is_open()) return;

    std::string line;
    while (std::getline(inFile, line)) {
        line = Utils::trim(line);
        if (line.empty()) continue;

        std::vector<std::string> tokens;
        std::string token;
        std::istringstream ss(line);
        while (std::getline(ss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 5) {
            std::string u    = tokens[0];
            std::string p    = tokens[1];
            std::string name = tokens[2];
            std::string role = tokens[3];
            std::string id   = tokens[4];

            if (role == "ADMIN") {
                users.push_back(std::make_shared<Admin>(u, p, name, id));
            } else {
                users.push_back(std::make_shared<Student>(u, p, name, id));
            }
        }
    }
    inFile.close();
}

void AuthManager::saveUsers() {
    std::ofstream outFile(usersFilePath, std::ios::trunc);
    if (!outFile.is_open()) return;

    for (const auto& u : users) {
        outFile << u->serialize() << "\n";
    }
    outFile.close();
}

std::shared_ptr<User> AuthManager::authenticate(const std::string& username, const std::string& password) {
    loadUsers();
    for (const auto& u : users) {
        if (u->getUsername() == username && u->verifyPassword(password)) {
            return u;
        }
    }
    return nullptr;
}

bool AuthManager::usernameExists(const std::string& username) const {
    for (const auto& u : users) {
        if (u->getUsername() == username) {
            return true;
        }
    }
    return false;
}

bool AuthManager::registerStudent(const std::string& username, const std::string& password,
                                 const std::string& fullName, const std::string& uid) {
    loadUsers();
    if (usernameExists(username)) {
        return false;
    }
    users.push_back(std::make_shared<Student>(username, password, fullName, uid));
    saveUsers();
    return true;
}
