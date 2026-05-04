#include "dbconnection.h"
#include <jdbc/cppconn/exception.h>
#include "decorator.h"
DBConnection* DBConnection::instance = nullptr;

//Constructor 
DBConnection::DBConnection() {
    driver = nullptr;
    con = nullptr;
}

//Instance 
DBConnection* DBConnection::getInstance() {

    if (instance == nullptr) {
        instance = new DBConnection();
    }

    return instance;
}

// Connect to MySQL database
void DBConnection::connect() {

    try {
        driver = sql::mysql::get_mysql_driver_instance();

        con = driver->connect("tcp://127.0.0.1:3307", "root", "1234");

        con->setSchema("skillnest");

        cout << "Connected to SkillNest MySQL database successfully!" << endl;
    }

    catch (sql::SQLException& e) {
        cout << "Database connection failed!" << endl;
        cout << "Error: " << e.what() << endl;
    }
}


sql::Connection* DBConnection::getConnection() {
    return con;
}


bool DBConnection::addQuiz(int courseId, string teacherName, string title, string quizType, int totalMarks) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "INSERT INTO quizzes (course_id, teacher_name, title, quiz_type, total_marks) VALUES (" +
            to_string(courseId) + ", '" + teacherName + "', '" + title + "', '" +
            quizType + "', " + to_string(totalMarks) + ")";

        stmt->execute(query);

        delete stmt;

        return true;
    }

    catch (sql::SQLException& e) {
        cout << "Quiz creation failed: " << e.what() << endl;
        return false;
    }
}


bool DBConnection::gradeQuiz(int resultId, int score) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "UPDATE quiz_results SET score = " + to_string(score) +
            ", graded = TRUE WHERE id = " + to_string(resultId);

        stmt->execute(query);

        delete stmt;

        return true;
    }

    catch (sql::SQLException& e) {
        cout << "Quiz grading failed: " << e.what() << endl;
        return false;
    }
}



vector<string> DBConnection::getEnrolledStudents(int courseId) {

    vector<string> students;

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT student_email FROM enrollments WHERE course_id = " +
            to_string(courseId);

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {
            students.push_back(res->getString("student_email"));
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        cout << "Fetching enrolled students failed: "
             << e.what() << endl;
    }

    return students;
}
bool DBConnection::publishGrade(string studentEmail, int courseId, int marks, string letterGrade) {

    try {
        sql::Statement* stmt = con->createStatement();

        string checkQuery =
            "SELECT id FROM grades WHERE student_email = '" +
            studentEmail +
            "' AND course_id = " +
            to_string(courseId);

        sql::ResultSet* res = stmt->executeQuery(checkQuery);

        string query;

        if (res->next()) {

            query =
                "UPDATE grades SET marks = " +
                to_string(marks) +
                ", letter_grade = '" +
                letterGrade +
                "', published = TRUE WHERE student_email = '" +
                studentEmail +
                "' AND course_id = " +
                to_string(courseId);
        }

        else {

            query =
                "INSERT INTO grades (student_email, course_id, marks, letter_grade, published) VALUES ('" +
                studentEmail + "', " +
                to_string(courseId) + ", " +
                to_string(marks) + ", '" +
                letterGrade + "', TRUE)";
        }

        stmt->execute(query);

        delete res;
        delete stmt;

        return true;
    }

    catch (sql::SQLException& e) {
        cout << "Grade publishing failed: " << e.what() << endl;
        return false;
    }
}

