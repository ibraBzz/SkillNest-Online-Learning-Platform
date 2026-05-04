#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
using namespace std;

class User {
protected:
    string name;
    string email;
    string role;

public:
    User(string n, string e, string r) {
        name = n;
        email = e;
        role = r;
    }

    virtual void showDashboard() = 0;

    string getRole() {
        return role;
    }

    string getEmail() {
        return email;
    }

    virtual ~User() {}
};

class Student : public User {
public:
    Student(string n, string e) : User(n, e, "student") {}

    void showDashboard() override {
        cout << "Student Dashboard: Browse courses, enroll, take quizzes, view progress.\n";
    }
};

class Teacher : public User {
public:
    Teacher(string n, string e) : User(n, e, "teacher") {}

    void showDashboard() override {
        cout << "Teacher Dashboard: Create courses, upload lessons, create quizzes.\n";
    }
};

class Admin : public User {
public:
    Admin(string n, string e) : User(n, e, "admin") {}

    void showDashboard() override {
        cout << "Admin Dashboard: Manage users, approve courses, publish grades.\n";
    }
};

#endif