#define CROW_MAIN
#include "include/crow.h"
#include "dbconnection.h"
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
#include "observer.h"
#include "StudentObserver.h"
#include "AnnouncementSystem.h"
#include "userFactory.h"
#include "proxy.h"
#include "quizstrategy.h"   
#include "decorator.h"
 
<<<<<<< HEAD
=======
=======

>>>>>>> f7de92974797c639f914a55b802a7f7ae855553a
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
 

AnnouncementSystem announcementSystem;
 

string currentUserEmail = "";
map<string, StudentObserver*> observerRegistry;
 
<<<<<<< HEAD
=======
=======


string currentUserEmail = "";

>>>>>>> f7de92974797c639f914a55b802a7f7ae855553a
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
string readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return "<h1>File not found: " + filename + "</h1>";
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
 

void attachStudentObserver(const string& email) {
    if (observerRegistry.find(email) == observerRegistry.end()) {
        StudentObserver* obs = new StudentObserver(email);
        announcementSystem.attach(obs);
        observerRegistry[email] = obs;
    }
}
QuizStrategy* createQuizStrategy(string quizType) {

    if (quizType == "mcq") {
        return new MCQQuiz();
    }

    else if (quizType == "written") {
        return new WrittenQuiz();
    }

    else if (quizType == "viva") {
        return new VivaQuiz();
    }

    return nullptr;
}
 
