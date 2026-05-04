#ifndef TEACHER_H
#define TEACHER_H

#include "user.h"

class Teacher : public User {
public:
    Teacher();
    void showRole() override;
};

#endif