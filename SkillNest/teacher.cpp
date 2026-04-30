#include <iostream>
#include "teacher.h"

using namespace std;

Teacher::Teacher() : User() {
}

void Teacher::showRole() {
    cout << "I am a Teacher" << endl;
}