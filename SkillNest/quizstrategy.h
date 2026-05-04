#ifndef QUIZSTRATEGY_H
#define QUIZSTRATEGY_H

#include <iostream>
#include <string>

using namespace std;

class QuizStrategy {
public:
    virtual void conductQuiz() = 0;
    virtual ~QuizStrategy() {}
};

class MCQQuiz : public QuizStrategy {
public:
    void conductQuiz() override {
        cout << "Conducting MCQ Quiz..." << endl;
        cout << "Student answers multiple choice questions." << endl;
    }
};

class WrittenQuiz : public QuizStrategy {
public:
    void conductQuiz() override {
        cout << "Conducting Written Quiz..." << endl;
        cout << "Student writes descriptive answers." << endl;
    }
};

#endif