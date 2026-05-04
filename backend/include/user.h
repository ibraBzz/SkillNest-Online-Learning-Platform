#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
using namespace std;

class User {
protected:
    string name;
    string email;
    string password;

public:
    User();
    User(string n, string e, string p);

    virtual void showRole() = 0;

    void setName(string n);
    void setEmail(string e);
    void setPassword(string p);

    string getName();
    string getEmail();
    string getPassword();

    virtual ~User();
};

#endif