Course* buildPremiumCourse(string title, bool certificate, bool notes, bool videos) {

    Course* course = new BasicCourse(title);

    if (certificate) {
        course = new CertificateDecorator(course);
    }

    if (notes) {
        course = new NotesDecorator(course);
    }

    if (videos) {
        course = new VideoBundleDecorator(course);
    }

    return course;
}
int main() {
 
    DBConnection* db = DBConnection::getInstance();
    db->connect();

    Course* studentCourse = buildPremiumCourse(
    "CSE101",
    true,   // Certificate
    true,   // Notes
    true    // Video Bundle
);

cout << "Purchased Course: "
     << studentCourse->getDescription()
     << endl;
  
    crow::SimpleApp app;
 
    CROW_ROUTE(app, "/")([]() {
        return crow::response(readFile("index.html"));
    });
 
    CROW_ROUTE(app, "/login")
    .methods("GET"_method, "POST"_method)
    ([db](const crow::request& req) {
 
        if (req.method == "GET"_method) {
            return crow::response(readFile("login.html"));
        }
 
        string body = req.body;
        string email = "";
        string password = "";
 
        size_t emailStart = body.find("email=");
        size_t passStart  = body.find("&password=");
 
        if (emailStart != string::npos && passStart != string::npos) {
            email    = body.substr(emailStart + 6, passStart - (emailStart + 6));
            password = body.substr(passStart + 10);
 
           
            size_t atPos = email.find("%40");
            if (atPos != string::npos) email.replace(atPos, 3, "@");
 
       
            while (!password.empty() &&
                   (password.back() == ' ' || password.back() == '\n' || password.back() == '\r'))
                password.pop_back();
                string role = db->loginUser(email, password);

User* currentUser = UserFactory::createUser(role);

if (currentUser == nullptr) {
    return crow::response(401, "Invalid credentials");
}

currentUser->showRole();

delete currentUser;

currentUserEmail = email;
 
          
            if (role == "student") {
 
                attachStudentObserver(email);
 
                string page = readFile("student_dashboard.html");
                size_t pos;
 
                pos = page.find("{{available_courses}}");
                if (pos != string::npos)
                    page.replace(pos, 21, db->getAvailableCoursesHTML(email));
 
                pos = page.find("{{student_courses}}");
                if (pos != string::npos)
                    page.replace(pos, 19, db->getStudentCoursesHTML(email));
 
                pos = page.find("{{student_lessons}}");
                if (pos != string::npos)
                    page.replace(pos, 19, "<p>Lessons module coming next...</p>");
 
                pos = page.find("{{student_quizzes}}");
                if (pos != string::npos)
<<<<<<< HEAD
                    page.replace(pos, 19, db->getStudentQuizzesHTML(email));
=======
                    page.replace(pos, 19, "<p>Quiz module coming next...</p>");
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
 
                pos = page.find("{{student_results}}");
                if (pos != string::npos)
                    page.replace(pos, 19, "<p>Results module coming next...</p>");
 
                pos = page.find("{{student_grades}}");
                if (pos != string::npos)
                    page.replace(pos, 18, "<p>Grades module coming next...</p>");
 
                pos = page.find("{{student_announcements}}");
                if (pos != string::npos)
                    page.replace(pos, 25, db->getStudentNotificationsHTML(email));
 
                { crow::response res(page); res.set_header("Content-Type", "text/html"); return res; }
            }
 
            else if (role == "teacher") {
 
                string page = readFile("teacher_dashboard.html");
                size_t pos;
 
                pos = page.find("{{teacher_courses_dropdown}}");
                if (pos != string::npos)
                    page.replace(pos, 28, db->getTeacherCoursesDropdown(email));
 
                pos = page.find("{{teacher_courses}}");
                if (pos != string::npos)
                    page.replace(pos, 19, db->getTeacherCoursesHTML(email));
 
                pos = page.find("{{teacher_lessons}}");
                if (pos != string::npos)
                    page.replace(pos, 19, "<p>Lessons module coming next...</p>");
 
                pos = page.find("{{teacher_quizzes}}");
                if (pos != string::npos)
<<<<<<< HEAD
                   page.replace(pos, 19, db->getTeacherQuizzesHTML(email));
=======
                    page.replace(pos, 19, "<p>Quiz module coming next...</p>");
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
 
                pos = page.find("{{teacher_grades}}");
                if (pos != string::npos)
                    page.replace(pos, 18, "<p>Grading module coming next...</p>");
 
                pos = page.find("{{teacher_enrolled_students}}");
                if (pos != string::npos)
                    page.replace(pos, 29, db->getEnrolledStudentsHTML(email));
 
                pos = page.find("{{teacher_announcements}}");
                if (pos != string::npos)
                    page.replace(pos, 25, db->getTeacherAnnouncementsHTML(email));
 
                { crow::response res(page); res.set_header("Content-Type", "text/html"); return res; }
            }
 
            else if (role == "admin") {
 
                string page = readFile("index.html");
                size_t pos;
<<<<<<< HEAD
 pos = page.find("{{total_students}}");
if (pos != string::npos)
    page.replace(pos, 18, to_string(db->getTotalStudents()));

pos = page.find("{{total_teachers}}");
if (pos != string::npos)
    page.replace(pos, 18, to_string(db->getTotalTeachers()));

pos = page.find("{{total_courses}}");
if (pos != string::npos)
    page.replace(pos, 17, to_string(db->getTotalCourses()));

pos = page.find("{{pending_approvals}}");
if (pos != string::npos)
    page.replace(pos, 21, to_string(db->getPendingApprovals()));
=======
 
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
                pos = page.find("{{admin_students}}");
                if (pos != string::npos)
                    page.replace(pos, 18, db->getAllStudentsHTML());
 
                pos = page.find("{{admin_teachers}}");
                if (pos != string::npos)
                    page.replace(pos, 18, db->getAllTeachersHTML());
 
                pos = page.find("{{admin_courses}}");
                if (pos != string::npos)
                    page.replace(pos, 17, db->getAllCoursesHTML());
 
                pos = page.find("{{admin_approvals}}");
                if (pos != string::npos)
                    page.replace(pos, 19, db->getPendingCoursesHTML());
 
                pos = page.find("{{admin_announcements}}");
                if (pos != string::npos)
                    page.replace(pos, 23, "<p>Observer announcement module coming next...</p>");
 
                pos = page.find("{{admin_reports}}");
                if (pos != string::npos)
                    page.replace(pos, 17, "<p>Reports module coming next...</p>");
 
<<<<<<< HEAD
=======
=======


int main() {

    DBConnection* db = DBConnection::getInstance();
db->connect();

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        return crow::response(readFile("index.html"));
    });

    CROW_ROUTE(app, "/login")
    .methods("GET"_method, "POST"_method)
    ([db](const crow::request& req) {

        if (req.method == "GET"_method) {
            return crow::response(readFile("login.html"));
        }

        string body = req.body;
        string email = "";
        string password = "";

        size_t emailStart = body.find("email=");
        size_t passStart  = body.find("&password=");

        if (emailStart != string::npos && passStart != string::npos) {
            email    = body.substr(emailStart + 6, passStart - (emailStart + 6));
            password = body.substr(passStart + 10);

            size_t atPos = email.find("%40");
            if (atPos != string::npos) email.replace(atPos, 3, "@");

            while (!password.empty() &&
                   (password.back() == ' ' || password.back() == '\n' || password.back() == '\r'))
                password.pop_back();

            string role = db->loginUser(email, password);
            currentUserEmail = email;

            if (role == "student") {

                string page = readFile("student_dashboard.html");
                size_t pos;

                pos = page.find("{{available_courses}}");
                if (pos != string::npos)
                    page.replace(pos, 21, db->getAvailableCoursesHTML(email));

                pos = page.find("{{student_courses}}");
                if (pos != string::npos)
                    page.replace(pos, 19, db->getStudentCoursesHTML(email));

                pos = page.find("{{student_lessons}}");
                if (pos != string::npos)
                    page.replace(pos, 19, "<p>Lessons</p>");

                pos = page.find("{{student_quizzes}}");
                if (pos != string::npos)
                    page.replace(pos, 19, "<p>Quiz module</p>");

                pos = page.find("{{student_results}}");
                if (pos != string::npos)
                    page.replace(pos, 19, "<p>Results module</p>");

                pos = page.find("{{student_grades}}");
                if (pos != string::npos)
                    page.replace(pos, 18, "<p>Grades module</p>");

                pos = page.find("{{student_announcements}}");
                if (pos != string::npos)
                    page.replace(pos, 25, "<p>Announcement module </p>");

                { crow::response res(page); res.set_header("Content-Type", "text/html"); return res; }
            }

            else if (role == "teacher") {

                string page = readFile("teacher_dashboard.html");
                size_t pos;

                pos = page.find("{{teacher_courses_dropdown}}");
                if (pos != string::npos)
                    page.replace(pos, 28, db->getTeacherCoursesDropdown(email));

                pos = page.find("{{teacher_courses}}");
                if (pos != string::npos)
                    page.replace(pos, 19, db->getTeacherCoursesHTML(email));

                pos = page.find("{{teacher_lessons}}");
                if (pos != string::npos)
                    page.replace(pos, 19, "<p>Lessons module</p>");

                pos = page.find("{{teacher_quizzes}}");
                if (pos != string::npos)
                    page.replace(pos, 19, "<p>Quiz module</p>");

                pos = page.find("{{teacher_grades}}");
                if (pos != string::npos)
                    page.replace(pos, 18, "<p>Grading module</p>");

                pos = page.find("{{teacher_enrolled_students}}");
                if (pos != string::npos)
                    page.replace(pos, 29, db->getEnrolledStudentsHTML(email));

                pos = page.find("{{teacher_announcements}}");
                if (pos != string::npos)
                    page.replace(pos, 25, "<p>Announcement module </p>");

                { crow::response res(page); res.set_header("Content-Type", "text/html"); return res; }
            }

            else if (role == "admin") {

                string page = readFile("index.html");
                size_t pos;

                pos = page.find("{{admin_students}}");
                if (pos != string::npos)
                    page.replace(pos, 18, db->getAllStudentsHTML());

                pos = page.find("{{admin_teachers}}");
                if (pos != string::npos)
                    page.replace(pos, 18, db->getAllTeachersHTML());

                pos = page.find("{{admin_courses}}");
                if (pos != string::npos)
                    page.replace(pos, 17, db->getAllCoursesHTML());

                pos = page.find("{{admin_approvals}}");
                if (pos != string::npos)
                    page.replace(pos, 19, db->getPendingCoursesHTML());

                pos = page.find("{{admin_announcements}}");
                if (pos != string::npos)
                    page.replace(pos, 23, "<p>Announcement module</p>");

                pos = page.find("{{admin_reports}}");
                if (pos != string::npos)
                    page.replace(pos, 17, "<p>Reports module</p>");

>>>>>>> f7de92974797c639f914a55b802a7f7ae855553a
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
                { crow::response res(page); res.set_header("Content-Type", "text/html"); return res; }
            }
            else {
                return crow::response(401, "Invalid credentials");
            }
        }
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
 
        return crow::response(400, "Bad request");
    });
 
<<<<<<< HEAD
=======
=======

        return crow::response(400, "Bad request");
    });

>>>>>>> f7de92974797c639f914a55b802a7f7ae855553a
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
    CROW_ROUTE(app, "/style.css")([]() {
        crow::response res(readFile("style.css"));
        res.set_header("Content-Type", "text/css");
        return res;
    });
<<<<<<< HEAD
 
=======
<<<<<<< HEAD
 
=======

>>>>>>> f7de92974797c639f914a55b802a7f7ae855553a
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
    CROW_ROUTE(app, "/approve-course")
    ([db](const crow::request& req) {
        const char* idParam = req.url_params.get("id");
        if (idParam) {
            int courseId = stoi(idParam);
            db->approveCourse(courseId);
        }
        crow::response res(302);
        res.set_header("Location", "/admin-dashboard");
        return res;
    });
<<<<<<< HEAD
 
=======
<<<<<<< HEAD
 
=======

>>>>>>> f7de92974797c639f914a55b802a7f7ae855553a
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
    CROW_ROUTE(app, "/admin-dashboard")
    ([db]() {
        string page = readFile("index.html");
        size_t pos;
<<<<<<< HEAD

        pos = page.find("{{total_students}}");
if (pos != string::npos)
    page.replace(pos, 18, to_string(db->getTotalStudents()));

pos = page.find("{{total_teachers}}");
if (pos != string::npos)
    page.replace(pos, 18, to_string(db->getTotalTeachers()));

pos = page.find("{{total_courses}}");
if (pos != string::npos)
    page.replace(pos, 17, to_string(db->getTotalCourses()));

pos = page.find("{{pending_approvals}}");
if (pos != string::npos)
    page.replace(pos, 21, to_string(db->getPendingApprovals()));
=======
<<<<<<< HEAD
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
 
        pos = page.find("{{admin_students}}");
        if (pos != string::npos) page.replace(pos, 18, db->getAllStudentsHTML());
 
        pos = page.find("{{admin_teachers}}");
        if (pos != string::npos) page.replace(pos, 18, db->getAllTeachersHTML());
 
        pos = page.find("{{admin_courses}}");
        if (pos != string::npos) page.replace(pos, 17, db->getAllCoursesHTML());
 
        pos = page.find("{{admin_approvals}}");
        if (pos != string::npos) page.replace(pos, 19, db->getPendingCoursesHTML());
 
        pos = page.find("{{admin_announcements}}");
        if (pos != string::npos) page.replace(pos, 23, "<p>Observer announcement module coming next...</p>");
 
        pos = page.find("{{admin_reports}}");
        if (pos != string::npos) page.replace(pos, 17, "<p>Reports module coming next...</p>");
 
        { crow::response res(page); res.set_header("Content-Type", "text/html"); return res; }
    });
 
<<<<<<< HEAD
=======
=======

        pos = page.find("{{admin_students}}");
        if (pos != string::npos) page.replace(pos, 18, db->getAllStudentsHTML());

        pos = page.find("{{admin_teachers}}");
        if (pos != string::npos) page.replace(pos, 18, db->getAllTeachersHTML());

        pos = page.find("{{admin_courses}}");
        if (pos != string::npos) page.replace(pos, 17, db->getAllCoursesHTML());

        pos = page.find("{{admin_approvals}}");
        if (pos != string::npos) page.replace(pos, 19, db->getPendingCoursesHTML());

        pos = page.find("{{admin_announcements}}");
        if (pos != string::npos) page.replace(pos, 23, "<p>Announcement module</p>");

        pos = page.find("{{admin_reports}}");
        if (pos != string::npos) page.replace(pos, 17, "<p>Reports module</p>");

        { crow::response res(page); res.set_header("Content-Type", "text/html"); return res; }
    });

