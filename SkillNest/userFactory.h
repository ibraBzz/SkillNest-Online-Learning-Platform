#ifndef USERFACTORY_H
#define USERFACTORY_H

#include <string>
#include "user.h"

using namespace std;

class UserFactory {
public:
    /*
    ================= FACTORY DESIGN PATTERN START =================
    Purpose:
    This method creates the correct User object based on the selected role.
    Instead of server.cpp directly creating Student/Teacher/Admin objects,
    it asks the factory to create the correct object.
    ================================================================
    */
    static User* createUser(string role);

    static User* createUser(string role, string name, string email, string password);
    /*
    ================== FACTORY DESIGN PATTERN END ==================
    */
};

#endif