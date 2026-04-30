#include "user.h"

User::User() {
    name = "";
    email = "";
    password = "";
}

User::User(string n, string e, string p) {
    name = n;
    email = e;
    password = p;
}

void User::setName(string n) {
    name = n;
}

void User::setEmail(string e) {
    email = e;
}

void User::setPassword(string p) {
    password = p;
}

string User::getName() {
    return name;
}

string User::getEmail() {
    return email;
}

string User::getPassword() {
    return password;
}

User::~User() {
}