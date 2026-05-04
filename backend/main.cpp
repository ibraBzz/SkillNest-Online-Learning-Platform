#include <iostream>
#include "UserFactory.h"
#include "CourseService.h"
#include "AuthProxy.h"

using namespace std;

int main() {
    cout << "===== SkillNest Backend Pattern Test =====\n\n";

    cout << "Testing Factory Pattern\n";
    User* student = UserFactory::createUser("student", "Nuha", "student@skillnest.com");
    User* teacher = UserFactory::createUser("teacher", "Ibra", "teacher@skillnest.com");
    User* admin = UserFactory::createUser("admin", "Admin", "admin@skillnest.com");

    student->showDashboard();
    teacher->showDashboard();
    admin->showDashboard();

    cout << "\nTesting Singleton Pattern\n";
    CourseService* service1 = CourseService::getInstance();
    CourseService* service2 = CourseService::getInstance();

    service1->createCourse("Introduction to C++");

    if (service1 == service2) {
        cout << "Singleton works: Both objects are the same instance.\n";
    } else {
        cout << "Singleton failed: Different instances were created.\n";
    }

    cout << "\nTesting Proxy Pattern\n";

    AuthProxy studentProxy(student);
    AuthProxy teacherProxy(teacher);
    AuthProxy adminProxy(admin);

    studentProxy.checkAccess("enroll_course");
    studentProxy.checkAccess("approve_course");

    teacherProxy.checkAccess("create_course");
    teacherProxy.checkAccess("approve_course");

    adminProxy.checkAccess("approve_course");
    adminProxy.checkAccess("create_course");

    delete student;
    delete teacher;
    delete admin;

    return 0;
}