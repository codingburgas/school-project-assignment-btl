#ifndef HOME_H
#define HOME_H

#include "raylib.h"
#include "login.h"
#include "exam.h"
#include <vector>



class Home {
public:
    Home(Login& login);
    Home() = default;

    void Update();
    void Draw();
    void HandleInput();
    void DisplayUserGrades();
    void DisplayUserRankings();
    void DisplayUserRemarks();
    void DrawTestMenu();
    bool isTesting = false;
    static string formatFloat(float value);
    string selectedSubject;
private:
    enum class Section {
        Grades,
        Rankings,
        Exams,
        TestMenu
    };

    Section activeSection = Section::Grades;
    float questionCount = 20;
    int counter = 0;
    float correctAnswersCount = 0;
    Font sansSerifBold;
    Font sansSerif;
    Font sansSerifSemiBold;
    Vector2 gradesTextPos;
    Vector2 rankingsTextPos;
    Vector2 examsTextPos;
    Rectangle gradesButton;
    Rectangle rankingsButton;
    Rectangle examsButton;
    Rectangle backButton;
    Rectangle homeButton;
    Color buttonColor;
    bool gradesButtonHovered;
    bool rankingsButtonHovered;
    bool examsButtonHovered;
    bool backButtonHovered;
    bool homeButtonHovered;
    Login& loginRef;
    bool displayGrades;
    bool displayRankings;
    bool displayRemarks;
    bool displayTestMenu;
    float CalculateAverageGrade(const string& username);
    string FindUserWithHighestAverage();
    string FindUserWithLowestAverage();
    int FindUserRanking(const string& email);
};

#endif