#ifndef USER_FACTORY_H
#define USER_FACTORY_H

#include "User.h"

/*
====================================================
DESIGN PATTERN 1: FACTORY PATTERN START
What it does:
The Factory Pattern creates objects without exposing
the creation logic directly in main.cpp.

Here, UserFactory creates Student, Teacher, or Admin
objects depending on the selected role.
====================================================
*/

class UserFactory {
public:
    static User* createUser(string role, string name, string email) {
        if (role == "student") {
            return new Student(name, email);
        }
        else if (role == "teacher") {
            return new Teacher(name, email);
        }
        else if (role == "admin") {
            return new Admin(name, email);
        }
        else {
            return nullptr;
        }
    }
};

/*
====================================================
DESIGN PATTERN 1: FACTORY PATTERN END
====================================================
*/

#endif