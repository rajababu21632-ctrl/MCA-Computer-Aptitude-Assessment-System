#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"

class Admin : public User {
private:
    std::string employeeId;

public:
    Admin();
    Admin(const std::string& u, const std::string& p, const std::string& name, const std::string& empId);

    std::string getEmployeeId() const { return employeeId; }
    void setEmployeeId(const std::string& empId) { employeeId = empId; }

    std::string serialize() const override;
    void displayMenu() const override;
};

#endif // ADMIN_H
