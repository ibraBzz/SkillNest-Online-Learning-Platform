#ifndef PROXY_H
#define PROXY_H

#include <iostream>
#include <string>
#include "dbconnection.h"

using namespace std;

class EnrollmentSystem {
public:
    virtual void enrollStudent(string studentEmail, int courseId) = 0;
    virtual ~EnrollmentSystem() {}
};

class RealEnrollment : public EnrollmentSystem {
public:
    void enrollStudent(string studentEmail, int courseId) override {
        DBConnection* db = DBConnection::getInstance();

        if (db->requestEnrollment(studentEmail, courseId)) {

    cout << "Enrollment request submitted for approval!"
         << endl;
}

else {

    cout << "Enrollment already requested or exists!"
         << endl;
}
    }
};

class EnrollmentProxy : public EnrollmentSystem {
private:
    RealEnrollment realEnrollment;

public:
    void enrollStudent(string studentEmail, int courseId) override {

        DBConnection* db = DBConnection::getInstance();

     
        string courseTitle = db->getCourseTitleById(courseId);

        if (courseTitle == "") {
            cout << "Invalid course!" << endl;
            return;
        }

      
        string enrolledHTML = db->getStudentCoursesHTML(studentEmail);

        if (enrolledHTML.find(courseTitle) != string::npos) {
            cout << "Already enrolled in this course!" << endl;
            return;
        }

       
        realEnrollment.enrollStudent(studentEmail, courseId);
    }
};
class QuizSystem {
public:
virtual string takeQuiz(string studentEmail, int quizId) = 0;
    virtual ~QuizSystem() {}
};

class RealQuizAccess : public QuizSystem {
public:
    string takeQuiz(string studentEmail, int quizId) override {

        DBConnection* db = DBConnection::getInstance();
cout << "REAL QUIZ ACCESS GRANTED | Saving to DB..." << endl;
       db->takeQuiz(studentEmail, quizId);

cout << "Quiz started successfully!" << endl;

return "success";
    }
};
class QuizAccessProxy : public QuizSystem {
private:
    RealQuizAccess realQuiz;

public:
    string takeQuiz(string studentEmail, int quizId) override {

        DBConnection* db = DBConnection::getInstance();
        cout << "Quiz Proxy Checking | Student: "
     << studentEmail
     << " | Quiz ID: "
     << quizId
     << endl;
string quizTitle = db->getQuizTitleById(quizId);

if (quizTitle == "") {
    cout << "Access Denied: Invalid or unavailable quiz!" << endl;
    return "failure";
}
       
        if (db->hasStudentTakenQuiz(studentEmail, quizId)) {
    cout << "Access Denied: You already submitted this quiz!" << endl;
    return "already_taken" ;
}

       return realQuiz.takeQuiz(studentEmail, quizId);
    }
};
#endif