>>>>>>> f7de92974797c639f914a55b802a7f7ae855553a
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb

    CROW_ROUTE(app, "/create-course")
    .methods("POST"_method)
    ([db](const crow::request& req) {
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
 
        string body = req.body;
        string title = "";
        string description = "";
 
        size_t titleStart = body.find("title=");
        size_t descStart  = body.find("&description=");
 
        if (titleStart != string::npos && descStart != string::npos) {
            title       = body.substr(titleStart + 6, descStart - (titleStart + 6));
            description = body.substr(descStart + 13);
 
<<<<<<< HEAD
=======
=======

        string body = req.body;
        string title = "";
        string description = "";

        size_t titleStart = body.find("title=");
        size_t descStart  = body.find("&description=");

        if (titleStart != string::npos && descStart != string::npos) {
            title       = body.substr(titleStart + 6, descStart - (titleStart + 6));
            description = body.substr(descStart + 13);

>>>>>>> f7de92974797c639f914a55b802a7f7ae855553a
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
            size_t plusPos;
            while ((plusPos = title.find("+")) != string::npos)
                title.replace(plusPos, 1, " ");
            while ((plusPos = description.find("+")) != string::npos)
                description.replace(plusPos, 1, " ");
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
 
            string teacherName = db->getUserNameByEmail(currentUserEmail);
            db->createCourse(title, description, teacherName, currentUserEmail);
        }
 
<<<<<<< HEAD
=======
=======

            string teacherName = db->getUserNameByEmail(currentUserEmail);
            db->createCourse(title, description, teacherName, currentUserEmail);
        }

>>>>>>> f7de92974797c639f914a55b802a7f7ae855553a
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
        crow::response res(302);
        res.set_header("Location", "/teacher-dashboard");
        return res;
    });
