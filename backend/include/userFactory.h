#ifndef USERFACTORY_H
#define USERFACTORY_H

#include <string>
#include "user.h"

using namespace std;

class UserFactory {
public:
    static User* createUser(string role);
};

#endif