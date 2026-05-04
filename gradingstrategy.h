#ifndef GRADINGSTRATEGY_H
#define GRADINGSTRATEGY_H
 
#include <string>
using namespace std;
 
// Strategy Interface
class GradingStrategy {
public:
    virtual string calculateGrade(int score, int totalMarks) = 0;
    virtual ~GradingStrategy() {}
};
 
// Concrete Strategy: Percentage-based grading
class PercentageGrading : public GradingStrategy {
public:
    string calculateGrade(int score, int totalMarks) override {

        if (totalMarks == 0) return "N/A";

        double percent =
            (double)score / totalMarks * 100.0;

        if (percent >= 93) return "A";
        else if (percent >= 90) return "A-";
        else if (percent >= 87) return "B+";
        else if (percent >= 83) return "B";
        else if (percent >= 80) return "B-";
        else if (percent >= 77) return "C+";
        else if (percent >= 73) return "C";
        else if (percent >= 70) return "C-";
        else if (percent >= 67) return "D+";
        else if (percent >= 60) return "D";
        else return "F";
    }
};
// Concrete Strategy: Pass/Fail grading
class PassFailGrading : public GradingStrategy {
public:
    string calculateGrade(int score, int totalMarks) override {
        if (totalMarks == 0) return "N/A";
        double percent = (double)score / totalMarks * 100.0;
        return (percent >= 50) ? "Pass" : "Fail";
    }
};
 
// Context class
class GradingContext {
private:
    GradingStrategy* strategy;
 
public:
    GradingContext(GradingStrategy* s) : strategy(s) {}
 
    void setStrategy(GradingStrategy* s) {
        strategy = s;
    }
 
    string grade(int score, int totalMarks) {
        return strategy->calculateGrade(score, totalMarks);
    }
};
 
#endif
 