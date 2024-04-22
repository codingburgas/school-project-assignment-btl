#ifndef EXAM_H
#define EXAM_H

#include "home.h"
#include "raylib.h"
#include "login.h"
#include <vector>
#include <sstream>

class Exam {

public:
    Exam();

    void Update();
    void GetTestQuestions(const string& subject);
    void LoadTestQuestions(const string& subject);
    int StartTest(const string& subject, const string& userEmail);
    void Draw();
    int CalculateGrade(float percentage);
    void WriteGradeToUserFile(const string& subject, int grade, const string& userEmail);
private:

    Rectangle examContainer;
    float questionCount = 20;
    int counter = 0;
    float correctAnswersCount = 0;
    string line, line2;
    int question[20] = {};
    char userAnswers[20] = {};
    char correctAnswers[20] = {};
    ifstream subjectAnswers;
    ifstream subjectQuestions;
};

#endif