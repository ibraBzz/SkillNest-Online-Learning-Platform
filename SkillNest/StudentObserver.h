#pragma once

#include "observer.h"
#include "dbconnection.h"
#include <iostream>
#include <string>

using namespace std;

class StudentObserver : public Observer {
private:
    string email;

public:
    StudentObserver(string e) {
        email = e;
    }

    /*
    ================= OBSERVER DESIGN PATTERN START =================
    Purpose:
    This observer represents one student.
    When a teacher posts an announcement, update() stores that
    announcement as a notification for this student in the database.
    ================================================================
    */
    void update(string message, string courseTitle) override {
        DBConnection* db = DBConnection::getInstance();

        string fullMessage = courseTitle + ": " + message;

        db->addNotification(email, fullMessage);

        cout << "Notification sent to: " << email << endl;
    }
    /*
    ================== OBSERVER DESIGN PATTERN END ==================
    */
};