string DBConnection::loginUser(string email, string password) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT role FROM users WHERE email = '" + email +
            "' AND password = '" + password + "'";

        sql::ResultSet* res = stmt->executeQuery(query);

        if (res->next()) {

            string role = res->getString("role");

            delete res;
            delete stmt;

            return role;
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        cout << "Login failed: " << e.what() << endl;
    }

    return "invalid";
}
string DBConnection::getStudentCoursesHTML(string email) {

    string html =
        "<h2>My Enrolled Courses</h2>"
        "<table border='1' cellpadding='10'>"
        "<tr><th>Course Title</th><th>Teacher</th><th>Features</th></tr>";


    try {
        sql::Statement* stmt = con->createStatement();

       string query =
    "SELECT courses.title, courses.teacher_name "
    "FROM enrollments "
    "JOIN courses ON enrollments.course_id = courses.id "
    "WHERE enrollments.student_email='" + email +
    "' AND enrollments.status='approved'";

        sql::ResultSet* res = stmt->executeQuery(query);
        

   while (res->next()) {
            string courseTitle = res->getString("title");

            Course* c = new CertificateDecorator(
                            new NotesDecorator(
                                new VideoBundleDecorator(
                                    new BasicCourse(courseTitle))));

            html += "<tr>";
            html += "<td>" + courseTitle + "</td>";
            html += "<td>" + res->getString("teacher_name") + "</td>";
            html += "<td><ul>" + c->getFeatures() + "</ul></td>";
            html += "</tr>";

            delete c;
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        cout << "Fetching courses failed: " << e.what() << endl;
    }

    html += "</table>";

    return html;
}
string DBConnection::getTeacherCoursesHTML(string teacherEmail) {

    string html =
        "<table border='1' cellpadding='10'>"
        "<tr><th>Course Title</th><th>Description</th><th>Approved</th></tr>";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT title, description, approved "
            "FROM courses "
            "WHERE teacher_email = '" + teacherEmail + "'";

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {

            html += "<tr>";

            html += "<td>" + res->getString("title") + "</td>";

            html += "<td>" + res->getString("description") + "</td>";

           if (res->getInt("approved") == 1) {
                html += "<td>Approved</td>";} 
          else {
                 html += "<td>Pending Approval</td>";}

            html += "</tr>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {

        html += "<tr><td colspan='3'>Error loading teacher courses</td></tr>";

        cout << "SQL ERROR: " << e.what() << endl;
    }

    html += "</table>";

    return html;
}
string DBConnection::getAllStudentsHTML() {

    string html =
        "<table border='1' cellpadding='10'>"
        "<tr><th>ID</th><th>Name</th><th>Email</th></tr>";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT id, name, email "
            "FROM users "
            "WHERE role = 'student'";

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {

            html += "<tr>";

            html += "<td>" + res->getString("id") + "</td>";

            html += "<td>" + res->getString("name") + "</td>";

            html += "<td>" + res->getString("email") + "</td>";

            html += "</tr>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {

        html += "<tr><td colspan='3'>Error loading students</td></tr>";

        cout << "SQL ERROR: " << e.what() << endl;
    }

    html += "</table>";

    return html;
}
string DBConnection::getAllTeachersHTML() {

    string html =
        "<table border='1' cellpadding='10'>"
        "<tr><th>ID</th><th>Name</th><th>Email</th></tr>";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT id, name, email "
            "FROM users "
            "WHERE role = 'teacher'";

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {

            html += "<tr>";

            html += "<td>" + res->getString("id") + "</td>";

            html += "<td>" + res->getString("name") + "</td>";

            html += "<td>" + res->getString("email") + "</td>";

            html += "</tr>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {

        html += "<tr><td colspan='3'>Error loading teachers</td></tr>";

        cout << "SQL ERROR: " << e.what() << endl;
    }

    html += "</table>";

    return html;
}
string DBConnection::getAllCoursesHTML() {

    string html =
        "<table border='1' cellpadding='10'>"
        "<tr><th>ID</th><th>Title</th><th>Teacher</th><th>Approved</th></tr>";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT id, title, teacher_name, approved "
            "FROM courses";

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {

            html += "<tr>";

            html += "<td>" + res->getString("id") + "</td>";

            html += "<td>" + res->getString("title") + "</td>";

            html += "<td>" + res->getString("teacher_name") + "</td>";

            html += "<td>" + res->getString("approved") + "</td>";

            html += "</tr>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {

        html += "<tr><td colspan='4'>Error loading courses</td></tr>";

        cout << "SQL ERROR: " << e.what() << endl;
    }

    html += "</table>";

    return html;
}

string DBConnection::getPendingCoursesHTML() {

    string html =
        "<table border='1' cellpadding='10'>"
      "<tr><th>ID</th><th>Title</th><th>Teacher</th><th>Status</th><th>Action</th></tr>";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT id, title, teacher_name, approved "
            "FROM courses "
            "WHERE approved = 0";

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {

            html += "<tr>";

            html += "<td>" + res->getString("id") + "</td>";

            html += "<td>" + res->getString("title") + "</td>";

            html += "<td>" + res->getString("teacher_name") + "</td>";

            html += "<td>Pending</td>";

            html += "<td><a href='/approve-course?id=" +
                res->getString("id") +
                "'>Approve</a></td>";

            html += "</tr>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {

        html += "<tr><td colspan='4'>Error loading pending courses</td></tr>";

        cout << "SQL ERROR: " << e.what() << endl;
    }

    html += "</table>";

    return html;
}

void DBConnection::approveCourse(int courseId) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "UPDATE courses "
            "SET approved = 1 "
            "WHERE id = " + to_string(courseId);

        stmt->execute(query);

        delete stmt;
    }

    catch (sql::SQLException& e) {

        cout << "SQL ERROR: " << e.what() << endl;
    }
}

string DBConnection::getUserNameByEmail(string email) {

    string userName = "";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT name "
            "FROM users "
            "WHERE email = '" + email + "'";

        sql::ResultSet* res = stmt->executeQuery(query);

        if (res->next()) {
            userName = res->getString("name");
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {

        cout << "SQL ERROR: " << e.what() << endl;
    }

    return userName;
}

void DBConnection::createCourse(string title, string description, string teacherName, string teacherEmail) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "INSERT INTO courses (title, description, teacher_name, teacher_email, approved) VALUES ('" +
            title + "', '" +
            description + "', '" +
            teacherName + "', '" +
            teacherEmail + "', 0)";

        stmt->execute(query);

        delete stmt;
    }

    catch (sql::SQLException& e) {

        cout << "SQL ERROR: " << e.what() << endl;
    }
}
string DBConnection::getPendingTeacherCoursesHTML(string teacherEmail) {

    string html =
        "<table border='1' cellpadding='10'>"
        "<tr><th>Course Title</th><th>Description</th><th>Status</th></tr>";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT title, description "
            "FROM courses "
            "WHERE teacher_email = '" + teacherEmail + "' "
            "AND approved = 0";

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {

            html += "<tr>";

            html += "<td>" + res->getString("title") + "</td>";

            html += "<td>" + res->getString("description") + "</td>";

            html += "<td>Pending Approval</td>";

            html += "</tr>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {

        html += "<tr><td colspan='3'>Error loading pending courses</td></tr>";
    }

    html += "</table>";

    return html;
}