<<<<<<< HEAD

CROW_ROUTE(app, "/create-quiz")
.methods("POST"_method)
([db](const crow::request& req) {

    string body = req.body;

    string title = "";
    string quizType = "";
    string courseIdStr = "";

   size_t courseStart = body.find("course_id=");
size_t titleStart  = body.find("&title=");
size_t typeStart  = body.find("&quiz_type=");
size_t marksStart = body.find("&total_marks=");

  if (courseStart != string::npos &&
    titleStart  != string::npos &&
    typeStart   != string::npos &&
    marksStart  != string::npos) {
        courseIdStr = body.substr(courseStart + 10,
                     titleStart - (courseStart + 10));

        title = body.substr(titleStart + 7,
                typeStart - (titleStart + 7));

     quizType = body.substr(
    typeStart + 11,
    marksStart - (typeStart + 11)
);

string marksStr = body.substr(marksStart + 13);

if (courseIdStr.empty() || marksStr.empty()) {
    return crow::response(400, "Course and marks are required");
}

int totalMarks = stoi(marksStr);
       if (courseIdStr.empty()) {
    return crow::response(400, "Please select a course");
}

int courseId = stoi(courseIdStr);

        size_t plusPos;

        while ((plusPos = title.find("+")) != string::npos)
            title.replace(plusPos, 1, " ");

        QuizStrategy* strategy =
            createQuizStrategy(quizType);

        if (strategy != nullptr) {

            QuizContext quiz(strategy);

            quiz.startQuiz();

            string teacherName =
                db->getUserNameByEmail(currentUserEmail);

            db->addQuiz(courseId,
                        teacherName,
                        title,
                        quizType,totalMarks
                        );

            delete strategy;
        }
    }

    crow::response res(302);

    res.set_header("Location", "/teacher-dashboard");

    return res;
});
 
