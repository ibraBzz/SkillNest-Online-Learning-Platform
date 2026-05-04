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
   
 
    bool addLesson(int courseId, string title, string videoURL, string content);
    
    bool addQuiz(int courseId, string teacherName, string title, string quizType, int totalMarks);
    
  
   bool gradeQuiz(int resultId, int score);
   bool publishGrade(string studentEmail, int courseId, int marks, string letterGrade);

   vector<string> getEnrolledStudents(int courseId);
   string loginUser(string email, string password);
   string getStudentCoursesHTML(string email);
   string getTeacherCoursesHTML(string teacherEmail);
string getAllStudentsHTML();
string getAllTeachersHTML();
string getAllCoursesHTML(); 
string getPendingCoursesHTML();
void approveCourse(int courseId);
string getUserNameByEmail(string email);
void createCourse(string title, string description,string teacherName, string teacherEmail);
string getPendingTeacherCoursesHTML(string teacherEmail);
string getAvailableCoursesHTML(string studentEmail);

string getEnrolledStudentsHTML(string teacherEmail);
void createAnnouncement(int courseId, string courseTitle, string message, string postedBy);
string getStudentAnnouncementsHTML(string studentEmail);
string getTeacherAnnouncementsHTML(string teacherEmail);
int getCourseIdByTitle(string title);
vector<int> getTeacherCourseIds(string teacherEmail);

void addNotification(string email, string message);
string getStudentNotificationsHTML(string email);
string getAllQuizzesHTML();
string getTeacherQuizzesHTML(string teacherEmail);
string getStudentQuizzesHTML(string studentEmail);

bool hasStudentTakenQuiz(string studentEmail, int quizId);

bool takeQuiz(string studentEmail, int quizId);
bool registerUser(string name, string email, string password, string role);
int getTotalStudents();
int getTotalTeachers();
int getTotalCourses();
int getPendingApprovals();

bool requestEnrollment(string studentEmail, int courseId);
string getPendingEnrollmentsHTML(string teacherEmail);
bool approveEnrollment(int enrollmentId);
bool rejectEnrollment(int enrollmentId);
bool addLesson(int courseId, string teacherEmail, string title, string videoUrl,string content);

string getTeacherLessonsHTML(string teacherEmail);

string getStudentLessonsHTML(string studentEmail);

int getTeacherTotalCourses(string teacherEmail);
int getTeacherTotalStudents(string teacherEmail);
int getTeacherTotalLessons(string teacherEmail);
int getTeacherTotalQuizzes(string teacherEmail);

int getStudentEnrolledCourses(string studentEmail);
int getStudentCompletedLessons(string studentEmail);
int getStudentPendingQuizzes(string studentEmail);
int getStudentPublishedGrades(string studentEmail);

string getQuizTitleById(int quizId);
string getTeacherCoursesDropdown(string teacherEmail);
string getCourseTitleById(int courseId);

string getPendingQuizResultsHTML();
string getStudentQuizResultsHTML(string studentEmail);
int getQuizTotalMarksByResultId(int resultId);
string getStudentEmailByResultId(int resultId);
int getCourseIdByResultId(int resultId);
string getStudentGradesHTML(string studentEmail);

string getAdminAnnouncementsHTML();
bool saveStudentFeatures(string email, string courseTitle, bool cert, bool notes, bool videos);
vector<bool> getSavedFeatures(string email, string courseTitle);

};




#endif