#include <iostream>
#include "admin.h"

using namespace std;

Admin::Admin() : User() {
}

void Admin::showRole() {
    cout << "I am an Admin" << endl;
}