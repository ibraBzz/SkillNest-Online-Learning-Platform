#include "userfactory.h"
#include "student.h"
#include "teacher.h"
#include "admin.h"

User* UserFactory::createUser(string role) {

    if (role == "student") {
        return new Student();
    }

    else if (role == "teacher") {
        return new Teacher();
    }

    else if (role == "admin") {
        return new Admin();
    }

    return nullptr;
}