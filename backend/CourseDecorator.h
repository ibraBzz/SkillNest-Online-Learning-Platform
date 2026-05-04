#ifndef COURSE_DECORATOR_H
#define COURSE_DECORATOR_H

#include <iostream>
#include <string>
using namespace std;

/*
====================================================
DESIGN PATTERN 5: DECORATOR PATTERN START
What it does:
The Decorator Pattern adds extra features to an
object without changing the original class.

Here, extra features are added to a course, such as
certificate, premium content, and progress badge.
====================================================
*/

class Course {
public:
    virtual string getDescription() = 0;
    virtual int getPrice() = 0;
};

class BasicCourse : public Course {
public:
    string getDescription() override {
        return "Basic Course";
    }

    int getPrice() override {
        return 0;
    }
};

class CourseDecorator : public Course {
protected:
    Course* course;

public:
    CourseDecorator(Course* c) {
        course = c;
    }
};

class CertificateDecorator : public CourseDecorator {
public:
    CertificateDecorator(Course* c) : CourseDecorator(c) {}

    string getDescription() override {
        return course->getDescription() + " + Certificate";
    }

    int getPrice() override {
        return course->getPrice() + 500;
    }
};

class PremiumContentDecorator : public CourseDecorator {
public:
    PremiumContentDecorator(Course* c) : CourseDecorator(c) {}

    string getDescription() override {
        return course->getDescription() + " + Premium Content";
    }

    int getPrice() override {
        return course->getPrice() + 1000;
    }
};

class ProgressBadgeDecorator : public CourseDecorator {
public:
    ProgressBadgeDecorator(Course* c) : CourseDecorator(c) {}

    string getDescription() override {
        return course->getDescription() + " + Progress Badge";
    }

    int getPrice() override {
        return course->getPrice() + 200;
    }
};

/*
====================================================
DESIGN PATTERN 5: DECORATOR PATTERN END
====================================================
*/

#endif
