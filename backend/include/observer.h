#pragma once
#include <vector>
#include <string>
using namespace std;
class Observer {
public:
    virtual void update(string message, string courseTitle) = 0;
};

class Subject {
protected:
    vector<Observer*> observers;

public:
    void attach(Observer* obs) {
        observers.push_back(obs);
    }

    void detach(Observer* obs) {
       
    }

    void notify(string message, string courseTitle) {
        for (auto obs : observers) {
            obs->update(message, courseTitle);
        }
    }
};