#include "raylib.h"
#include "login.h"

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Login Page");

    Login loginPage; // Create an instance of the Login class

    while (!WindowShouldClose()) {
        loginPage.Update();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        loginPage.Draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}