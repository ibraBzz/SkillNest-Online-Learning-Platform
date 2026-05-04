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
<<<<<<< HEAD
=======
<<<<<<< HEAD
=======
<<<<<<< HEAD
=======
    
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
>>>>>>> 451e74d704a00d31deef478e5ed597f4517dcb45
>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b

    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;

    DBConnection();

public:
    static DBConnection* getInstance();

    void connect();

    sql::Connection* getConnection();
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b
   
 
    bool addLesson(int courseId, string title, string videoURL, string content);
    
    bool addQuiz(int courseId, string teacherName, string title, string quizType, int totalMarks);
    
  
   bool gradeQuiz(int resultId, int score);
   bool publishGrade(string studentEmail, int courseId, int marks, string letterGrade);

<<<<<<< HEAD
=======
=======
    bool loginUser(string email, string password, string &role);
<<<<<<< HEAD
    
=======
    bool registerUser(string name, string email, string password, string role);
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
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
<<<<<<< HEAD
  
=======
   bool takeQuiz(string studentEmail, int quizId);
>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
   void viewPendingSubmissions();
   bool gradeQuiz(int resultId, int score);
   void viewQuizResults(string studentEmail);
   bool publishGrade(string studentEmail, int courseId, int marks, string letterGrade);
   void viewGrades(string studentEmail);
   void viewStudentProgress(string studentEmail);
>>>>>>> 451e74d704a00d31deef478e5ed597f4517dcb45
>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b
   vector<string> getEnrolledStudents(int courseId);
   string loginUser(string email, string password);
   string getStudentCoursesHTML(string email);
   string getTeacherCoursesHTML(string teacherEmail);
string getAllStudentsHTML();
string getAllTeachersHTML();
string getAllCoursesHTML(); 
<<<<<<< HEAD
=======
<<<<<<< HEAD
=======
string getCourseDetailsHTML();
>>>>>>> 451e74d704a00d31deef478e5ed597f4517dcb45
>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b
string getPendingCoursesHTML();
void approveCourse(int courseId);
string getUserNameByEmail(string email);
void createCourse(string title, string description,string teacherName, string teacherEmail);
string getPendingTeacherCoursesHTML(string teacherEmail);
string getAvailableCoursesHTML(string studentEmail);
<<<<<<< HEAD

=======
<<<<<<< HEAD

=======
void enrollStudentInCourse(string studentEmail, int courseId);
>>>>>>> 451e74d704a00d31deef478e5ed597f4517dcb45
>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b
string getEnrolledStudentsHTML(string teacherEmail);
void createAnnouncement(int courseId, string courseTitle, string message, string postedBy);
string getStudentAnnouncementsHTML(string studentEmail);
string getTeacherAnnouncementsHTML(string teacherEmail);
int getCourseIdByTitle(string title);
vector<int> getTeacherCourseIds(string teacherEmail);
<<<<<<< HEAD

void addNotification(string email, string message);
string getStudentNotificationsHTML(string email);
=======
<<<<<<< HEAD

void addNotification(string email, string message);
string getStudentNotificationsHTML(string email);
=======
string getCourseTitleById(int courseId);
string getTeacherCoursesDropdown(string teacherEmail);
void addNotification(string email, string message);
string getStudentNotificationsHTML(string email);
<<<<<<< HEAD
>>>>>>> 451e74d704a00d31deef478e5ed597f4517dcb45
>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b
string getAllQuizzesHTML();
string getTeacherQuizzesHTML(string teacherEmail);
string getStudentQuizzesHTML(string studentEmail);

bool hasStudentTakenQuiz(string studentEmail, int quizId);
<<<<<<< HEAD

=======
<<<<<<< HEAD

=======
bool teacherCourseExists(string teacherEmail, string title);
>>>>>>> 451e74d704a00d31deef478e5ed597f4517dcb45
>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b
bool takeQuiz(string studentEmail, int quizId);
bool registerUser(string name, string email, string password, string role);
int getTotalStudents();
int getTotalTeachers();
int getTotalCourses();
int getPendingApprovals();
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b

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
<<<<<<< HEAD
bool saveStudentFeatures(string email, string courseTitle, bool cert, bool notes, bool videos);
vector<bool> getSavedFeatures(string email, string courseTitle);
=======

>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b

};




<<<<<<< HEAD
=======
=======
};


=======

};

>>>>>>> ea47ecb20d3ff2b046e1ffb5b495339bf3b235bb
>>>>>>> 451e74d704a00d31deef478e5ed597f4517dcb45
>>>>>>> cbcd5d9be179275e877090e2ba9baa8884ce0a7b
#endif