=======
<<<<<<< HEAD
 
=======

>>>>>>> f7de92974797c639f914a55b802a7f7ae855553a
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
    CROW_ROUTE(app, "/teacher-dashboard")
    ([db]() {
        string teacherEmail = currentUserEmail;
        string page = readFile("teacher_dashboard.html");
        size_t pos;
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
 
        pos = page.find("{{teacher_courses_dropdown}}");
        if (pos != string::npos)
            page.replace(pos, 28, db->getTeacherCoursesDropdown(teacherEmail));
 
        pos = page.find("{{teacher_courses}}");
        if (pos != string::npos)
            page.replace(pos, 19, db->getTeacherCoursesHTML(teacherEmail));
 
        pos = page.find("{{teacher_lessons}}");
        if (pos != string::npos)
            page.replace(pos, 19, "<p>Lessons module coming next...</p>");
 
        pos = page.find("{{teacher_quizzes}}");
        if (pos != string::npos)
<<<<<<< HEAD
         page.replace(pos, 19, db->getTeacherQuizzesHTML(teacherEmail));
=======
            page.replace(pos, 19, "<p>Quiz module coming next...</p>");
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
 
        pos = page.find("{{teacher_grades}}");
        if (pos != string::npos)
            page.replace(pos, 18, "<p>Grading module coming next...</p>");
 
        pos = page.find("{{teacher_enrolled_students}}");
        if (pos != string::npos)
            page.replace(pos, 29, db->getEnrolledStudentsHTML(teacherEmail));
 
        pos = page.find("{{teacher_announcements}}");
        if (pos != string::npos)
            page.replace(pos, 25, db->getTeacherAnnouncementsHTML(teacherEmail));
 
        { crow::response res(page); res.set_header("Content-Type", "text/html"); return res; }
    });
 