string DBConnection::getAvailableCoursesHTML(string studentEmail) {

    string html =
        "<table border='1' cellpadding='10'>"
        "<tr><th>ID</th><th>Course Title</th><th>Teacher</th><th>Action</th></tr>";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT id, title, teacher_name "
            "FROM courses "
            "WHERE approved = 1 "
            "AND id NOT IN (SELECT course_id FROM enrollments WHERE student_email = '" + studentEmail + "')";

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {

            string courseId = res->getString("id");

            html += "<tr>";

            html += "<td>" + courseId + "</td>";

            html += "<td>" + res->getString("title") + "</td>";

            html += "<td>" + res->getString("teacher_name") + "</td>";

            html += "<td><a href='/enroll-course?id=" + courseId + "'>Enroll</a></td>";

            html += "</tr>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {

        html += "<tr><td colspan='4'>Error loading available courses</td></tr>";
    }

    html += "</table>";

    return html;
}


string DBConnection::getEnrolledStudentsHTML(string teacherEmail) {

    string html =
        "<table border='1' cellpadding='10'>"
        "<tr><th>Student Name</th><th>Student Email</th><th>Course Title</th></tr>";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT users.name AS student_name, users.email AS student_email, courses.title AS course_title "
            "FROM enrollments "
            "JOIN users ON enrollments.student_email = users.email "
            "JOIN courses ON enrollments.course_id = courses.id "
            "WHERE courses.teacher_email = '" + teacherEmail + "'";

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {

            html += "<tr>";

            html += "<td>" + res->getString("student_name") + "</td>";

            html += "<td>" + res->getString("student_email") + "</td>";

            html += "<td>" + res->getString("course_title") + "</td>";
            html += "</tr>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {

        html += "<tr><td colspan='3'>Error loading enrolled students</td></tr>";
        cout << "SQL ERROR: " << e.what() << endl;
    }

    html += "</table>";
    return html;
}

void DBConnection::createAnnouncement(int courseId, string courseTitle, string message, string postedBy) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "INSERT INTO announcements (course_id, course_title, message, posted_by) VALUES (" +
            to_string(courseId) + ", '" +
            courseTitle + "', '" +
            message + "', '" +
            postedBy + "')";

        stmt->execute(query);

        delete stmt;
    }

    catch (sql::SQLException& e) {

        cout << "SQL ERROR: " << e.what() << endl;
    }
}

string DBConnection::getStudentAnnouncementsHTML(string studentEmail) {

    string html =
        "<table border='1' cellpadding='10'>"
        "<tr><th>Course</th><th>Announcement</th><th>Posted By</th></tr>";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT announcements.course_title, announcements.message, announcements.posted_by "
            "FROM announcements "
            "JOIN enrollments ON announcements.course_id = enrollments.course_id "
            "WHERE enrollments.student_email = '" + studentEmail + "' "
            "ORDER BY announcements.id DESC";

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {

            html += "<tr>";

            html += "<td>" + res->getString("course_title") + "</td>";

            html += "<td>" + res->getString("message") + "</td>";

            html += "<td>" + res->getString("posted_by") + "</td>";

            html += "</tr>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {

        html += "<tr><td colspan='3'>Error loading announcements</td></tr>";

        cout << "SQL ERROR: " << e.what() << endl;
    }

    html += "</table>";

    return html;
}

