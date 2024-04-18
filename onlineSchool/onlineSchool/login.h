#ifndef LOGIN_H
#define LOGIN_H

#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "raylib.h"

using namespace std;

class Login {
public:
    Login();

    void Update();
    void Draw();
    void HandleInput();
    bool IsLoggedIn() const;

    string GetLoggedInUserEmail() const;
    vector<string> GetGrades(const string& email);

private:
    const int MAX_EMAIL_LENGTH = 45;
    const int MAX_PASSWORD_LENGTH = 20;

    Rectangle line;
    Rectangle outerBox;
    Rectangle emailBox;
    Rectangle passwordBox;
    Rectangle loginButton;
    Rectangle registerButton;

    string email;
    string password;
    bool loginClicked;
    bool registerClicked;
    bool isPasswordHidden;
    bool capsLockEnabled;
    bool emailBoxHovered;
    bool emailBoxClicked;
    bool passwordBoxHovered;
    bool passwordBoxClicked;
    bool loginButtonHovered;
    bool registerButtonHovered;

    Font sansSerifBold;
    Font sansSerif;
    Font sansSerifSemiBold;
    Vector2 loginButtonTextPos;
    Vector2 registerButtonTextPos;
    Vector2 emailBoxTextPos;
    Vector2 passwordBoxTextPos;
    Color emailBoxOutlineColor;
    Color passwordBoxOutlineColor;
    string loggedInUserEmail;
    bool CheckLogin(const string& email, const string& password);
    bool CheckExistingEmail(const string& email);
    void RegisterNewAccount(const string& email, const string& password);

    bool isLoggedIn;
};

#endif