<<<<<<< HEAD
=======
=======

        pos = page.find("{{teacher_courses_dropdown}}");
        if (pos != string::npos)
            page.replace(pos, 28, db->getTeacherCoursesDropdown(teacherEmail));

        pos = page.find("{{teacher_courses}}");
        if (pos != string::npos)
            page.replace(pos, 19, db->getTeacherCoursesHTML(teacherEmail));

        pos = page.find("{{teacher_lessons}}");
        if (pos != string::npos)
            page.replace(pos, 19, "<p>Lessons module</p>");

        pos = page.find("{{teacher_quizzes}}");
        if (pos != string::npos)
            page.replace(pos, 19, "<p>Quiz module</p>");

        pos = page.find("{{teacher_grades}}");
        if (pos != string::npos)
            page.replace(pos, 18, "<p>Grading module</p>");

        pos = page.find("{{teacher_enrolled_students}}");
        if (pos != string::npos)
            page.replace(pos, 29, db->getEnrolledStudentsHTML(teacherEmail));

        pos = page.find("{{teacher_announcements}}");
        if (pos != string::npos)
            page.replace(pos, 25, "<p>Announcement module coming next...</p>");

        { crow::response res(page); res.set_header("Content-Type", "text/html"); return res; }
    });

>>>>>>> f7de92974797c639f914a55b802a7f7ae855553a
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
    CROW_ROUTE(app, "/enroll-course")
    ([db](const crow::request& req) {
        const char* idParam = req.url_params.get("id");
        if (idParam) {
            int courseId = stoi(idParam);
<<<<<<< HEAD
            EnrollmentProxy proxy;
proxy.enrollStudent(currentUserEmail, courseId);
            attachStudentObserver(currentUserEmail);
=======
<<<<<<< HEAD
            EnrollmentProxy proxy;
proxy.enrollStudent(currentUserEmail, courseId);
            attachStudentObserver(currentUserEmail);
=======
            db->enrollStudentInCourse(currentUserEmail, courseId);
>>>>>>> f7de92974797c639f914a55b802a7f7ae855553a
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
        }
        crow::response res(302);
        res.set_header("Location", "/student-dashboard");
        return res;
    });
