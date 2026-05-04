<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b
#pragma once
#include <string>
#include <iostream>
using namespace std;

<<<<<<< HEAD
class Course {
public:
    virtual string getDescription() = 0;
    virtual string getFeatures() = 0;
    virtual ~Course() {}
};

=======
// ─────────────────────────────────────────────
// STEP 1: Abstract Component
// Every course (basic or decorated) must implement
// getDescription() and getFeatures().
// ─────────────────────────────────────────────
class Course {
public:
    virtual string getDescription() = 0;
    virtual string getFeatures()    = 0;  // returns HTML-friendly feature list
=======
#ifndef DECORATOR_H
#define DECORATOR_H

#include <iostream>
#include <string>

using namespace std;

class Course {
public:
    virtual string getDescription() = 0;
>>>>>>> 451e74d704a00d31deef478e5ed597f4517dcb45
    virtual ~Course() {}
};


<<<<<<< HEAD
// ─────────────────────────────────────────────
// STEP 2: Concrete Component
// The plain course with no extras.
// ─────────────────────────────────────────────
=======
>>>>>>> 451e74d704a00d31deef478e5ed597f4517dcb45
>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b
class BasicCourse : public Course {
private:
    string title;

public:
<<<<<<< HEAD
    BasicCourse(string t) : title(t) {}
=======
<<<<<<< HEAD
    BasicCourse(string t) : title(t) {}
=======
    BasicCourse(string t) {
        title = t;
    }
>>>>>>> 451e74d704a00d31deef478e5ed597f4517dcb45
>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b

    string getDescription() override {
        return title;
    }
<<<<<<< HEAD

    string getFeatures() override {
        return "<label><input type='checkbox' name='basic' checked disabled> Basic Course Access</label><br>";
    }
};

class CourseDecorator : public Course {
protected:
    Course* wrappedCourse;
=======
<<<<<<< HEAD

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
>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b

public:
    CourseDecorator(Course* c) : wrappedCourse(c) {}

    string getDescription() override {
        return wrappedCourse->getDescription();
    }

    string getFeatures() override {
        return wrappedCourse->getFeatures();
    }

    virtual ~CourseDecorator() {
<<<<<<< HEAD
        delete wrappedCourse;
=======
        delete wrappedCourse; 
>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b
    }
};

class CertificateDecorator : public CourseDecorator {
<<<<<<< HEAD
private:
    bool selected;

public:
    CertificateDecorator(Course* c, bool isSelected = false)
        : CourseDecorator(c), selected(isSelected) {}
=======
public:
    CertificateDecorator(Course* c) : CourseDecorator(c) {}
>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b

    string getDescription() override {
        return wrappedCourse->getDescription() + " + Certificate";
    }

    string getFeatures() override {
        return wrappedCourse->getFeatures()
<<<<<<< HEAD
            + "<label><input type='checkbox' name='certificate' "
            + string(selected ? "checked" : "")
            + "> Completion Certificate</label><br>";
=======
             + "<li><label><input type='checkbox'> Completion Certificate</label></li>";
>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b
    }
};

class NotesDecorator : public CourseDecorator {
<<<<<<< HEAD
private:
    bool selected;

public:
    NotesDecorator(Course* c, bool isSelected = false)
        : CourseDecorator(c), selected(isSelected) {}
=======
public:
    NotesDecorator(Course* c) : CourseDecorator(c) {}
>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b

    string getDescription() override {
        return wrappedCourse->getDescription() + " + Notes";
    }

    string getFeatures() override {
        return wrappedCourse->getFeatures()
<<<<<<< HEAD
            + "<label><input type='checkbox' name='notes' "
            + string(selected ? "checked" : "")
            + "> Downloadable PDF Notes</label><br>";
=======
             + "<li><label><input type='checkbox'> Downloadable PDF Notes</label></li>";
>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b
    }
};

class VideoBundleDecorator : public CourseDecorator {
<<<<<<< HEAD
private:
    bool selected;

public:
    VideoBundleDecorator(Course* c, bool isSelected = false)
        : CourseDecorator(c), selected(isSelected) {}
=======
public:
    VideoBundleDecorator(Course* c) : CourseDecorator(c) {}
>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b

    string getDescription() override {
        return wrappedCourse->getDescription() + " + Video Bundle";
    }

    string getFeatures() override {
        return wrappedCourse->getFeatures()
<<<<<<< HEAD
            + "<label><input type='checkbox' name='videos' "
            + string(selected ? "checked" : "")
            + "> Extra Video Bundle</label><br>";
    }
};
=======
             + "<li><label><input type='checkbox'> Extra Video Bundle</label></li>";
    }
};
=======
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
>>>>>>> 451e74d704a00d31deef478e5ed597f4517dcb45
>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b
