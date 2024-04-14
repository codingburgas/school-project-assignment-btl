#ifndef HOME_H
#define HOME_H

#include "raylib.h"
#include "login.h"
#include <vector>

class Home {
public:
    Home(Login& login);

    void Update();
    void Draw();
    void HandleInput();
    void DisplayUserGrades();
    void DisplayUserAbsences();
    void DisplayUserRemarks();

private:
    Rectangle gradesButton;
    Rectangle absencesButton;
    Rectangle remarksButton;
    Rectangle backButton;
    Color buttonColor;
    bool gradesButtonHovered;
    bool absencesButtonHovered;
    bool remarksButtonHovered;
    bool backButtonHovered;
    Login& loginRef;
    bool displayGrades;
    bool displayAbsences;
    bool displayRemarks;
};

#endif