<<<<<<< HEAD
 
CROW_ROUTE(app, "/take-quiz")
.methods("POST"_method)
([db](const crow::request& req) {

    string body = req.body;

    size_t idStart = body.find("quiz_id=");

    if (idStart == string::npos) {
        return crow::response(400, "Quiz ID missing");
    }

    string quizIdStr = body.substr(idStart + 8);

    int quizId = stoi(quizIdStr);

    cout << "TAKE QUIZ ROUTE HIT | Student: "
         << currentUserEmail
         << " | Quiz ID: "
         << quizId
         << endl;

    QuizAccessProxy proxy;

    string result = proxy.takeQuiz(currentUserEmail, quizId);

    if (result == "success") {

        crow::response res(302);
        res.set_header("Location",
                       "/quiz-page?id=" + to_string(quizId));
        return res;

    } else if (result == "already_taken") {

        return crow::response(
            403,
            "You already submitted this quiz."
        );

    } else {

        return crow::response(
            403,
            "Invalid or unavailable quiz."
        );
    }

});

=======
<<<<<<< HEAD
 
=======

>>>>>>> f7de92974797c639f914a55b802a7f7ae855553a
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
    CROW_ROUTE(app, "/student-dashboard")
    ([db]() {
        string studentEmail = currentUserEmail;
        string page = readFile("student_dashboard.html");
        size_t pos;
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
 
        pos = page.find("{{available_courses}}");
        if (pos != string::npos)
            page.replace(pos, 21, db->getAvailableCoursesHTML(studentEmail));
 
        pos = page.find("{{student_courses}}");
        if (pos != string::npos)
            page.replace(pos, 19, db->getStudentCoursesHTML(studentEmail));
 
        pos = page.find("{{student_lessons}}");
        if (pos != string::npos)
            page.replace(pos, 19, "<p>Lessons module coming next...</p>");
 
        pos = page.find("{{student_quizzes}}");
        if (pos != string::npos)
<<<<<<< HEAD
            db->viewQuizzes();
                    page.replace(pos, 19, db->getStudentQuizzesHTML(studentEmail));
         


=======
            page.replace(pos, 19, "<p>Quiz module coming next...</p>");
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
 
        pos = page.find("{{student_results}}");
        if (pos != string::npos)
            page.replace(pos, 19, "<p>Results module coming next...</p>");
 
        pos = page.find("{{student_grades}}");
        if (pos != string::npos)
            page.replace(pos, 18, "<p>Grades module coming next...</p>");
 
        pos = page.find("{{student_announcements}}");
        if (pos != string::npos)
            page.replace(pos, 25, db->getStudentNotificationsHTML(studentEmail));
 
        { crow::response res(page); res.set_header("Content-Type", "text/html"); return res; }
    });
 
    CROW_ROUTE(app, "/post-announcement")
    .methods("POST"_method)
    ([db](const crow::request& req) {
 
        string body = req.body;
 
        size_t idStart  = body.find("course_id=");
    size_t msgStart = body.find("&message=");
 
        if (idStart == string::npos || msgStart == string::npos) {
            cout << "ERROR: Invalid form data" << endl;
            return crow::response(400);
        }
 
        string courseIdStr = body.substr(idStart + 10, msgStart - (idStart + 10));
        string message     = body.substr(msgStart + 9);
 
       
        size_t pos;
        while ((pos = message.find("+")) != string::npos)
            message.replace(pos, 1, " ");
 
        auto urlDecode = [](string s) {
            string result;
            for (size_t i = 0; i < s.size(); ++i) {
                if (s[i] == '%' && i + 2 < s.size()) {
                    string hex = s.substr(i + 1, 2);
                    char c = (char)stoi(hex, nullptr, 16);
                    result += c;
                    i += 2;
                } else {
                    result += s[i];
                }
            }
            return result;
        };
        message = urlDecode(message);
 
        int courseId = stoi(courseIdStr);
        string courseTitle = db->getCourseTitleById(courseId);
 
        cout << "DEBUG courseId: "    << courseId    << endl;
        cout << "DEBUG courseTitle: " << courseTitle << endl;
        cout << "DEBUG message: "     << message     << endl;
 
   
        db->createAnnouncement(courseId, courseTitle, message, currentUserEmail);
 
        vector<string> enrolledStudents = db->getEnrolledStudents(courseId);
        for (const string& studentEmail : enrolledStudents) {
            attachStudentObserver(studentEmail);
        }
 
        announcementSystem.notify(message, courseTitle);
 
        crow::response res(302);
        res.set_header("Location", "/teacher-dashboard");
        return res;
    });