string DBConnection::getTeacherAnnouncementsHTML(string teacherEmail) {

    string html = "";
    bool hasAny = false;

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT course_title, message "
            "FROM announcements "
            "WHERE posted_by = '" + teacherEmail + "' "
            "ORDER BY id DESC";

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {
            hasAny = true;
            string courseTitle = res->getString("course_title");
            string message     = res->getString("message");

         html += "<div style='"
        "background:#ffffff;"
        "border-left:5px solid #3498db;"
        "border-radius:10px;"
        "padding:16px 20px;"
        "margin:12px 0;"
        "box-shadow:0 4px 12px rgba(0,0,0,0.08);"
        "'>";

html += "<span style='"
        "display:inline-block;"
        "background:linear-gradient(135deg,#3498db,#2a5298);"
        "color:white;"
        "font-size:12px;"
        "font-weight:700;"
        "padding:4px 12px;"
        "border-radius:20px;"
        "margin-bottom:10px;"
        "'>" + courseTitle + "</span><br>";

html += "<span style='font-size:15px;color:#2c3e50;line-height:1.6;'>"
        + message +
        "</span>";

html += "</div>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        html += "<p style='color:red;'>Error loading announcements</p>";
        cout << "SQL ERROR: " << e.what() << endl;
    }

    if (!hasAny) {
        html += "<p style='color:#888;font-style:italic;'>No announcements posted yet.</p>";
    }

    return html;
}
int DBConnection::getCourseIdByTitle(string title) {

    int courseId = -1;

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT id FROM courses WHERE title = '" + title + "'";

        sql::ResultSet* res = stmt->executeQuery(query);

        if (res->next()) {
            courseId = res->getInt("id");
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {

        cout << "SQL ERROR: " << e.what() << endl;
    }

    return courseId;
}
string DBConnection::getCourseTitleById(int courseId) {

    string title = "";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT title FROM courses WHERE id = " +
            to_string(courseId);

        sql::ResultSet* res = stmt->executeQuery(query);

        if (res->next()) {
            title = res->getString("title");
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        cout << "Course lookup failed: " << e.what() << endl;
    }

    return title;
}

string DBConnection::getTeacherCoursesDropdown(string teacherEmail) {
    string html = "";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT id, title FROM courses "
            "WHERE teacher_email = '" + teacherEmail + "' "
            "AND approved = 1";

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {
            html += "<option value=\"" +
                    res->getString("id") +
                    "\">" +
                    res->getString("title") +
                    "</option>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        cout << "Dropdown load failed: " << e.what() << endl;
    }

    if (html == "") {
        html = "<option disabled>No approved courses available</option>";
    }

    return html;
}
string DBConnection::getQuizTitleById(int quizId) {

    string title = "";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT title FROM quizzes WHERE id = " +
            to_string(quizId);

        sql::ResultSet* res = stmt->executeQuery(query);

        if (res->next()) {
            title = res->getString("title");
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        cout << "Quiz lookup failed: " << e.what() << endl;
    }

    return title;
}
void DBConnection::addNotification(string email, string message) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "INSERT INTO notifications (email, message) VALUES ('" +
            email + "', '" +
            message + "')";

        stmt->execute(query);

        delete stmt;
    }
    catch (sql::SQLException& e) {
        cout << "DB ERROR: " << e.what() << endl;
    }
}
string DBConnection::getStudentNotificationsHTML(string email) {

    string html = "";
    bool hasAny = false;

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT message FROM notifications WHERE email = '" + email + "' ORDER BY id DESC";

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {
            hasAny = true;
            string msg = res->getString("message");

        
            string coursePart = "";
            string msgPart    = msg;
            size_t sep = msg.find(": ");
            if (sep != string::npos) {
                coursePart = msg.substr(0, sep);
                msgPart    = msg.substr(sep + 2);
            }

            html += "<div style='"
                    "background:#fff;"
                    "border-left:4px solid #4a90d9;"
                    "border-radius:6px;"
                    "padding:14px 18px;"
                    "margin:8px 0;"
                    "box-shadow:0 1px 4px rgba(0,0,0,0.08);"
                    "'>";

            if (!coursePart.empty()) {
                html += "<span style='"
                        "display:inline-block;"
                        "background:#e3f0fb;"
                        "color:#2a6099;"
                        "font-size:12px;"
                        "font-weight:700;"
                        "padding:2px 10px;"
                        "border-radius:12px;"
                        "margin-bottom:8px;"
                        "'>" + coursePart + "</span><br>";
            }

            html += "<span style='font-size:15px;color:#222;line-height:1.5;'>"
                    + msgPart + "</span>";
            html += "</div>";
        }

        delete res;
        delete stmt;
    }
    catch (sql::SQLException& e) {
        html += "<p style='color:red;'>Error loading notifications: " + string(e.what()) + "</p>";
    }

    if (!hasAny) {
        html += "<p style='color:#888;font-style:italic;'>No announcements yet.</p>";
    }

    return html;
}
string DBConnection::getAllQuizzesHTML() {

    string html =
        "<table border='1' cellpadding='10'>"
        "<tr><th>ID</th><th>Title</th><th>Type</th><th>Total Marks</th></tr>";

    try {
        sql::Statement* stmt = con->createStatement();

        sql::ResultSet* res =
            stmt->executeQuery("SELECT id, title, quiz_type, total_marks FROM quizzes");

        while (res->next()) {

            html += "<tr>";

            html += "<td>" + res->getString("id") + "</td>";

            html += "<td>" + res->getString("title") + "</td>";

            html += "<td>" + res->getString("quiz_type") + "</td>";

            html += "<td>" + res->getString("total_marks") + "</td>";

            html += "</tr>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {

        html += "<tr><td colspan='4'>Error loading quizzes</td></tr>";

        cout << "SQL ERROR: " << e.what() << endl;
    }

    html += "</table>";

    return html;
}
string DBConnection::getTeacherQuizzesHTML(string teacherEmail) {

    string html = "";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT quizzes.id, courses.title AS course_title, quizzes.title, quizzes.quiz_type, quizzes.total_marks "
            "FROM quizzes "
            "JOIN courses ON quizzes.course_id = courses.id "
            "WHERE courses.teacher_email = '" + teacherEmail + "'";

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {

            html += "<div style='"
                    "background:white;"
                    "border-radius:12px;"
                    "padding:18px 22px;"
                    "margin:14px 0;"
                    "box-shadow:0 4px 12px rgba(0,0,0,0.08);"
                    "border-left:5px solid #f39c12;"
                    "'>";

            html += "<p style='margin:0 0 8px 0;color:#888;font-size:13px;'>"
                    "Quiz ID: " + res->getString("id") +
                    "</p>";

            html += "<span style='"
                    "display:inline-block;"
                    "background:#fff3cd;"
                    "color:#d68910;"
                    "font-size:12px;"
                    "font-weight:700;"
                    "padding:4px 10px;"
                    "border-radius:20px;"
                    "margin-bottom:10px;'>"
                    + res->getString("quiz_type") +
                    "</span>";

            html += "<h3 style='margin:0;color:#2c3e50;'>"
                    + res->getString("title") +
                    "</h3>";

            html += "<p style='margin:8px 0;color:#555;'>"
                    "Course: " + res->getString("course_title") +
                    "</p>";

            html += "<p style='margin:4px 0;color:#3498db;font-weight:600;'>"
                    "Total Marks: " + res->getString("total_marks") +
                    "</p>";

            html += "</div>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {

        html += "<p>Error loading quizzes</p>";

        cout << "SQL ERROR: " << e.what() << endl;
    }

    return html;
}
string DBConnection::getStudentQuizzesHTML(string studentEmail) {

    string html =
        "<table border='1' cellpadding='10'>"
        "<tr><th>ID</th><th>Course</th><th>Title</th><th>Type</th><th>Total Marks</th></tr>";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT quizzes.id, courses.title AS course_title, quizzes.title, quizzes.quiz_type, quizzes.total_marks "
            "FROM quizzes "
            "JOIN courses ON quizzes.course_id = courses.id "
            "JOIN enrollments ON quizzes.course_id = enrollments.course_id "
            "WHERE enrollments.student_email = '" + studentEmail + "'";

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {

            html += "<tr>";

            html += "<td>" + res->getString("id") + "</td>";

            html += "<td>" + res->getString("course_title") + "</td>";

            html += "<td>" + res->getString("title") + "</td>";

            html += "<td>" + res->getString("quiz_type") + "</td>";

            html += "<td>" + res->getString("total_marks") + "</td>";

            html += "</tr>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {

        html += "<tr><td colspan='5'>Error loading quizzes</td></tr>";

        cout << "SQL ERROR: " << e.what() << endl;
    }

    html += "</table>";

    return html;
}
bool DBConnection::hasStudentTakenQuiz(string studentEmail, int quizId) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT id FROM quiz_results "
            "WHERE student_email = '" + studentEmail +
            "' AND quiz_id = " + to_string(quizId);

        sql::ResultSet* res = stmt->executeQuery(query);

        bool taken = res->next();

        delete res;
        delete stmt;

        return taken;
    }

    catch (sql::SQLException& e) {

        cout << "SQL ERROR: " << e.what() << endl;
    }

    return false;
}

