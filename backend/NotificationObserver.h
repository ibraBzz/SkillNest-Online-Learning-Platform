#ifndef NOTIFICATION_OBSERVER_H
#define NOTIFICATION_OBSERVER_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

/*
====================================================
DESIGN PATTERN 3: OBSERVER PATTERN START
What it does:
The Observer Pattern notifies many users when one
important event happens.

Here, when a teacher posts an update, all enrolled
students receive notifications.
====================================================
*/

class Observer {
public:
    virtual void update(string message) = 0;
};

class StudentObserver : public Observer {
private:
    string studentName;

public:
    StudentObserver(string name) {
        studentName = name;
    }

    void update(string message) override {
        cout << "Notification for " << studentName << ": " << message << endl;
    }
};

class CourseNotification {
private:
    vector<Observer*> students;

public:
    void addStudent(Observer* student) {
        students.push_back(student);
    }

    void notifyStudents(string message) {
        for (int i = 0; i < students.size(); i++) {
            students[i]->update(message);
        }
    }
};

/*
====================================================
DESIGN PATTERN 3: OBSERVER PATTERN END
====================================================
*/

#endif
