#pragma once
#include "observer.h"
#include <string>
using namespace std;

class AnnouncementSystem : public Subject {

public:

    void createAnnouncement(string courseTitle, string message) {

       
    notify(courseTitle, message);
    }
};