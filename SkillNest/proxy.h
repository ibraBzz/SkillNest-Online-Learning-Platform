#ifndef PROXY_H
#define PROXY_H

#include <iostream>
#include <string>
#include "dbconnection.h"

using namespace std;

/*
================= PROXY DESIGN PATTERN START =================
Purpose:
SystemAccess is the common interface.
Both RealEnrollmentService and EnrollmentProxy follow this interface.

The frontend should not directly enroll a student.
It should call EnrollmentProxy first.
===============================================================
*/

class EnrollmentAccess {
public:
    virtual string enroll(string studentEmail, int courseId) = 0;
    virtual ~EnrollmentAccess() {}
};

/*
Real subject:
This class performs the actual enrollment.
It should only be called after proxy validation passes.
*/
class RealEnrollmentService : public EnrollmentAccess {
public:
    string enroll(string studentEmail, int courseId) override {
        DBConnection* db = DBConnection::getInstance();
        db->enrollStudentInCourse(studentEmail, courseId);

        return "Enrollment successful!";
    }
};

/*
Proxy:
This class controls access before real enrollment.
It checks:
1. Course exists
2. Course is approved
3. Student is not already enrolled
*/
class EnrollmentProxy : public EnrollmentAccess {
private:
    RealEnrollmentService realEnrollment;

public:
    string enroll(string studentEmail, int courseId) override {
        DBConnection* db = DBConnection::getInstance();

        /*
        ================= PROXY DESIGN PATTERN START =================
        Purpose:
        The proxy controls access before real enrollment happens.

        It checks:
        1. Course exists
        2. Course is approved
        3. Student is not already enrolled

        If the course is no longer approved, the proxy also removes
        any old enrollment for that student and course.
        ================================================================
        */

        if (!db->courseExists(courseId)) {
            return "Enrollment failed: Course does not exist.";
        }

        if (!db->isCourseApproved(courseId)) {
            db->removeEnrollment(studentEmail, courseId);
            return "Enrollment failed: Course is not approved. You were removed from this course if previously enrolled.";
        }

        if (db->isAlreadyEnrolled(studentEmail, courseId)) {
            return "Enrollment failed: You are already enrolled in this course.";
        }

        return realEnrollment.enroll(studentEmail, courseId);

        /*
        ================== PROXY DESIGN PATTERN END ==================
        */
    }
};

/*
================== PROXY DESIGN PATTERN END ==================
*/


/*
Old admin proxy kept below.
You may keep this for admin-only access demonstration.
*/

class SystemAccess {
public:
    virtual void publishGrade() = 0;
    virtual void manageUsers() = 0;
    virtual ~SystemAccess() {}
};

class RealSystemAccess : public SystemAccess {
public:
    void publishGrade() override {
        cout << "Grade published successfully!" << endl;
    }

    void manageUsers() override {
        cout << "User management accessed successfully!" << endl;
    }
};

class SystemAccessProxy : public SystemAccess {
private:
    string role;
    RealSystemAccess realAccess;

public:
    SystemAccessProxy(string userRole) {
        role = userRole;
    }

    void publishGrade() override {
        if (role == "admin") {
            realAccess.publishGrade();
        }
        else {
            cout << "Access Denied: Only Admin can publish grades." << endl;
        }
    }

    void manageUsers() override {
        if (role == "admin") {
            realAccess.manageUsers();
        }
        else {
            cout << "Access Denied: Only Admin can manage users." << endl;
        }
    }
};

#endif