#define CROW_MAIN
#include "include/crow.h"
#include "dbconnection.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;


string currentUserEmail = "";

string readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return "<h1>File not found: " + filename + "</h1>";
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


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

                { crow::response res(page); res.set_header("Content-Type", "text/html"); return res; }
            }
            else {
                return crow::response(401, "Invalid credentials");
            }
        }

        return crow::response(400, "Bad request");
    });

    CROW_ROUTE(app, "/style.css")([]() {
        crow::response res(readFile("style.css"));
        res.set_header("Content-Type", "text/css");
        return res;
    });

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

    CROW_ROUTE(app, "/admin-dashboard")
    ([db]() {
        string page = readFile("index.html");
        size_t pos;

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


    CROW_ROUTE(app, "/create-course")
    .methods("POST"_method)
    ([db](const crow::request& req) {

        string body = req.body;
        string title = "";
        string description = "";

        size_t titleStart = body.find("title=");
        size_t descStart  = body.find("&description=");

        if (titleStart != string::npos && descStart != string::npos) {
            title       = body.substr(titleStart + 6, descStart - (titleStart + 6));
            description = body.substr(descStart + 13);

            size_t plusPos;
            while ((plusPos = title.find("+")) != string::npos)
                title.replace(plusPos, 1, " ");
            while ((plusPos = description.find("+")) != string::npos)
                description.replace(plusPos, 1, " ");

            string teacherName = db->getUserNameByEmail(currentUserEmail);
            db->createCourse(title, description, teacherName, currentUserEmail);
        }

        crow::response res(302);
        res.set_header("Location", "/teacher-dashboard");
        return res;
    });

    CROW_ROUTE(app, "/teacher-dashboard")
    ([db]() {
        string teacherEmail = currentUserEmail;
        string page = readFile("teacher_dashboard.html");
        size_t pos;

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

    CROW_ROUTE(app, "/enroll-course")
    ([db](const crow::request& req) {
        const char* idParam = req.url_params.get("id");
        if (idParam) {
            int courseId = stoi(idParam);
            db->enrollStudentInCourse(currentUserEmail, courseId);
        }
        crow::response res(302);
        res.set_header("Location", "/student-dashboard");
        return res;
    });

    CROW_ROUTE(app, "/student-dashboard")
    ([db]() {
        string studentEmail = currentUserEmail;
        string page = readFile("student_dashboard.html");
        size_t pos;

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

    app.port(18080).multithreaded().run();
    return 0;
}