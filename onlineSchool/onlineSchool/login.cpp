#include "login.h"

Login::Login() {
    // Initialize UI elements
    emailBox = { 400, 250, 480, 50 };
    passwordBox = { 400, 320, 480, 50 };
    loginButton = { 400, 400, 200, 50 };
    registerButton = { 680, 400, 200, 50 };

    email = "";
    password = "";
    loginClicked = false;
    registerClicked = false;
    isPasswordHidden = true;
    isLoggedIn = false;

    // Initialize outline colors
    emailBoxOutlineColor = BLACK;
    passwordBoxOutlineColor = BLACK;
}

void Login::Update() {
    HandleInput();
}

void Login::Draw() {
    // Draw UI elements
    DrawText("Login", 520, 150, 40, BLACK);

    // Email box
    DrawRectangleLines(emailBox.x, emailBox.y, emailBox.width, emailBox.height, emailBoxOutlineColor);
    DrawText(email.c_str(), emailBox.x + 10, emailBox.y + 15, 20, BLACK);

    // Password box
    DrawRectangleLines(passwordBox.x, passwordBox.y, passwordBox.width, passwordBox.height, passwordBoxOutlineColor);
    if (isPasswordHidden) {
        string hiddenPassword(password.length(), '*');
        DrawText(hiddenPassword.c_str(), passwordBox.x + 10, passwordBox.y + 15, 20, BLACK);
    }
    else {
        DrawText(password.c_str(), passwordBox.x + 10, passwordBox.y + 15, 20, BLACK);
    }

    // Login button
    DrawRectangleLines(loginButton.x, loginButton.y, loginButton.width, loginButton.height, loginButtonHovered ? GREEN : BLACK);
    DrawText("Login", loginButton.x + 40, loginButton.y + 15, 20, BLACK);

    // Register button
    DrawRectangleLines(registerButton.x, registerButton.y, registerButton.width, registerButton.height, registerButtonHovered ? GREEN : BLACK);
    DrawText("Register", registerButton.x + 20, registerButton.y + 15, 20, BLACK);
}

void Login::HandleInput() {
    int key = GetKeyPressed();

    // Handle Caps Lock toggle
    static bool capsLockPrevState = false;
    bool capsLockCurrState = IsKeyDown(KEY_CAPS_LOCK);
    if (capsLockCurrState && !capsLockPrevState) {
        capsLockEnabled = !capsLockEnabled;
    }
    capsLockPrevState = capsLockCurrState;

    // Handle keyboard input
    if (key != 0) {
        if (key == KEY_BACKSPACE) {
            if (!email.empty() && emailBoxHovered) {
                email.pop_back();
            }
            else if (!password.empty() && passwordBoxHovered) {
                password.pop_back();
            }
        }
        else if ((key >= 32 && key <= 125) && (email.length() < MAX_EMAIL_LENGTH || password.length() < MAX_PASSWORD_LENGTH)) {
            // Convert to uppercase if Caps Lock is active or shift is pressed
            if ((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT) || capsLockEnabled) && key >= 'a' && key <= 'z') {
                key -= 32; // Convert to uppercase
            }

            if (emailBoxHovered && email.length() < MAX_EMAIL_LENGTH) {
                email += static_cast<char>(key);
            }
            else if (passwordBoxHovered && password.length() < MAX_PASSWORD_LENGTH) {
                password += static_cast<char>(key);
            }
        }
    }

    // Handle mouse input
    Vector2 mousePos = GetMousePosition();
    emailBoxHovered = CheckCollisionPointRec(mousePos, emailBox);
    passwordBoxHovered = CheckCollisionPointRec(mousePos, passwordBox);
    loginButtonHovered = CheckCollisionPointRec(mousePos, loginButton);
    registerButtonHovered = CheckCollisionPointRec(mousePos, registerButton);

    // Set active text box outline color
    emailBoxOutlineColor = emailBoxHovered ? GREEN : BLACK;
    passwordBoxOutlineColor = passwordBoxHovered ? GREEN : BLACK;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (loginButtonHovered) {
            loginClicked = true;
            // Check if the email exists and if the password matches
            if (CheckLogin(email, password)) {
                // Successful login
                isLoggedIn = true;
            }
            else {
                // Login failed
                DrawText("Invalid Email or Password!", 400, 500, 30, RED);
            }
        }
        if (registerButtonHovered) {
            registerClicked = true;
            // Check if the email already exists
            if (CheckExistingEmail(email)) {
                // Email already exists
                DrawText("Email Already Exists!", 400, 500, 30, RED);
            }
            else {
                // Create new account
                RegisterNewAccount(email, password);
                DrawText("Account Registered Successfully!", 400, 500, 30, GREEN);
            }
        }
    }
}

bool Login::CheckLogin(const string& email, const string& password) {
    ifstream loginFile("login_info.txt");
    if (loginFile.is_open()) {
        string line;
        while (getline(loginFile, line)) {
            if (line.find("Email: " + email) != string::npos) {
                getline(loginFile, line); // Read the corresponding password line
                if (line.find("Password: " + password) != string::npos) {
                    cout << "Successful login!" << endl;
                    loginFile.close();
                    return true;
                }
                else {
                    cout << "Incorrect password!" << endl;
                    loginFile.close();
                    return false;
                }
            }
        }
        cout << "Account doesn't exist!" << endl;
        loginFile.close();
        return false;
    }
    else {
        cout << "Error: Unable to open login_info.txt!" << endl;
        return false;
    }
}

bool Login::CheckExistingEmail(const string& email) {
    ifstream loginFile("login_info.txt");
    if (loginFile.is_open()) {
        string line;
        while (getline(loginFile, line)) {
            if (line.find("Email: " + email) != string::npos) {
                cout << "Username is already taken!" << endl;
                loginFile.close();
                return true;
            }
        }
        loginFile.close();
        return false;
    }
    else {
        cout << "Error: Unable to open login_info.txt!" << endl;
        return true;
    }
}

void Login::RegisterNewAccount(const string& email, const string& password) {
    if (!CheckExistingEmail(email)) {
        ofstream loginFile("login_info.txt", ios_base::app);
        if (loginFile.is_open()) {
            loginFile << "Email: " << email << endl;
            loginFile << "Password: " << password << endl;
            cout << "Account registered successfully!" << endl;
            loginFile.close();
        }
        else {
            cout << "Error: Unable to open login_info.txt for writing!" << endl;
        }
    }
}

bool Login::IsLoggedIn() const {
    return isLoggedIn;
}
