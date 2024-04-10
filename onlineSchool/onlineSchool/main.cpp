#include "raylib.h"
#include "login.h"
#include "home.h"

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Login Page");

    Login loginPage;
    Home homeScreen(loginPage);

    while (!WindowShouldClose()) {
        if (!loginPage.IsLoggedIn()) {
            loginPage.Update();
            BeginDrawing();
            ClearBackground(DARKBLUE);
            loginPage.Draw();
            EndDrawing();
        }
        else {
            homeScreen.Update();
            BeginDrawing();
            ClearBackground(DARKBLUE);
            homeScreen.Draw();
            EndDrawing();
        }
    }

    CloseWindow();

    return 0;
}