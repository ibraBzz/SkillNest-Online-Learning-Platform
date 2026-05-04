#pragma once
#include <string>
#include <iostream>
using namespace std;

// ─────────────────────────────────────────────
// STEP 1: Abstract Component
// Every course (basic or decorated) must implement
// getDescription() and getFeatures().
// ─────────────────────────────────────────────
class Course {
public:
    virtual string getDescription() = 0;
    virtual string getFeatures()    = 0;  // returns HTML-friendly feature list
    virtual ~Course() {}
};


// ─────────────────────────────────────────────
// STEP 2: Concrete Component
// The plain course with no extras.
// ─────────────────────────────────────────────
class BasicCourse : public Course {
private:
    string title;

public:
    BasicCourse(string t) : title(t) {}

    string getDescription() override {
        return title;
    }

    string getFeatures() override {
        return "<li><label><input type='checkbox'> Basic Course Access</label></li>";
    }
};


// ─────────────────────────────────────────────
// STEP 3: Abstract Decorator
// Wraps any Course* and delegates to it.
// All concrete decorators inherit from this.
// ─────────────────────────────────────────────
class CourseDecorator : public Course {
protected:
    Course* wrappedCourse;   // the course being decorated

public:
    CourseDecorator(Course* c) : wrappedCourse(c) {}

    string getDescription() override {
        return wrappedCourse->getDescription();
    }

    string getFeatures() override {
        return wrappedCourse->getFeatures();
    }

    virtual ~CourseDecorator() {
        delete wrappedCourse; 
    }
};

class CertificateDecorator : public CourseDecorator {
public:
    CertificateDecorator(Course* c) : CourseDecorator(c) {}

    string getDescription() override {
        return wrappedCourse->getDescription() + " + Certificate";
    }

    string getFeatures() override {
        return wrappedCourse->getFeatures()
             + "<li><label><input type='checkbox'> Completion Certificate</label></li>";
    }
};

class NotesDecorator : public CourseDecorator {
public:
    NotesDecorator(Course* c) : CourseDecorator(c) {}

    string getDescription() override {
        return wrappedCourse->getDescription() + " + Notes";
    }

    string getFeatures() override {
        return wrappedCourse->getFeatures()
             + "<li><label><input type='checkbox'> Downloadable PDF Notes</label></li>";
    }
};

class VideoBundleDecorator : public CourseDecorator {
public:
    VideoBundleDecorator(Course* c) : CourseDecorator(c) {}

    string getDescription() override {
        return wrappedCourse->getDescription() + " + Video Bundle";
    }

    string getFeatures() override {
        return wrappedCourse->getFeatures()
             + "<li><label><input type='checkbox'> Extra Video Bundle</label></li>";
    }
};