<<<<<<< HEAD
    CROW_ROUTE(app, "/quiz-page")
([db](const crow::request& req) {

    const char* idParam = req.url_params.get("id");

    if (!idParam) {
        return crow::response(400, "Quiz ID missing");
    }

    int quizId = stoi(idParam);

    string html =
        "<!DOCTYPE html>"
        "<html><head><title>Quiz Page</title></head><body>";

    html += "<h1>Quiz Started</h1>";

    html += "<p>Quiz ID: " + to_string(quizId) + "</p>";

    html += "<p>Quiz interface coming next...</p>";

    html += "<a href='/student-dashboard'>Back to Dashboard</a>";

    html += "</body></html>";

    return crow::response(html);
});
 
CROW_ROUTE(app, "/register")
.methods("POST"_method)
([db](const crow::request& req) {

    string body = req.body;

    string name = "";
    string email = "";
    string password = "";
    string role = "";

    size_t nameStart  = body.find("name=");
    size_t emailStart = body.find("&email=");
    size_t passStart  = body.find("&password=");
    size_t roleStart  = body.find("&role=");

    if (nameStart != string::npos &&
        emailStart != string::npos &&
        passStart != string::npos &&
        roleStart != string::npos) {

        name = body.substr(
            nameStart + 5,
            emailStart - (nameStart + 5)
        );

        email = body.substr(
            emailStart + 7,
            passStart - (emailStart + 7)
        );

        password = body.substr(
            passStart + 10,
            roleStart - (passStart + 10)
        );

        role = body.substr(roleStart + 6);

        size_t plusPos;

        while ((plusPos = name.find("+")) != string::npos)
            name.replace(plusPos, 1, " ");

        while ((plusPos = email.find("%40")) != string::npos)
            email.replace(plusPos, 3, "@");

        User* newUser =
            UserFactory::createUser(role);

        if (newUser == nullptr) {
            return crow::response(400, "Invalid role selected");
        }

        db->registerUser(
            name,
            email,
            password,
            role
        );

        delete newUser;

     crow::response res(302);

res.set_header("Location", "/login");

return res;
    }

    return crow::response(
        400,
        "Registration failed"
    );
});
=======
 
=======

        pos = page.find("{{available_courses}}");
        if (pos != string::npos)
            page.replace(pos, 21, db->getAvailableCoursesHTML(studentEmail));

        pos = page.find("{{student_courses}}");
        if (pos != string::npos)
            page.replace(pos, 19, db->getStudentCoursesHTML(studentEmail));

        pos = page.find("{{student_lessons}}");
        if (pos != string::npos)
            page.replace(pos, 19, "<p>Lessons module </p>");

        pos = page.find("{{student_quizzes}}");
        if (pos != string::npos)
            page.replace(pos, 19, "<p>Quiz module</p>");

        pos = page.find("{{student_results}}");
        if (pos != string::npos)
            page.replace(pos, 19, "<p>Results module</p>");

        pos = page.find("{{student_grades}}");
        if (pos != string::npos)
            page.replace(pos, 18, "<p>Grades module</p>");

        pos = page.find("{{student_announcements}}");
        if (pos != string::npos)
            page.replace(pos, 25, "<p>Announcement module</p>");

        { crow::response res(page); res.set_header("Content-Type", "text/html"); return res; }
    });

>>>>>>> f7de92974797c639f914a55b802a7f7ae855553a
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
    app.port(18080).multithreaded().run();
    return 0;
}