#ifndef AUTH_PROXY_H
#define AUTH_PROXY_H

#include "User.h"

/*
====================================================
DESIGN PATTERN 3: PROXY PATTERN START
What it does:
The Proxy Pattern controls access to another object.

Here, AuthProxy checks the user's role before allowing
sensitive actions like approving or deleting courses.
====================================================
*/

class AuthProxy {
private:
    User* currentUser;

public:
    AuthProxy(User* user) {
        currentUser = user;
    }

    bool canApproveCourse() {
        return currentUser->getRole() == "admin";
    }

    bool canCreateCourse() {
        return currentUser->getRole() == "teacher";
    }

    bool canEnrollCourse() {
        return currentUser->getRole() == "student";
    }

    void checkAccess(string action) {
        if (action == "approve_course") {
            if (canApproveCourse()) {
                cout << "Access granted: Admin can approve courses.\n";
            } else {
                cout << "Access denied: Only admin can approve courses.\n";
            }
        }
        else if (action == "create_course") {
            if (canCreateCourse()) {
                cout << "Access granted: Teacher can create courses.\n";
            } else {
                cout << "Access denied: Only teacher can create courses.\n";
            }
        }
        else if (action == "enroll_course") {
            if (canEnrollCourse()) {
                cout << "Access granted: Student can enroll in courses.\n";
            } else {
                cout << "Access denied: Only student can enroll in courses.\n";
            }
        }
        else {
            cout << "Unknown action.\n";
        }
    }
};

/*
====================================================
DESIGN PATTERN 3: PROXY PATTERN END
====================================================
*/

#endif