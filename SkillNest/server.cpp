#define CROW_MAIN
#include "userFactory.h"
#include "include/crow.h"
#include "dbconnection.h"
#include "observer.h"
#include "proxy.h"
#include "StudentObserver.h"
#include "AnnouncementSystem.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;
AnnouncementSystem announcementSystem;
 

string currentUserEmail = "";
map<string, StudentObserver*> observerRegistry;
 
string readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return "<h1>File not found: " + filename + "</h1>";
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
 

void attachStudentObserver(const string& email) {
    if (observerRegistry.find(email) == observerRegistry.end()) {
        StudentObserver* obs = new StudentObserver(email);
        announcementSystem.attach(obs);
        observerRegistry[email] = obs;
    }
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
            /*
            ================= FACTORY DESIGN PATTERN START =================
            Purpose:
            After login, the role from the database is passed to UserFactory.
            The factory creates the correct object: Student, Teacher, or Admin.
            This connects the Factory Pattern to the webpage login system.
            ================================================================
            */
            User* loggedInUser = UserFactory::createUser(role);

            if (loggedInUser == nullptr) {
                return crow::response(401, "Invalid role. Could not create user object.");
            }

            delete loggedInUser;
            /*
            ================== FACTORY DESIGN PATTERN END ==================
            */
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
                    page.replace(pos, 19, "<p>Quiz module coming next...</p>");
 
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
                    page.replace(pos, 19, "<p>Quiz module coming next...</p>");
 
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
        if (pos != string::npos) page.replace(pos, 23, "<p>Observer announcement module coming next...</p>");
 
        pos = page.find("{{admin_reports}}");
        if (pos != string::npos) page.replace(pos, 17, "<p>Reports module coming next...</p>");
 
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
            page.replace(pos, 19, "<p>Lessons module coming next...</p>");
 
        pos = page.find("{{teacher_quizzes}}");
        if (pos != string::npos)
            page.replace(pos, 19, "<p>Quiz module coming next...</p>");
 
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
 
    CROW_ROUTE(app, "/enroll-course")
    ([db](const crow::request& req) {

        const char* idParam = req.url_params.get("id");
        string resultMessage = "Enrollment failed: Invalid request.";

        if (idParam) {
            int courseId = stoi(idParam);

        /*
        ================= PROXY DESIGN PATTERN START =================
        Purpose:
        Enrollment request from frontend goes through EnrollmentProxy.
        The proxy validates the course before allowing real enrollment.
        ================================================================
        */

            EnrollmentAccess* enrollment = new EnrollmentProxy();

            resultMessage = enrollment->enroll(currentUserEmail, courseId);

            delete enrollment;

        /*
        ================== PROXY DESIGN PATTERN END ==================
        */

            cout << resultMessage << endl;

            if (resultMessage == "Enrollment successful!") {
                attachStudentObserver(currentUserEmail);
            }
        }

        crow::response res(302);

        if (resultMessage == "Enrollment successful!") {
            res.set_header("Location", "/student-dashboard?message=enrolled");
        }
        else if (resultMessage.find("already enrolled") != string::npos) {
            res.set_header("Location", "/student-dashboard?message=already");
        }
        else if (resultMessage.find("not approved") != string::npos) {
            res.set_header("Location", "/student-dashboard?message=unapproved");
        }
        else if (resultMessage.find("does not exist") != string::npos) {
            res.set_header("Location", "/student-dashboard?message=invalid");
        }
        else {
            res.set_header("Location", "/student-dashboard?message=failed");
        }

        return res;
    });
 
    CROW_ROUTE(app, "/student-dashboard")
    ([db](const crow::request& req) {
        string studentEmail = currentUserEmail;
        db->removeUnapprovedEnrollmentsForStudent(studentEmail);
        string page = readFile("student_dashboard.html");
        size_t pos;

        string messageHTML = "";

        const char* messageParam = req.url_params.get("message");

        if (messageParam) {
            string msg = messageParam;

            if (msg == "enrolled") {
                messageHTML = "<p style='color:green;font-weight:bold;'>Enrollment successful!</p>";
            }
            else if (msg == "already") {
                messageHTML = "<p style='color:#b36b00;font-weight:bold;'>You are already enrolled in this course.</p>";
            }
            else if (msg == "unapproved") {
                messageHTML = "<p style='color:red;font-weight:bold;'>This course is not approved. You cannot enroll in it.</p>";
            }
            else if (msg == "invalid") {
                messageHTML = "<p style='color:red;font-weight:bold;'>Invalid course. This course does not exist.</p>";
            }
            else {
                messageHTML = "<p style='color:red;font-weight:bold;'>Enrollment failed.</p>";
            }
        }

        pos = page.find("{{student_message}}");
        if (pos != string::npos)
            page.replace(pos, 19, messageHTML); 
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
            page.replace(pos, 19, "<p>Quiz module coming next...</p>");
 
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
 
   
        bool saved = db->createAnnouncement(courseId, courseTitle, message, currentUserEmail);

        if (!saved) {
            return crow::response(500, "Announcement could not be saved to database.");
        }

    vector<string> enrolledStudents = db->getEnrolledStudents(courseId);

    if (enrolledStudents.empty()) {
        cout << "No enrolled students found for course ID: " << courseId << endl;
    }

    for (const string& studentEmail : enrolledStudents) {

    /*
    ================= OBSERVER DESIGN PATTERN START =================
    Purpose:
    Each enrolled student is treated as an observer.
    When an announcement is posted, each observer receives
    a notification stored in the database.
    ================================================================
    */

        StudentObserver* observer = new StudentObserver(studentEmail);

        observer->update(message, courseTitle);

        delete observer;

    /*
    ================== OBSERVER DESIGN PATTERN END ==================
    */
}       crow::response res(302);
        res.set_header("Location", "/teacher-dashboard");
        return res;
    });

    CROW_ROUTE(app, "/register")
    .methods("POST"_method)
    ([db](const crow::request& req) {

        string body = req.body;

        auto urlDecode = [](string s) {
            string result;

            for (size_t i = 0; i < s.size(); i++) {
                if (s[i] == '+') {
                    result += ' ';
                }
                else if (s[i] == '%' && i + 2 < s.size()) {
                    string hex = s.substr(i + 1, 2);
                    char c = (char)stoi(hex, nullptr, 16);
                    result += c;
                    i += 2;
                }
                else {
                    result += s[i];
                }
            }

            return result;
        };

        auto getFormValue = [&](string key) {
            string searchKey = key + "=";
            size_t start = body.find(searchKey);

            if (start == string::npos) {
                return string("");
            }

            start += searchKey.length();
            size_t end = body.find("&", start);

            if (end == string::npos) {
                return urlDecode(body.substr(start));
            }

            return urlDecode(body.substr(start, end - start));
        };

        string name = getFormValue("name");
        string email = getFormValue("email");
        string password = getFormValue("password");
        string role = getFormValue("role");

        /*
        ================= FACTORY DESIGN PATTERN START =================
        Purpose:
        The frontend registration form sends name, email, password, and role.
        The backend passes the role to UserFactory.
        UserFactory creates the correct object: Student or Teacher.
        ================================================================
        */

        User* newUser = UserFactory::createUser(role);

        if (newUser == nullptr) {
            return crow::response(400, "Invalid role selected.");
        }

        newUser->setName(name);
        newUser->setEmail(email);
        newUser->setPassword(password);

        bool registered = db->registerUser(
            newUser->getName(),
            newUser->getEmail(),
            newUser->getPassword(),
            role
        );

        delete newUser;

        /*
        ================== FACTORY DESIGN PATTERN END ==================
        */

        if (registered) {
            crow::response res(302);
            res.set_header("Location", "/login");
            return res;
        }

        return crow::response(400, "Registration failed. Email may already exist.");
    });

    CROW_ROUTE(app, "/register-page")
    ([]() {
        crow::response res(readFile("register.html"));
        res.set_header("Content-Type", "text/html");
        return res;
    });

    app.port(18080).multithreaded().run();
    return 0;
}
