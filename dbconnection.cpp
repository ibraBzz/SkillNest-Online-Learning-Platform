#include "dbconnection.h"
#include <jdbc/cppconn/exception.h>

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


bool DBConnection::loginUser(string email, string password, string &role) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query = "SELECT role FROM users WHERE email='" + email +
                       "' AND password='" + password + "'";

        sql::ResultSet* res = stmt->executeQuery(query);

        if (res->next()) {
            role = res->getString("role");

            delete res;
            delete stmt;

            return true;
        }

        delete res;
        delete stmt;

        return false;
    }

    catch (sql::SQLException& e) {
        cout << "Login query failed: " << e.what() << endl;
        return false;
    }
}

bool DBConnection::registerUser(string name, string email, string password, string role) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query = "INSERT INTO users (name, email, password, role) VALUES ('" +
                       name + "', '" + email + "', '" + password + "', '" + role + "')";

        stmt->execute(query);

        delete stmt;

        return true;
    }

    catch (sql::SQLException& e) {
        cout << "Registration failed: " << e.what() << endl;
        return false;
    }
}


bool DBConnection::addCourse(string title, string description, string teacherName) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query = "INSERT INTO courses (title, description, teacher_name) VALUES ('" +
                       title + "', '" + description + "', '" + teacherName + "')";

        stmt->execute(query);

        delete stmt;

        return true;
    }

    catch (sql::SQLException& e) {
        cout << "Course creation failed: " << e.what() << endl;
        return false;
    }
}


void DBConnection::viewCourses() {

    try {
        sql::Statement* stmt = con->createStatement();

        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM courses");

        cout << "\n=== Available Courses ===" << endl;

        while (res->next()) {
            cout << "Course ID: " << res->getInt("id") << endl;
            cout << "Title: " << res->getString("title") << endl;
            cout << "Description: " << res->getString("description") << endl;
            cout << "Teacher: " << res->getString("teacher_name") << endl;
            cout << "-------------------------" << endl;
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        cout << "Course viewing failed: " << e.what() << endl;
    }
}
bool DBConnection::enrollCourse(string studentEmail, int courseId) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query = "INSERT INTO enrollments (student_email, course_id) VALUES ('" +
                       studentEmail + "', " + to_string(courseId) + ")";

        stmt->execute(query);

        delete stmt;

        return true;
    }

    catch (sql::SQLException& e) {
        cout << "Enrollment failed: " << e.what() << endl;
        return false;
    }
}


void DBConnection::viewStudentEnrollments(string studentEmail) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT courses.id, courses.title, courses.description, courses.teacher_name "
            "FROM enrollments "
            "JOIN courses ON enrollments.course_id = courses.id "
            "WHERE enrollments.student_email = '" + studentEmail + "'";

        sql::ResultSet* res = stmt->executeQuery(query);

        cout << "\n=== Enrolled Courses ===" << endl;

        while (res->next()) {
            cout << "Course ID: " << res->getInt("id") << endl;
            cout << "Title: " << res->getString("title") << endl;
            cout << "Description: " << res->getString("description") << endl;
            cout << "Teacher: " << res->getString("teacher_name") << endl;
            cout << "-------------------------" << endl;
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        cout << "Viewing enrollments failed: " << e.what() << endl;
    }
}
bool DBConnection::addLesson(int courseId, string title, string videoURL, string content) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "INSERT INTO lessons (course_id, title, video_url, content) VALUES (" +
            to_string(courseId) + ", '" + title + "', '" + videoURL + "', '" + content + "')";

        stmt->execute(query);

        delete stmt;

        return true;
    }

    catch (sql::SQLException& e) {
        cout << "Lesson upload failed: " << e.what() << endl;
        return false;
    }
}

void DBConnection::viewLessons(int courseId) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query = "SELECT * FROM lessons WHERE course_id = " + to_string(courseId);

        sql::ResultSet* res = stmt->executeQuery(query);

        cout << "\n=== Course Lessons ===" << endl;

        while (res->next()) {
            cout << "Lesson ID: " << res->getInt("id") << endl;
            cout << "Title: " << res->getString("title") << endl;
            cout << "Video URL: " << res->getString("video_url") << endl;
            cout << "Content: " << res->getString("content") << endl;
            cout << "-------------------------" << endl;
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        cout << "Viewing lessons failed: " << e.what() << endl;
    }
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

