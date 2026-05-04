#ifndef QUIZ_STRATEGY_H
#define QUIZ_STRATEGY_H

#include <iostream>
#include <string>
using namespace std;

/*
====================================================
DESIGN PATTERN 4: STRATEGY PATTERN START
What it does:
The Strategy Pattern allows different algorithms
to be used for the same task.

Here, different quiz types use different grading
strategies.
====================================================
*/

class QuizStrategy {
public:
    virtual int calculateScore(int correctAnswers, int totalQuestions) = 0;
};

class MCQQuizStrategy : public QuizStrategy {
public:
    int calculateScore(int correctAnswers, int totalQuestions) override {
        return (correctAnswers * 100) / totalQuestions;
    }
};

class WrittenQuizStrategy : public QuizStrategy {
public:
    int calculateScore(int correctAnswers, int totalQuestions) override {
        return (correctAnswers * 80) / totalQuestions;
    }
};

class MixedQuizStrategy : public QuizStrategy {
public:
    int calculateScore(int correctAnswers, int totalQuestions) override {
        return (correctAnswers * 90) / totalQuestions;
    }
};

class QuizGrader {
private:
    QuizStrategy* strategy;

public:
    QuizGrader(QuizStrategy* selectedStrategy) {
        strategy = selectedStrategy;
    }

    void setStrategy(QuizStrategy* selectedStrategy) {
        strategy = selectedStrategy;
    }

    int gradeQuiz(int correctAnswers, int totalQuestions) {
        if (totalQuestions == 0) {
            return 0;
        }

        return strategy->calculateScore(correctAnswers, totalQuestions);
    }
};

/*
====================================================
DESIGN PATTERN 4: STRATEGY PATTERN END
====================================================
*/

#endif
