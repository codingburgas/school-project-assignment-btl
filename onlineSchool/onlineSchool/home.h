#ifndef HOME_H
#define HOME_H

#include "raylib.h"
#include "login.h"
#include <vector>
#include <numeric>
class Home {
public:
    Home(Login& login);

    void Update();
    void Draw();
    void HandleInput();
    void DisplayUserGrades();
    void DisplayUserAbsences();
    void DisplayUserRemarks();
    void DrawTestMenu();
    void HandleTestMenuInput(Vector2 mousePos);
    void StartTest(const string& subject);

private:
    Font sansSerifBold;
    Font sansSerif;
    Font sansSerifSemiBold;
    Vector2 gradesTextPos;
    Vector2 absencesTextPos;
    Vector2 examsTextPos;
    Rectangle gradesButton;
    Rectangle absencesButton;
    Rectangle examsButton;
    Rectangle backButton;
    Rectangle homeButton;
    Color buttonColor;
    bool gradesButtonHovered;
    bool absencesButtonHovered;
    bool examsButtonHovered;
    bool backButtonHovered;
    bool homeButtonHovered;
    Login& loginRef;
    bool displayGrades;
    bool displayAbsences;
    bool displayRemarks;
    bool displayTestMenu;
    string selectedSubject;
    float CalculateAverageGrade(const std::string& username);
    string FindUserWithHighestAverage();
    string FindUserWithLowestAverage();
    int FindUserRanking(const string& email);
};

#endif