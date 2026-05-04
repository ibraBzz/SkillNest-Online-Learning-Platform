#pragma once
#include "observer.h"
#include "dbconnection.h"
#include <iostream>
#include <string>
using namespace std;

class StudentObserver : public Observer {

    string email;

public:
    StudentObserver(string e) {
        email = e;
    }
void update(string message, string courseTitle) override {

    string fullMsg = courseTitle + ": " + message;

    cout << "[NOTIFICATION] " << email
         << " | " << fullMsg << endl;
    DBConnection* db = DBConnection::getInstance();
    db->addNotification(email, fullMsg);
}
};