#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <iostream>
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/exception.h>
using namespace std;

class DBConnection {
private:
    static DBConnection* instance;

    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;

    DBConnection();

public:
    static DBConnection* getInstance();

    void connect();

    sql::Connection* getConnection();
    bool loginUser(string email, string password, string &role);
    
    bool addCourse(string title, string description, string teacherName);
    void viewCourses();
    bool enrollCourse(string studentEmail, int courseId);
    void viewStudentEnrollments(string studentEmail);
    bool addAnnouncement(int courseId, string message, string postedBy);
    void viewAnnouncements();
    bool addLesson(int courseId, string title, string videoURL, string content);
    void viewLessons(int courseId);
    bool addQuiz(int courseId, string teacherName, string title, string quizType, int totalMarks);
    void viewQuizzes();
  
   void viewPendingSubmissions();
   bool gradeQuiz(int resultId, int score);
   void viewQuizResults(string studentEmail);
   bool publishGrade(string studentEmail, int courseId, int marks, string letterGrade);
   void viewGrades(string studentEmail);
   void viewStudentProgress(string studentEmail);
   vector<string> getEnrolledStudents(int courseId);
   string loginUser(string email, string password);
   string getStudentCoursesHTML(string email);
   string getTeacherCoursesHTML(string teacherEmail);
string getAllStudentsHTML();
string getAllTeachersHTML();
string getAllCoursesHTML(); 
string getCourseDetailsHTML();
string getPendingCoursesHTML();
void approveCourse(int courseId);
string getUserNameByEmail(string email);
void createCourse(string title, string description,string teacherName, string teacherEmail);
string getPendingTeacherCoursesHTML(string teacherEmail);
string getAvailableCoursesHTML(string studentEmail);
void enrollStudentInCourse(string studentEmail, int courseId);
string getEnrolledStudentsHTML(string teacherEmail);
void createAnnouncement(int courseId, string courseTitle, string message, string postedBy);
string getStudentAnnouncementsHTML(string studentEmail);
string getTeacherAnnouncementsHTML(string teacherEmail);
int getCourseIdByTitle(string title);
vector<int> getTeacherCourseIds(string teacherEmail);
string getCourseTitleById(int courseId);
string getTeacherCoursesDropdown(string teacherEmail);
void addNotification(string email, string message);
string getStudentNotificationsHTML(string email);
string getAllQuizzesHTML();
string getTeacherQuizzesHTML(string teacherEmail);
string getStudentQuizzesHTML(string studentEmail);

bool hasStudentTakenQuiz(string studentEmail, int quizId);
bool teacherCourseExists(string teacherEmail, string title);
bool takeQuiz(string studentEmail, int quizId);
bool registerUser(string name, string email, string password, string role);
int getTotalStudents();
int getTotalTeachers();
int getTotalCourses();
int getPendingApprovals();
};


#endif