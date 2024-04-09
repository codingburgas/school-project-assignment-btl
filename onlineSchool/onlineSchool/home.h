#ifndef HOME_H
#define HOME_H

#include "raylib.h"

class Home {
public:
    Home();

    void Update();
    void Draw();
    void HandleInput();

private:
    Rectangle button; // Example button
    Color buttonColor;
    bool buttonHovered;

    void DrawButton(const char* text, Rectangle rect, Color color, Color textColor);
};

#endif
