#ifndef STUDENT_H
#define STUDENT_H

#include "user.h"

class Student : public User {
public:
    Student();
    void showRole() override;
};

#endif