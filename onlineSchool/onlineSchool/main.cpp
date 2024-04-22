#include "raylib.h"
#include "login.h"
#include "home.h"
#include "exam.h"

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Login Page");

    Login loginPage;
    Exam exam;
    Home homeScreen(loginPage);

    while (!WindowShouldClose()) {
        if (!loginPage.IsLoggedIn() && homeScreen.isTesting == false) {
            loginPage.Update();
            BeginDrawing();
            ClearBackground(DARKBLUE);
            loginPage.Draw();
            EndDrawing();
        }
        if (loginPage.IsLoggedIn() && homeScreen.isTesting == false) {
            homeScreen.Update();
            BeginDrawing();
            ClearBackground(DARKBLUE);
            homeScreen.Draw();
            EndDrawing();
        }
        if (homeScreen.isTesting) {
            BeginDrawing();
            ClearBackground(DARKBLUE);
            exam.Update();
            float score = exam.StartTest(homeScreen.selectedSubject, loginPage.GetLoggedInUserEmail());
            EndDrawing();
            homeScreen.isTesting = false;
        }
    }
    CloseWindow();
    return 0;
}