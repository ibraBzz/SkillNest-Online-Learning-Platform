#include <iostream>
#include "student.h"

using namespace std;

Student::Student() : User() {
}

void Student::showRole() {
    cout << "I am a Student" << endl;
}