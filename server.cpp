#define CROW_MAIN
#include "include/crow.h"
#include "dbconnection.h"
#include "observer.h"
#include "StudentObserver.h"
#include "AnnouncementSystem.h"
#include "userFactory.h"
#include "proxy.h"
#include "quizstrategy.h"   
#include "decorator.h"
#include "gradingstrategy.h"
 
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
                   page.replace(pos, 19, db->getStudentLessonsHTML(email)
);
 
                pos = page.find("{{student_quizzes}}");
                if (pos != string::npos)
                    page.replace(pos, 19, db->getStudentQuizzesHTML(email));
 
                pos = page.find("{{student_results}}");
                if (pos != string::npos)
                  page.replace(pos, 19, db->getStudentQuizResultsHTML(email));
               
                   pos = page.find("{{student_grades}}");
if (pos != string::npos)
    page.replace(pos, 18, db->getStudentGradesHTML(email));
 
                pos = page.find("{{student_announcements}}");
                if (pos != string::npos)
                    page.replace(pos, 25, db->getStudentNotificationsHTML(email));
 pos = page.find("{{enrolled_courses}}");
if (pos != string::npos)
    page.replace(pos, 20, to_string(db->getStudentEnrolledCourses(email)));

pos = page.find("{{completed_lessons}}");
if (pos != string::npos)
    page.replace(pos, 21, to_string(db->getStudentCompletedLessons(email)));

pos = page.find("{{pending_quizzes}}");
if (pos != string::npos)
    page.replace(pos, 19, to_string(db->getStudentPendingQuizzes(email)));

