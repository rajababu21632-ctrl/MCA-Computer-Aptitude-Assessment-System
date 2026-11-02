#ifndef USER_H
#define USER_H

#include <string>

class User {
protected:
    std::string username;
    std::string password;
    std::string fullName;
    std::string role; // "STUDENT" or "ADMIN"

public:
    User();
    User(const std::string& u, const std::string& p, const std::string& name, const std::string& r);
    virtual ~User() = default;

    // Getters
    std::string getUsername() const { return username; }
    std::string getFullName() const { return fullName; }
    std::string getRole() const { return role; }

    // Authentication check
    bool verifyPassword(const std::string& p) const { return password == p; }

    // Serialization
    virtual std::string serialize() const;
    
    // Pure virtual method for polymorphism
    virtual void displayMenu() const = 0;
};

#endif // USER_H