bool DBConnection::takeQuiz(string studentEmail, int quizId) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "INSERT INTO quiz_results (student_email, quiz_id, score, graded) VALUES ('" +
            studentEmail + "', " +
            to_string(quizId) +
            ", NULL, FALSE)";

        stmt->execute(query);

        delete stmt;

        cout << "Quiz submission saved for "
             << studentEmail
             << " | Quiz ID: "
             << quizId
             << endl;

        return true;
    }

    catch (sql::SQLException& e) {

        cout << "Quiz submission failed: "
             << e.what()
             << endl;
    }

    return false;
}
bool DBConnection::registerUser(string name,
                                string email,
                                string password,
                                string role) {

    try {

        sql::Statement* stmt = con->createStatement();

        string query =
            "INSERT INTO users (name, email, password, role) VALUES ('" +
            name + "', '" +
            email + "', '" +
            password + "', '" +
            role + "')";

        stmt->execute(query);

        delete stmt;

        cout << "New user registered: "
             << email
             << " | Role: "
             << role
             << endl;

        return true;
    }

    catch (sql::SQLException& e) {

        cout << "Registration failed: "
             << e.what()
             << endl;
    }

    return false;
}

int DBConnection::getTotalStudents() {
    try {
        sql::Statement* stmt = con->createStatement();

        sql::ResultSet* res =
            stmt->executeQuery(
                "SELECT COUNT(*) AS total FROM users WHERE role='student'"
            );

        int total = 0;

        if (res->next()) {
            total = res->getInt("total");
        }

        delete res;
        delete stmt;

        return total;
    }
    catch (...) {}

    return 0;
}

int DBConnection::getTotalTeachers() {
    try {
        sql::Statement* stmt = con->createStatement();

        sql::ResultSet* res =
            stmt->executeQuery(
                "SELECT COUNT(*) AS total FROM users WHERE role='teacher'"
            );

        int total = 0;

        if (res->next()) {
            total = res->getInt("total");
        }

        delete res;
        delete stmt;

        return total;
    }
    catch (...) {}

    return 0;
}

int DBConnection::getTotalCourses() {
    try {
        sql::Statement* stmt = con->createStatement();

        sql::ResultSet* res =
            stmt->executeQuery(
                "SELECT COUNT(*) AS total FROM courses"
            );

        int total = 0;

        if (res->next()) {
            total = res->getInt("total");
        }

        delete res;
        delete stmt;

        return total;
    }
    catch (...) {}

    return 0;
}

int DBConnection::getPendingApprovals() {
    try {
        sql::Statement* stmt = con->createStatement();

        sql::ResultSet* res =
            stmt->executeQuery(
               "SELECT COUNT(*) AS total FROM courses WHERE approved=0"
            );

        int total = 0;

        if (res->next()) {
            total = res->getInt("total");
        }

        delete res;
        delete stmt;

        return total;
    }
    catch (...) {}

    return 0;

}
bool DBConnection::requestEnrollment(string studentEmail, int courseId) {

    try {

        sql::Statement* stmt = con->createStatement();

        string checkQuery =
            "SELECT COUNT(*) AS total FROM enrollments "
            "WHERE student_email='" + studentEmail +
            "' AND course_id=" + to_string(courseId);

        sql::ResultSet* res =
            stmt->executeQuery(checkQuery);

        if (res->next() && res->getInt("total") > 0) {

            delete res;
            delete stmt;

            return false;
        }

        delete res;

        string insertQuery =
            "INSERT INTO enrollments (student_email, course_id, status) "
            "VALUES ('" + studentEmail +
            "', " + to_string(courseId) +
            ", 'pending')";

        stmt->execute(insertQuery);

        delete stmt;

        return true;
    }

    catch (sql::SQLException& e) {

        cout << "Enrollment request failed: "
             << e.what()
             << endl;
    }

    return false;
}

