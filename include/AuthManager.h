#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H

#include <string>
#include <vector>
#include <memory>
#include "User.h"
#include "Student.h"
#include "Admin.h"

class AuthManager {
private:
    std::string usersFilePath;
    std::vector<std::shared_ptr<User>> users;

    void loadUsers();
    void saveUsers();
    void initializeDefaultUsers();

public:
    explicit AuthManager(const std::string& path = "data/users.txt");

    std::shared_ptr<User> authenticate(const std::string& username, const std::string& password);
    bool registerStudent(const std::string& username, const std::string& password,
                         const std::string& fullName, const std::string& uid);
    bool usernameExists(const std::string& username) const;
};

#endif // AUTH_MANAGER_H
