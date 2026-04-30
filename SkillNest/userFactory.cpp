#include "userfactory.h"
#include "student.h"
#include "teacher.h"
#include "admin.h"

User* UserFactory::createUser(string role) {

    /*
    ================= FACTORY DESIGN PATTERN START =================
    Purpose:
    Creates a user object based only on role.
    Used during login when the role already exists in the database.
    ================================================================
    */

    if (role == "student") {
        return new Student();
    }

    else if (role == "teacher") {
        return new Teacher();
    }

    else if (role == "admin") {
        return new Admin();
    }

    return nullptr;

    /*
    ================== FACTORY DESIGN PATTERN END ==================
    */
}

User* UserFactory::createUser(string role, string name, string email, string password) {

    /*
    ================= FACTORY DESIGN PATTERN START =================
    Purpose:
    Creates a full user profile object during registration.
    The factory decides whether the new profile is a Student, Teacher, or Admin.
    ================================================================
    */

    User* user = createUser(role);

    if (user != nullptr) {
        user->setName(name);
        user->setEmail(email);
        user->setPassword(password);
    }

    return user;

    /*
    ================== FACTORY DESIGN PATTERN END ==================
    */
}