string DBConnection::getPendingEnrollmentsHTML(string teacherEmail) {

    string html =
        "<table border='1' cellpadding='10'>"
        "<tr><th>ID</th><th>Student</th><th>Course</th><th>Approve</th><th>Reject</th></tr>";

    try {

        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT enrollments.id, enrollments.student_email, courses.title "
            "FROM enrollments "
            "JOIN courses ON enrollments.course_id = courses.id "
            "WHERE courses.teacher_email='" + teacherEmail +
            "' AND enrollments.status='pending'";

        sql::ResultSet* res =
            stmt->executeQuery(query);

        while (res->next()) {

            string enrollId =
                res->getString("id");

           html += "<tr>";
html += "<td>" + enrollId + "</td>";
html += "<td>" + res->getString("student_email") + "</td>";
html += "<td>" + res->getString("title") + "</td>";
html += "<td><a href='/approve-enrollment?id=" + enrollId + "'>Approve</a></td>";
html += "<td><a href='/reject-enrollment?id=" + enrollId + "'>Reject</a></td>";
html += "</tr>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {

        cout << "Pending enrollment load failed: "
             << e.what()
             << endl;
    }

    html += "</table>";

    return html;
}
bool DBConnection::approveEnrollment(int enrollmentId) {

    try {

        sql::Statement* stmt = con->createStatement();

        string query =
            "UPDATE enrollments SET status='approved' WHERE id=" +
            to_string(enrollmentId);

        stmt->execute(query);

        delete stmt;

        return true;
    }

    catch (sql::SQLException& e) {

        cout << "Enrollment approval failed: "
             << e.what()
             << endl;
    }

    return false;
}
bool DBConnection::rejectEnrollment(int enrollmentId) {

    try {

        sql::Statement* stmt = con->createStatement();

        string query =
            "UPDATE enrollments SET status='rejected' WHERE id=" +
            to_string(enrollmentId);

        stmt->execute(query);

        delete stmt;

        return true;
    }

    catch (sql::SQLException& e) {

        cout << "Enrollment rejection failed: "
             << e.what()
             << endl;
    }

    return false;
}
bool DBConnection::addLesson(int courseId,
                             string teacherEmail,
                             string title,
                             string videoUrl,
                             string content) {

    try {

        sql::Statement* stmt = con->createStatement();

        string query =
            "INSERT INTO lessons (course_id, teacher_email, title, video_url, content) VALUES (" +
            to_string(courseId) +
            ", '" + teacherEmail +
            "', '" + title +
            "', '" + videoUrl +
            "', '" + content +
            "')";

        stmt->execute(query);

        delete stmt;

        return true;
    }

    catch (sql::SQLException& e) {

        cout << "Lesson creation failed: "
             << e.what()
             << endl;
    }

    return false;
}
string DBConnection::getTeacherLessonsHTML(string teacherEmail) {

    string html = "";

    try {

        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT lessons.title, lessons.video_url, lessons.content, courses.title AS course_title "
            "FROM lessons "
            "JOIN courses ON lessons.course_id = courses.id "
            "WHERE courses.teacher_email='" + teacherEmail + "'";

        sql::ResultSet* res =
            stmt->executeQuery(query);

        while (res->next()) {

            html += "<div style='"
                    "background:white;"
                    "border-radius:12px;"
                    "padding:18px 22px;"
                    "margin:14px 0;"
                    "box-shadow:0 4px 12px rgba(0,0,0,0.08);"
                    "border-left:5px solid #2ecc71;"
                    "'>";

            html += "<span style='"
                    "display:inline-block;"
                    "background:#eafaf1;"
                    "color:#27ae60;"
                    "font-size:12px;"
                    "font-weight:700;"
                    "padding:4px 10px;"
                    "border-radius:20px;"
                    "margin-bottom:10px;'>"
                    + res->getString("course_title") +
                    "</span>";

            html += "<h3 style='margin:0;color:#2c3e50;'>"
                    + res->getString("title") +
                    "</h3>";

            html += "<p style='margin:10px 0;color:#555;line-height:1.6;'>"
                    + res->getString("content") +
                    "</p>";

            html += "<a href='" +
                    res->getString("video_url") +
                    "' target='_blank' style='"
                    "display:inline-block;"
                    "margin-top:8px;"
                    "color:#3498db;"
                    "font-weight:600;"
                    "text-decoration:none;'>Watch Video</a>";

            html += "</div>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {

        html += "<p>Error loading lessons</p>";

        cout << "Teacher lessons load failed: "
             << e.what()
             << endl;
    }

    return html;
}
string DBConnection::getStudentLessonsHTML(string studentEmail) {

    string html = "";

    try {

        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT lessons.title, lessons.video_url, lessons.content, courses.title AS course_title "
            "FROM lessons "
            "JOIN courses ON lessons.course_id = courses.id "
            "JOIN enrollments ON courses.id = enrollments.course_id "
            "WHERE enrollments.student_email='" + studentEmail +
            "' AND enrollments.status='approved'";

        sql::ResultSet* res =
            stmt->executeQuery(query);

        while (res->next()) {

            html += "<div style='"
                    "background:white;"
                    "border-radius:12px;"
                    "padding:18px 22px;"
                    "margin:14px 0;"
                    "box-shadow:0 4px 12px rgba(0,0,0,0.08);"
                    "border-left:5px solid #2ecc71;"
                    "'>";

            html += "<span style='"
                    "display:inline-block;"
                    "background:#eafaf1;"
                    "color:#27ae60;"
                    "font-size:12px;"
                    "font-weight:700;"
                    "padding:4px 10px;"
                    "border-radius:20px;"
                    "margin-bottom:10px;'>"
                    + res->getString("course_title") +
                    "</span>";

            html += "<h3 style='margin:0;color:#2c3e50;'>"
                    + res->getString("title") +
                    "</h3>";

            html += "<p style='margin:10px 0;color:#555;line-height:1.6;'>"
                    + res->getString("content") +
                    "</p>";

            html += "<a href='" +
                    res->getString("video_url") +
                    "' target='_blank' style='"
                    "display:inline-block;"
                    "margin-top:8px;"
                    "color:#3498db;"
                    "font-weight:600;"
                    "text-decoration:none;'>Watch Video</a>";

            html += "</div>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {

        html += "<p>Error loading lessons</p>";

        cout << "Student lessons load failed: "
             << e.what()
             << endl;
    }

    return html;
}

int DBConnection::getTeacherTotalCourses(string teacherEmail) {

    try {

        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT COUNT(*) AS total FROM courses "
            "WHERE teacher_email='" + teacherEmail + "'";

        sql::ResultSet* res =
            stmt->executeQuery(query);

        int total = 0;

        if (res->next()) {
            total = res->getInt("total");
        }

        delete res;
        delete stmt;

        return total;
    }

    catch (...) {}

    return 0;
}
int DBConnection::getTeacherTotalStudents(string teacherEmail) {

    try {

        sql::Statement* stmt = con->createStatement();

        string query =
          "SELECT COUNT(DISTINCT users.email) AS total "
          "FROM enrollments "
          "JOIN courses ON enrollments.course_id = courses.id "
          "JOIN users ON enrollments.student_email = users.email "
          "WHERE courses.teacher_email='" + teacherEmail +
          "' AND enrollments.status='approved'";  
        sql::ResultSet* res =
            stmt->executeQuery(query);

        int total = 0;

        if (res->next()) {
            total = res->getInt("total");
        }

        delete res;
        delete stmt;

        return total;
    }

    catch (...) {}

    return 0;
}
int DBConnection::getTeacherTotalLessons(string teacherEmail) {

    try {

        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT COUNT(*) AS total FROM lessons "
            "WHERE teacher_email='" + teacherEmail + "'";

        sql::ResultSet* res =
            stmt->executeQuery(query);

        int total = 0;

        if (res->next()) {
            total = res->getInt("total");
        }

        delete res;
        delete stmt;

        return total;
    }

    catch (...) {}

    return 0;
}
int DBConnection::getTeacherTotalQuizzes(string teacherEmail) {

    try {

        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT COUNT(*) AS total FROM quizzes "
            "WHERE teacher_name='" +
            getUserNameByEmail(teacherEmail) + "'";

        sql::ResultSet* res =
            stmt->executeQuery(query);

        int total = 0;

        if (res->next()) {
            total = res->getInt("total");
        }

        delete res;
        delete stmt;

        return total;
    }

    catch (...) {}

    return 0;
}
int DBConnection::getStudentEnrolledCourses(string studentEmail) {
 
    try {
 
        sql::Statement* stmt = con->createStatement();
 
        string query =
            "SELECT COUNT(*) AS total FROM enrollments "
            "WHERE student_email='" + studentEmail +
            "' AND status='approved'";
 
        sql::ResultSet* res =
            stmt->executeQuery(query);
 
        int total = 0;
 
        if (res->next()) {
            total = res->getInt("total");
        }
 
        delete res;
        delete stmt;
 
        return total;
    }
 
    catch (...) {}
 
    return 0;
}
 
int DBConnection::getStudentCompletedLessons(string studentEmail) {
 
    try {
 
        sql::Statement* stmt = con->createStatement();
 
        string query =
            "SELECT COUNT(*) AS total FROM lessons "
            "JOIN enrollments ON lessons.course_id = enrollments.course_id "
            "WHERE enrollments.student_email='" + studentEmail +
            "' AND enrollments.status='approved'";
 
        sql::ResultSet* res =
            stmt->executeQuery(query);
 
        int total = 0;
 
        if (res->next()) {
            total = res->getInt("total");
        }
 
        delete res;
        delete stmt;
 
        return total;
    }
 
    catch (...) {}
 
    return 0;
}
 
int DBConnection::getStudentPendingQuizzes(string studentEmail) {

    int count = 0;

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT COUNT(*) AS total "
            "FROM quizzes "
            "JOIN enrollments ON quizzes.course_id = enrollments.course_id "
            "WHERE enrollments.student_email = '" + studentEmail + "' "
           
            "AND quizzes.id NOT IN ("
            "SELECT quiz_id FROM quiz_results "
            "WHERE student_email = '" + studentEmail + "'"
            ")";

        sql::ResultSet* res = stmt->executeQuery(query);

        if (res->next()) {
            count = res->getInt("total");
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        cout << "SQL ERROR: " << e.what() << endl;
    }

    return count;
}

int DBConnection::getStudentPublishedGrades(string studentEmail) {
 
    try {
 
        sql::Statement* stmt = con->createStatement();
 
        string query =
            "SELECT COUNT(*) AS total FROM grades "
            "WHERE student_email='" + studentEmail +
            "' AND published=1";
 
        sql::ResultSet* res =
            stmt->executeQuery(query);
 
        int total = 0;
 
        if (res->next()) {
            total = res->getInt("total");
        }
 
        delete res;
        delete stmt;
 
        return total;
    }
 
    catch (...) {}
 
    return 0;
}
string DBConnection::getPendingQuizResultsHTML() {

    string html =
        "<table border='1' cellpadding='10'>"
        "<tr><th>Result ID</th><th>Student Email</th><th>Quiz ID</th></tr>";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT id, student_email, quiz_id "
            "FROM quiz_results "
            "WHERE graded = FALSE";

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {

            html += "<tr>";

            html += "<td>" + res->getString("id") + "</td>";

            html += "<td>" + res->getString("student_email") + "</td>";

            html += "<td>" + res->getString("quiz_id") + "</td>";

            html += "</tr>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {

        html += "<tr><td colspan='3'>Error loading pending quiz submissions</td></tr>";

        cout << "SQL ERROR: " << e.what() << endl;
    }

    html += "</table>";

    return html;
}
string DBConnection::getStudentQuizResultsHTML(string studentEmail) {

    string html =
        "<table border='1' cellpadding='10'>"
        "<tr><th>Quiz Title</th><th>Score</th></tr>";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT quizzes.title, quiz_results.score "
            "FROM quiz_results "
            "JOIN quizzes ON quiz_results.quiz_id = quizzes.id "
            "WHERE quiz_results.student_email = '" + studentEmail + "' "
            "AND quiz_results.graded = TRUE";

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {

            html += "<tr>";

            html += "<td>" +
                    res->getString("title") +
                    "</td>";

            html += "<td>" +
                    res->getString("score") +
                    "</td>";

            html += "</tr>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {

        html += "<tr><td colspan='2'>Error loading quiz results</td></tr>";

        cout << "SQL ERROR: " << e.what() << endl;
    }

    html += "</table>";

    return html;
}
int DBConnection::getQuizTotalMarksByResultId(int resultId) {

    int totalMarks = 0;

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT quizzes.total_marks "
            "FROM quiz_results "
            "JOIN quizzes ON quiz_results.quiz_id = quizzes.id "
            "WHERE quiz_results.id = " +
            to_string(resultId);

        sql::ResultSet* res = stmt->executeQuery(query);

        if (res->next()) {
            totalMarks = res->getInt("total_marks");
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        cout << "SQL ERROR: " << e.what() << endl;
    }

    return totalMarks;
}
string DBConnection::getStudentEmailByResultId(int resultId) {

    string studentEmail = "";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT student_email "
            "FROM quiz_results "
            "WHERE id = " +
            to_string(resultId);

        sql::ResultSet* res = stmt->executeQuery(query);

        if (res->next()) {
            studentEmail = res->getString("student_email");
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        cout << "SQL ERROR: " << e.what() << endl;
    }

    return studentEmail;
}
int DBConnection::getCourseIdByResultId(int resultId) {

    int courseId = -1;

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT quizzes.course_id "
            "FROM quiz_results "
            "JOIN quizzes ON quiz_results.quiz_id = quizzes.id "
            "WHERE quiz_results.id = " +
            to_string(resultId);

        sql::ResultSet* res = stmt->executeQuery(query);

        if (res->next()) {
            courseId = res->getInt("course_id");
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        cout << "SQL ERROR: " << e.what() << endl;
    }

    return courseId;
}
string DBConnection::getStudentGradesHTML(string studentEmail) {

    string html =
        "<table border='1' cellpadding='10'>"
        "<tr><th>Course</th><th>Marks</th><th>Letter Grade</th></tr>";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT courses.title, grades.marks, grades.letter_grade "
            "FROM grades "
            "JOIN courses ON grades.course_id = courses.id "
            "WHERE grades.student_email = '" + studentEmail + "' "
            "AND grades.published = TRUE";

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {

            html += "<tr>";

            html += "<td>" +
                    res->getString("title") +
                    "</td>";

            html += "<td>" +
                    res->getString("marks") +
                    "</td>";

            html += "<td>" +
                    res->getString("letter_grade") +
                    "</td>";

            html += "</tr>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {

        html += "<tr><td colspan='3'>Error loading grades</td></tr>";

        cout << "SQL ERROR: " << e.what() << endl;
    }

    html += "</table>";

    return html;
}
string DBConnection::getAdminAnnouncementsHTML() {

    string html =
        "<table border='1' cellpadding='10'>"
        "<tr><th>Course</th><th>Announcement</th><th>Posted By</th></tr>";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT course_title, message, posted_by "
            "FROM announcements "
            "ORDER BY id DESC";

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {
            html += "<tr>";
            html += "<td>" + res->getString("course_title") + "</td>";
            html += "<td>" + res->getString("message") + "</td>";
            html += "<td>" + res->getString("posted_by") + "</td>";
            html += "</tr>";
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        html += "<tr><td colspan='3'>Error loading announcements</td></tr>";
        cout << "SQL ERROR: " << e.what() << endl;
    }

    html += "</table>";
    return html;
}
