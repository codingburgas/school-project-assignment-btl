#ifndef HOME_H
#define HOME_H

#include "raylib.h"
#include "login.h"
#include "vector"
class Home {
public:

    Home(Login& login);

    void Update();
    void Draw();
    void HandleInput();
    void DisplayUserGrades();

private:
    Rectangle button;
    Color buttonColor;
    bool buttonHovered;
    Login& loginRef;
};

#endif
