#include "User.h"
#include <sstream>

User::User() : role("STUDENT") {}

User::User(const std::string& u, const std::string& p, const std::string& name, const std::string& r)
    : username(u), password(p), fullName(name), role(r) {}

std::string User::serialize() const {
    // Format: username|password|fullName|role
    return username + "|" + password + "|" + fullName + "|" + role;
}
