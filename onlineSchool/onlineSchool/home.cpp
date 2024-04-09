#include "home.h"

Home::Home(Login& login) : loginRef(login) {
    button = { 400, 300, 200, 50 };
    buttonColor = GRAY;
    buttonHovered = false;
}

void Home::Update() {
    HandleInput();
}

void Home::Draw() {
    ClearBackground(RAYWHITE);
    DrawText("Home", 400, 500, 30, RED);
    DisplayUserGrades();
}

void Home::HandleInput() {
    Vector2 mousePos = GetMousePosition();
    buttonHovered = CheckCollisionPointRec(mousePos, button);
}

void Home::DisplayUserGrades() {
    // Retrieve the logged-in user's email
    string email = loginRef.GetLoggedInUserEmail();
    vector<string> grades = loginRef.GetGrades(email);
    if (!grades.empty()) {
        int yOffset = 100;
        for (const auto& grade : grades) {
            DrawText(grade.c_str(), 400, yOffset, 20, BLACK);
            yOffset += 30;
        }
    }
    else {
        DrawText("No grades available", 400, 100, 20, BLACK);
    }
}