pos = page.find("{{published_grades}}");
if (pos != string::npos)
    page.replace(pos, 20, to_string(db->getStudentPublishedGrades(email)));

                { crow::response res(page);
                     res.set_header("Content-Type", "text/html"); return res; }
            }
 
            else if (role == "teacher") {
 
                string page = readFile("teacher_dashboard.html");
                size_t pos;
                pos = page.find("{{teacher_total_courses}}");
if (pos != string::npos)
    page.replace(pos,25, to_string( db->getTeacherTotalCourses(email)
        )
    );

pos = page.find("{{teacher_total_students");
if (pos != string::npos)
    page.replace( pos,26,to_string(db->getTeacherTotalStudents(email)
        )
    );

pos = page.find("{{teacher_total_lessons}}");
if (pos != string::npos)
    page.replace( pos,25,to_string( db->getTeacherTotalLessons(email)
        )
    );

pos = page.find("{{teacher_total_quizzes}}");
if (pos != string::npos)
    page.replace( pos,  25, to_string( db->getTeacherTotalQuizzes(email)
        )
    );
 
             string dropdownHTML = db->getTeacherCoursesDropdown(email);
cout << "DROPDOWN HTML: " << dropdownHTML << endl;
 pos = 0;

while ((pos = page.find("{{teacher_courses_dropdown}}", pos)) != string::npos) {
    page.replace(pos, 28, dropdownHTML);
    pos += dropdownHTML.length();
}
 
                pos = page.find("{{teacher_courses}}");
                if (pos != string::npos)
                    page.replace(pos, 19, db->getTeacherCoursesHTML(email));
 
                pos = page.find("{{teacher_lessons}}");
                if (pos != string::npos)
                  page.replace(pos, 19, db->getTeacherLessonsHTML(email)
);
 
                pos = page.find("{{teacher_quizzes}}");
                if (pos != string::npos)
                   page.replace(pos, 19, db->getTeacherQuizzesHTML(email));
 
                pos = page.find("{{teacher_grades}}");
                if (pos != string::npos)
                  
 page.replace(pos, 18, db->getPendingQuizResultsHTML());
                pos = page.find("{{teacher_enrolled_students}}");
                if (pos != string::npos)
                    page.replace(pos, 29, db->getEnrolledStudentsHTML(email));
 
                    pos = page.find("{{teacher_pending_enrollments}}");
if (pos != string::npos)
    page.replace(
        pos,
        29,
        db->getPendingEnrollmentsHTML(email)
    );
                pos = page.find("{{teacher_announcements}}");
                if (pos != string::npos)
                    page.replace(pos, 25, db->getTeacherAnnouncementsHTML(email));
 
                { crow::response res(page); res.set_header("Content-Type", "text/html"); return res; }
            }
 
            else if (role == "admin") {
 
                string page = readFile("index.html");
                size_t pos;
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
                  page.replace(pos, 23, db->getAdminAnnouncementsHTML());
 
        
 
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
 
        pos = page.find("{{admin_students}}");
        if (pos != string::npos) page.replace(pos, 18, db->getAllStudentsHTML());
 
        pos = page.find("{{admin_teachers}}");
        if (pos != string::npos) page.replace(pos, 18, db->getAllTeachersHTML());
 
        pos = page.find("{{admin_courses}}");
        if (pos != string::npos) page.replace(pos, 17, db->getAllCoursesHTML());
 
        pos = page.find("{{admin_approvals}}");
        if (pos != string::npos) page.replace(pos, 19, db->getPendingCoursesHTML());
 
        pos = page.find("{{admin_announcements}}");
        if (pos != string::npos) page.replace(pos, 23, db->getAdminAnnouncementsHTML());
 
    
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
CROW_ROUTE(app, "/grade-quiz")
.methods("POST"_method)
([db](const crow::request& req) {

    string body = req.body;

    string resultIdStr = "";
    string scoreStr = "";

    size_t resultStart = body.find("result_id=");
    size_t scoreStart  = body.find("&score=");

    if (resultStart != string::npos &&
        scoreStart  != string::npos) {

        resultIdStr = body.substr(
            resultStart + 10,
            scoreStart - (resultStart + 10)
        );

        scoreStr = body.substr(scoreStart + 7);

        int resultId = stoi(resultIdStr);
        int score    = stoi(scoreStr);

        // Strategy Pattern
        GradingStrategy* strategy = new PercentageGrading();

        GradingContext gradingContext(strategy);
int totalMarks =
    db->getQuizTotalMarksByResultId(resultId);

string grade =
    gradingContext.grade(score, totalMarks);

        db->gradeQuiz(resultId, score);
        string studentEmail =
    db->getStudentEmailByResultId(resultId);

int courseId =
    db->getCourseIdByResultId(resultId);

db->publishGrade(
    studentEmail,
    courseId,
    score,
    grade
);

        delete strategy;
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
 pos = page.find("{{teacher_total_courses}}");
if (pos != string::npos)
    page.replace(
        pos,
        25,
        to_string(
            db->getTeacherTotalCourses(teacherEmail)
        )
    );

pos = page.find("{{teacher_total_students}}");
if (pos != string::npos)
    page.replace(
        pos,
        26,
        to_string(
            db->getTeacherTotalStudents(teacherEmail)
        )
    );

pos = page.find("{{teacher_total_lessons}}");
if (pos != string::npos)
    page.replace(
        pos,
        25,
        to_string(
            db->getTeacherTotalLessons(teacherEmail)
        )
    );

pos = page.find("{{teacher_total_quizzes}}");
if (pos != string::npos)
    page.replace(
        pos,
        25,
        to_string(
            db->getTeacherTotalQuizzes(teacherEmail)
        )
    );
  
    //dropdown replacement  
  
    string dropdownHTML = db->getTeacherCoursesDropdown(teacherEmail);
cout << "DROPDOWN HTML: " << dropdownHTML << endl;

string placeholder = "{{teacher_courses_dropdown}}";

size_t testPos = 0;
int count = 0;

while ((testPos = page.find(placeholder, testPos)) != string::npos) {
    count++;
    testPos += placeholder.length();
}

cout << "Teacher dropdown placeholders found: " << count << endl;

 pos = 0;

while ((pos = page.find(placeholder, pos)) != string::npos) {
    page.replace(pos, placeholder.length(), dropdownHTML);
    pos += dropdownHTML.length();
}
  



pos = page.find("{{teacher_courses}}");
        if (pos != string::npos)
            page.replace(pos, 23, db->getTeacherCoursesHTML(teacherEmail));
 
        pos = page.find("{{teacher_lessons}}");
        if (pos != string::npos)
            page.replace( pos, 23, db->getTeacherLessonsHTML(teacherEmail)
);
 
        pos = page.find("{{teacher_quizzes}}");
        if (pos != string::npos)
         page.replace(pos, 23, db->getTeacherQuizzesHTML(teacherEmail));
 
        pos = page.find("{{teacher_grades}}");
        if (pos != string::npos)
           page.replace(pos, 18, db->getPendingQuizResultsHTML());
 
        pos = page.find("{{teacher_enrolled_students}}");
        if (pos != string::npos)
            page.replace(pos, 29, db->getEnrolledStudentsHTML(teacherEmail));
 pos = page.find("{{teacher_pending_enrollments}}");
if (pos != string::npos)
    page.replace(
        pos,
        29,
        db->getPendingEnrollmentsHTML(teacherEmail)
    );
        pos = page.find("{{teacher_announcements}}");
        if (pos != string::npos)
            page.replace(pos, 25, db->getTeacherAnnouncementsHTML(teacherEmail));
 
        crow::response res(page);
res.set_header("Content-Type", "text/html");
return res;
    });
 
    CROW_ROUTE(app, "/enroll-course")
    ([db](const crow::request& req) {
        const char* idParam = req.url_params.get("id");
        if (idParam) {
            int courseId = stoi(idParam);
            EnrollmentProxy proxy;
proxy.enrollStudent(currentUserEmail, courseId);
            attachStudentObserver(currentUserEmail);
        }
        crow::response res(302);
        res.set_header("Location", "/student-dashboard");
        return res;
    });
 CROW_ROUTE(app, "/approve-enrollment")
([db](const crow::request& req) {

    const char* idParam = req.url_params.get("id");

    if (idParam) {

        int enrollmentId = stoi(idParam);

        db->approveEnrollment(enrollmentId);
    }

    crow::response res(302);

   res.set_header("Location", "/teacher-dashboard?refresh=1");

    return res;
});

CROW_ROUTE(app, "/reject-enrollment")
([db](const crow::request& req) {

    const char* idParam = req.url_params.get("id");

    if (idParam) {

        int enrollmentId = stoi(idParam);

        db->rejectEnrollment(enrollmentId);
    }

    crow::response res(302);

  res.set_header("Location", "/teacher-dashboard?refresh=1");

    return res;
});
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
            page.replace(pos,19,db->getStudentLessonsHTML(studentEmail)
);
 
        pos = page.find("{{student_quizzes}}");
        if (pos != string::npos)
            page.replace(pos, 19, db->getStudentQuizzesHTML(studentEmail));
 
        pos = page.find("{{student_results}}");
        if (pos != string::npos)
            page.replace(pos, 19, db->getStudentQuizResultsHTML(studentEmail));
 
        pos = page.find("{{student_grades}}");
        if (pos != string::npos)
            page.replace(pos, 18, db->getStudentGradesHTML(studentEmail));
 
        pos = page.find("{{student_announcements}}");
        if (pos != string::npos)
            page.replace(pos, 25, db->getStudentNotificationsHTML(studentEmail));
         
pos = page.find("{{enrolled_courses}}");
if (pos != string::npos)
    page.replace(pos, 20, to_string(db->getStudentEnrolledCourses(studentEmail)));

pos = page.find("{{completed_lessons}}");
if (pos != string::npos)
    page.replace(pos, 21, to_string(db->getStudentCompletedLessons(studentEmail)));

pos = page.find("{{pending_quizzes}}");
if (pos != string::npos)
    page.replace(pos, 19, to_string(db->getStudentPendingQuizzes(studentEmail)));

pos = page.find("{{published_grades}}");
if (pos != string::npos)
    page.replace(pos, 20, to_string(db->getStudentPublishedGrades(studentEmail)));

 
        pos = page.find("{{student_results}}");
        if (pos != string::npos)
           page.replace(pos, 19, db->getStudentQuizResultsHTML(studentEmail));
 
        pos = page.find("{{student_grades}}");
        if (pos != string::npos)
pos = page.find("{{student_grades}}");
if (pos != string::npos)
    page.replace(pos, 18, db->getStudentGradesHTML(studentEmail));
 
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

CROW_ROUTE(app, "/create-lesson")
.methods("POST"_method)
([db](const crow::request& req) {

    string body = req.body;

    size_t courseStart = body.find("course_id=");
    size_t titleStart  = body.find("&title=");
    size_t videoStart  = body.find("&video_url=");
    size_t contentStart = body.find("&content=");

    if (courseStart == string::npos ||
        titleStart == string::npos ||
        videoStart == string::npos ||
        contentStart == string::npos) {

        return crow::response(400, "Missing lesson fields");
    }

    string courseIdStr =
        body.substr(
            courseStart + 10,
            titleStart - (courseStart + 10)
        );

    string title =
        body.substr(
            titleStart + 7,
            videoStart - (titleStart + 7)
        );

    string videoUrl =
        body.substr(
            videoStart + 11,
            contentStart - (videoStart + 11)
        );

    string content =
        body.substr(contentStart + 9);

    size_t plusPos;

    while ((plusPos = title.find("+")) != string::npos)
        title.replace(plusPos, 1, " ");

    while ((plusPos = content.find("+")) != string::npos)
        content.replace(plusPos, 1, " ");
        while ((plusPos = videoUrl.find("%3A")) != string::npos)
    videoUrl.replace(plusPos, 3, ":");

while ((plusPos = videoUrl.find("%2F")) != string::npos)
    videoUrl.replace(plusPos, 3, "/");

    int courseId = stoi(courseIdStr);

    db->addLesson(
        courseId,
        currentUserEmail,
        title,
        videoUrl,
        content
    );

    crow::response res(302);

    res.set_header("Location", "/teacher-dashboard");

    return res;
});
CROW_ROUTE(app, "/save-features")
.methods("POST"_method)
([db](const crow::request& req) {

    string body = req.body;

    string courseTitle = "";
    bool certificate = body.find("certificate=on") != string::npos;
    bool notes = body.find("notes=on") != string::npos;
    bool videos = body.find("videos=on") != string::npos;

  size_t courseStart = body.find("course_title=");
if (courseStart != string::npos) {

    size_t endPos = body.find("&", courseStart);

    if (endPos == string::npos) {
        courseTitle = body.substr(courseStart + 13);
    } else {
        courseTitle = body.substr(
            courseStart + 13,
            endPos - (courseStart + 13)
        );
    }

    size_t plusPos;
    while ((plusPos = courseTitle.find("+")) != string::npos)
        courseTitle.replace(plusPos, 1, " ");
}
cout << "Saving Course: " << courseTitle << endl;
cout << "Cert: " << certificate
     << " Notes: " << notes
     << " Videos: " << videos << endl;

    db->saveStudentFeatures(
        currentUserEmail,
        courseTitle,
        certificate,
        notes,
        videos
    );

    crow::response res(302);
    res.set_header("Location", "/student-dashboard ");
    return res;
});



    app.port(18080).multithreaded().run();
    return 0;
}