void DBConnection::viewQuizzes() {

    try {
        sql::Statement* stmt = con->createStatement();

        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM quizzes");

        cout << "\n=== Available Quizzes ===" << endl;

        while (res->next()) {
            cout << "Quiz ID: " << res->getInt("id") << endl;
            cout << "Course ID: " << res->getInt("course_id") << endl;
            cout << "Teacher: " << res->getString("teacher_name") << endl;
            cout << "Title: " << res->getString("title") << endl;
            cout << "Type: " << res->getString("quiz_type") << endl;
            cout << "Total Marks: " << res->getInt("total_marks") << endl;
            cout << "-------------------------" << endl;
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        cout << "Viewing quizzes failed: " << e.what() << endl;
    }
}
bool DBConnection::takeQuiz(string studentEmail, int quizId) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "INSERT INTO quiz_results (student_email, quiz_id, score, graded) VALUES ('" +
            studentEmail + "', " + to_string(quizId) + ", NULL, FALSE)";

        stmt->execute(query);

        delete stmt;

        return true;
    }

    catch (sql::SQLException& e) {
        cout << "Quiz submission failed: " << e.what() << endl;
        return false;
    }
}
void DBConnection::viewPendingSubmissions() {

    try {
        sql::Statement* stmt = con->createStatement();

        sql::ResultSet* res =
            stmt->executeQuery("SELECT * FROM quiz_results WHERE graded = FALSE");

        cout << "\n=== Pending Quiz Submissions ===" << endl;

        while (res->next()) {
            cout << "Result ID: " << res->getInt("id") << endl;
            cout << "Student Email: " << res->getString("student_email") << endl;
            cout << "Quiz ID: " << res->getInt("quiz_id") << endl;
            cout << "-------------------------" << endl;
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        cout << "Viewing pending submissions failed: " << e.what() << endl;
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


void DBConnection::viewQuizResults(string studentEmail) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT quizzes.title, quiz_results.score "
            "FROM quiz_results "
            "JOIN quizzes ON quiz_results.quiz_id = quizzes.id "
            "WHERE quiz_results.student_email = '" + studentEmail +
            "' AND quiz_results.graded = TRUE";

        sql::ResultSet* res = stmt->executeQuery(query);

        cout << "\n=== Final Quiz Results ===" << endl;

        while (res->next()) {
            cout << "Quiz: " << res->getString("title") << endl;
            cout << "Score: " << res->getInt("score") << endl;
            cout << "-------------------------" << endl;
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        cout << "Viewing quiz results failed: " << e.what() << endl;
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

        string query =
            "INSERT INTO grades (student_email, course_id, marks, letter_grade, published) VALUES ('" +
            studentEmail + "', " + to_string(courseId) + ", " +
            to_string(marks) + ", '" + letterGrade + "', TRUE)";

        stmt->execute(query);

        delete stmt;

        return true;
    }

    catch (sql::SQLException& e) {
        cout << "Grade publishing failed: " << e.what() << endl;
        return false;
    }
}


void DBConnection::viewGrades(string studentEmail) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT courses.title, grades.marks, grades.letter_grade "
            "FROM grades "
            "JOIN courses ON grades.course_id = courses.id "
            "WHERE grades.student_email = '" + studentEmail +
            "' AND grades.published = TRUE";

        sql::ResultSet* res = stmt->executeQuery(query);

        cout << "\n=== Published Grades ===" << endl;

        while (res->next()) {
            cout << "Course: " << res->getString("title") << endl;
            cout << "Marks: " << res->getInt("marks") << endl;
            cout << "Grade: " << res->getString("letter_grade") << endl;
            cout << "-------------------------" << endl;
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        cout << "Viewing grades failed: " << e.what() << endl;
    }
}
void DBConnection::viewStudentProgress(string studentEmail) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "SELECT courses.title, progress.progress_percent, progress.completed "
            "FROM progress "
            "JOIN courses ON progress.course_id = courses.id "
            "WHERE progress.student_email = '" + studentEmail + "'";

        sql::ResultSet* res = stmt->executeQuery(query);

        cout << "\n=== Student Progress Report ===" << endl;

        while (res->next()) {

            cout << "Course: " << res->getString("title") << endl;

            cout << "Progress: "
                 << res->getInt("progress_percent")
                 << "%" << endl;

            cout << "Completed: "
                 << (res->getBoolean("completed") ? "Yes" : "No")
                 << endl;

            cout << "-------------------------" << endl;
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        cout << "Viewing progress failed: "
             << e.what() << endl;
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
        "<tr><th>Course Title</th><th>Teacher</th></tr>";

    try {
        sql::Statement* stmt = con->createStatement();

       string query =
    "SELECT courses.title, courses.teacher_name "
    "FROM enrollments "
    "JOIN courses ON enrollments.course_id = courses.id "
    "WHERE enrollments.student_email = '" + email + "'";

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {

            html += "<tr>";

            html += "<td>" + res->getString("title") + "</td>";

            html += "<td>" + res->getString("teacher_name") + "</td>";

            html += "</tr>";
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

void DBConnection::enrollStudentInCourse(string studentEmail, int courseId) {

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
            "INSERT INTO enrollments (student_email, course_id) VALUES ('" +
            studentEmail + "', " +
            to_string(courseId) + ")";

        stmt->execute(query);

        delete stmt;
    }

    catch (sql::SQLException& e) {

        cout << "SQL ERROR: " << e.what() << endl;
    }
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
                    "background:#fff;"
                    "border-left:4px solid #4a90d9;"
                    "border-radius:6px;"
                    "padding:14px 18px;"
                    "margin:8px 0;"
                    "box-shadow:0 1px 4px rgba(0,0,0,0.08);"
                    "'>";

            html += "<span style='"
                    "display:inline-block;"
                    "background:#e3f0fb;"
                    "color:#2a6099;"
                    "font-size:12px;"
                    "font-weight:700;"
                    "padding:2px 10px;"
                    "border-radius:12px;"
                    "margin-bottom:8px;"
                    "'>" + courseTitle + "</span><br>";

            html += "<span style='font-size:15px;color:#222;line-height:1.5;'>"
                    + message + "</span>";
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
            "SELECT title FROM courses WHERE id = " + to_string(courseId);

        sql::ResultSet* res = stmt->executeQuery(query);

        if (res->next()) {
            title = res->getString("title");
        }

        delete res;
        delete stmt;
    }

    catch (sql::SQLException& e) {
        cout << "SQL ERROR getCourseTitleById: " << e.what() << endl;
    }

    return title;
}

string DBConnection::getTeacherCoursesDropdown(string teacherEmail) {

    string html = "";

    try {
        sql::Statement* stmt = con->createStatement();

        string query =
             "SELECT id, title FROM courses WHERE teacher_email = '" + teacherEmail + "' AND approved = 1";

        sql::ResultSet* res = stmt->executeQuery(query);

        while (res->next()) {
            html += "<option value='" + res->getString("id") + "'>";
            html += res->getString("title");
            html += "</option>";
        }

        delete res;
        delete stmt;
    }
    catch (...) {}

    return html;
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