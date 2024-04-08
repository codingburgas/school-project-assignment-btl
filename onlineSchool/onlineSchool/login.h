#ifndef LOGIN_H
#define LOGIN_H

#include <raylib.h>
#include <string>

using namespace std;

#define MAX_EMAIL_LENGTH 30
#define MAX_PASSWORD_LENGTH 20

class Login {
public:
    Login();

    void Update();
    void Draw();

    string GetEmail() const { return email; }
    string GetPassword() const { return password; }
    bool IsLoginClicked() const { return loginClicked; }
    bool IsRegisterClicked() const { return registerClicked; }
    bool IsEmailBoxHovered() const { return emailBoxHovered; }
    bool IsPasswordBoxHovered() const { return passwordBoxHovered; }
    bool IsLoginButtonHovered() const { return loginButtonHovered; }
    bool IsRegisterButtonHovered() const { return registerButtonHovered; }

private:
    Rectangle emailBox;
    Rectangle passwordBox;
    Rectangle loginButton;
    Rectangle registerButton;

    string email;
    string password;

    bool loginClicked;
    bool registerClicked;
    bool isPasswordHidden;

    bool emailBoxHovered;
    bool passwordBoxHovered;
    bool loginButtonHovered;
    bool registerButtonHovered;

    Color emailBoxOutlineColor;
    Color passwordBoxOutlineColor;

    bool capsLockEnabled;

    void HandleInput();
};

#endif // LOGIN_H