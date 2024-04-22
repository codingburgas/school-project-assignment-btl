#include "exam.h"
#include "home.h"

Exam::Exam() {
    examContainer = { 250, 125, 800, 400 };
}

void Exam::Draw() {
    DrawText("Test", 80, 45, 90, WHITE);
    DrawText(line.c_str(), 50, 135, 40, WHITE);
    DrawText(line2.c_str(), 50, 175, 40, WHITE);
}

void Exam::Update() {
    BeginDrawing();
    ClearBackground(DARKBLUE);
    Draw();
    EndDrawing();
}

void Exam::LoadTestQuestions(const string& subject) {
    subjectAnswers.open("tests\\" + subject + "_answers.txt");
    subjectQuestions.open("tests\\" + subject + "_questions.txt");
}

void Exam::GetTestQuestions(const string& subject) {
    int randomNumber;
    bool isUnique;
    do {
        randomNumber = rand() % 50 + 1;
        isUnique = true;
        for (int j = 0; j < questionCount; ++j) {
            if (randomNumber == question[j]) {
                isUnique = false;
                break;
            }
        }
    } while (!isUnique);
    question[counter] = randomNumber;
    subjectQuestions.clear();
    subjectQuestions.seekg(0);
    subjectAnswers.clear();
    subjectAnswers.seekg(0);
    for (int lineNum = 0; lineNum < randomNumber; ++lineNum) {
        getline(subjectQuestions, line);
        getline(subjectAnswers, line2);
        if (!subjectQuestions) {
            return;
        }
    }
    correctAnswers[counter] = line2[line2.length() - 1];
}

float Exam::StartTest(const string& subject) {
    cout << "Starting test for subject: " << subject << endl;
    LoadTestQuestions(subject);
    for (int i = 0; i < questionCount; i++) {
        GetTestQuestions(subject);
        Update();
        cin >> userAnswers[i];
        if (userAnswers[i] == correctAnswers[i]) {
            correctAnswersCount++;
        }
    }
    return (correctAnswersCount / questionCount) * 100;
}