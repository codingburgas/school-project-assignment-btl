#include "home.h"

Home::Home() {
    // Initialize button properties
    button = { 400, 300, 200, 50 };
    buttonColor = GRAY;
    buttonHovered = false;
}

void Home::Update() {
    HandleInput();
}

void Home::Draw() {
    // Draw your home screen elements here
    ClearBackground(RAYWHITE);
    DrawText("home", 400, 500, 30, RED);
}

void Home::HandleInput() {
    // Handle input for your home screen
    Vector2 mousePos = GetMousePosition();
    buttonHovered = CheckCollisionPointRec(mousePos, button);
}