#ifndef DATABASE_SINGLETON_H
#define DATABASE_SINGLETON_H

#include <iostream>
#include <string>
using namespace std;

/*
====================================================
DESIGN PATTERN 2: SINGLETON PATTERN START
What it does:
The Singleton Pattern ensures that only one object
of a class is created.

Here, DatabaseSingleton represents one shared database
connection/system object for SkillNest.
====================================================
*/

class DatabaseSingleton {
private:
    static DatabaseSingleton* instance;

    DatabaseSingleton() {
        cout << "Database connection created successfully.\n";
    }

public:
    static DatabaseSingleton* getInstance() {
        if (instance == nullptr) {
            instance = new DatabaseSingleton();
        }
        return instance;
    }

    void connect() {
        cout << "Connected to SkillNest database.\n";
    }

    void executeQuery(string query) {
        cout << "Executing SQL Query: " << query << endl;
    }
};

DatabaseSingleton* DatabaseSingleton::instance = nullptr;

/*
====================================================
DESIGN PATTERN 2: SINGLETON PATTERN END
====================================================
*/

#endif
