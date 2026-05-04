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
class VivaQuiz : public QuizStrategy {
public:
    void conductQuiz() override {
        cout << "Conducting Viva Exam..." << endl;
        cout << "Student answers oral questions live." << endl;
    }
};
class QuizContext {
private:
    QuizStrategy* strategy;

public:
    QuizContext(QuizStrategy* s) {
        strategy = s;
    }

    void setStrategy(QuizStrategy* s) {
        strategy = s;
    }

    void startQuiz() {
        strategy->conductQuiz();
    }
};

#endif