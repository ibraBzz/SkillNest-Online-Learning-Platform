#ifndef DECORATOR_H
#define DECORATOR_H

#include <iostream>
#include <string>

using namespace std;

class Course {
public:
    virtual string getDescription() = 0;
    virtual ~Course() {}
};


class BasicCourse : public Course {
private:
    string title;

public:
    BasicCourse(string t) {
        title = t;
    }

    string getDescription() override {
        return title;
    }
};


class CourseDecorator : public Course {
protected:
    Course* decoratedCourse;

public:
    CourseDecorator(Course* course) {
        decoratedCourse = course;
    }
};


class CertificateDecorator : public CourseDecorator {
public:
    CertificateDecorator(Course* course) : CourseDecorator(course) {}

    string getDescription() override {
        return decoratedCourse->getDescription() + " + Certificate";
    }
};


class NotesDecorator : public CourseDecorator {
public:
    NotesDecorator(Course* course) : CourseDecorator(course) {}

    string getDescription() override {
        return decoratedCourse->getDescription() + " + Extra Notes";
    }
};
class VideoBundleDecorator : public CourseDecorator {
public:
    VideoBundleDecorator(Course* course) : CourseDecorator(course) {}

    string getDescription() override {
        return decoratedCourse->getDescription() + " + Video Bundle";
    }
};
#endif