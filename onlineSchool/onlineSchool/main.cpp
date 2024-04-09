#include "raylib.h"
#include "login.h"
#include "home.h"

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Login Page");

    Login loginPage; // Create an instance of the Login class
    Home homeScreen; // Create an instance of the Home class

    while (!WindowShouldClose()) {
        if (!loginPage.IsLoggedIn()) {
            loginPage.Update();
            BeginDrawing();
            ClearBackground(RAYWHITE);
            loginPage.Draw();
            EndDrawing();
        }
        else {
            homeScreen.Update();
            BeginDrawing();
            ClearBackground(RAYWHITE);
            homeScreen.Draw();
            EndDrawing();
        }
    }

    CloseWindow();

    return 0;
}
