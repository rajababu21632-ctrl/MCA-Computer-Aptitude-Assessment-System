#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"

class Student : public User {
private:
    std::string uid; // MCA Student UID

public:
    Student();
    Student(const std::string& u, const std::string& p, const std::string& name, const std::string& studentUid);

    std::string getUid() const { return uid; }
    void setUid(const std::string& newUid) { uid = newUid; }

    std::string serialize() const override;
    void displayMenu() const override;
};

#endif // STUDENT_H
