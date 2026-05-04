#pragma once
#include <string>
#include <iostream>
using namespace std;

class Course {
public:
    virtual string getDescription() = 0;
    virtual string getFeatures() = 0;
    virtual ~Course() {}
};

class BasicCourse : public Course {
private:
    string title;

public:
    BasicCourse(string t) : title(t) {}

    string getDescription() override {
        return title;
    }

    string getFeatures() override {
        return "<label><input type='checkbox' name='basic' checked disabled> Basic Course Access</label><br>";
    }
};

class CourseDecorator : public Course {
protected:
    Course* wrappedCourse;

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
private:
    bool selected;

public:
    CertificateDecorator(Course* c, bool isSelected = false)
        : CourseDecorator(c), selected(isSelected) {}

    string getDescription() override {
        return wrappedCourse->getDescription() + " + Certificate";
    }

    string getFeatures() override {
        return wrappedCourse->getFeatures()
            + "<label><input type='checkbox' name='certificate' "
            + string(selected ? "checked" : "")
            + "> Completion Certificate</label><br>";
    }
};

class NotesDecorator : public CourseDecorator {
private:
    bool selected;

public:
    NotesDecorator(Course* c, bool isSelected = false)
        : CourseDecorator(c), selected(isSelected) {}

    string getDescription() override {
        return wrappedCourse->getDescription() + " + Notes";
    }

    string getFeatures() override {
        return wrappedCourse->getFeatures()
            + "<label><input type='checkbox' name='notes' "
            + string(selected ? "checked" : "")
            + "> Downloadable PDF Notes</label><br>";
    }
};

class VideoBundleDecorator : public CourseDecorator {
private:
    bool selected;

public:
    VideoBundleDecorator(Course* c, bool isSelected = false)
        : CourseDecorator(c), selected(isSelected) {}

    string getDescription() override {
        return wrappedCourse->getDescription() + " + Video Bundle";
    }

    string getFeatures() override {
        return wrappedCourse->getFeatures()
            + "<label><input type='checkbox' name='videos' "
            + string(selected ? "checked" : "")
            + "> Extra Video Bundle</label><br>";
    }
};