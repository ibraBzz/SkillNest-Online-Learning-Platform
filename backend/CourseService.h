#ifndef COURSE_SERVICE_H
#define COURSE_SERVICE_H

#include <iostream>
#include <string>
using namespace std;

/*
====================================================
DESIGN PATTERN 2: SINGLETON PATTERN START
What it does:
The Singleton Pattern makes sure only ONE object of
a class exists in the whole program.

Here, CourseService is used as the single central
service for course-related actions.
====================================================
*/

class CourseService {
private:
    static CourseService* instance;

    CourseService() {
        cout << "CourseService started only once.\n";
    }

public:
    static CourseService* getInstance() {
        if (instance == nullptr) {
            instance = new CourseService();
        }
        return instance;
    }

    void createCourse(string title) {
        cout << "Course created: " << title << endl;
    }

    void approveCourse(string title) {
        cout << "Course approved: " << title << endl;
    }

    void deleteCourse(string title) {
        cout << "Course deleted: " << title << endl;
    }
};

CourseService* CourseService::instance = nullptr;

/*
====================================================
DESIGN PATTERN 2: SINGLETON PATTERN END
====================================================
*/

#endif