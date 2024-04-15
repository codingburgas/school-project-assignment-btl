#include "login.h"

Login::Login() {
    // Initialize UI elements
    outerBox = {250, 125, 800, 400};
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
    //Outer Box
    DrawRectangle(outerBox.x, outerBox.y, outerBox.width, outerBox.height, WHITE);

    // Draw UI elements
    DrawText("Login", 585, 150, 40, BLACK);

    // Email box
    DrawRectangleLines(emailBox.x, emailBox.y, emailBox.width, emailBox.height, emailBoxOutlineColor);
    DrawText(email.c_str(), emailBox.x + 10, emailBox.y + 15, 20, BLACK);
    if (!emailBoxHovered&&email.empty()) {
        DrawText("Email/username", 420, 267, 20, BLACK);
    }

    // Password box
    DrawRectangleLines(passwordBox.x, passwordBox.y, passwordBox.width, passwordBox.height, passwordBoxOutlineColor);
   
    if (!passwordBoxHovered&&password.empty()) {
        DrawText("Password", 420, 335, 20, BLACK);
    }
    if (isPasswordHidden) {
        string hiddenPassword(password.length(), '*');
        DrawText(hiddenPassword.c_str(), passwordBox.x + 10, passwordBox.y + 15, 20, BLACK);
    }
    else {
        DrawText(password.c_str(), passwordBox.x + 10, passwordBox.y + 15, 20, BLACK);
    }

    // Login button
    DrawRectangle(loginButton.x, loginButton.y, loginButton.width, loginButton.height, loginButtonHovered ? BLUE:DARKBLUE);
    DrawText("Login", loginButton.x + 40, loginButton.y + 15, 20,  WHITE);
    if (!loginButtonHovered) {
        
    }

    // Register button
    DrawRectangle(registerButton.x, registerButton.y, registerButton.width, registerButton.height, registerButtonHovered ? BLUE:DARKBLUE);
        DrawText("Register", registerButton.x + 20, registerButton.y + 15, 20, WHITE);
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
            if ((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT) || capsLockEnabled) && key >= 'a' && key <= 'z') {
                key -= 32;
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
    emailBoxOutlineColor =  BLACK;
    passwordBoxOutlineColor =  BLACK;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (loginButtonHovered) {
            loginClicked = true;
            if (CheckLogin(email, password)) {
                isLoggedIn = true;
                loggedInUserEmail = email;
            }
            else {
                // Login failed
                DrawText("Invalid Email or Password!", 400, 500, 30, RED);
            }


        }
        if (registerButtonHovered) {
            registerClicked = true;
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
    ifstream loginFile("users/login_info.txt");
    if (loginFile.is_open()) {
        string line;
        while (getline(loginFile, line)) {
            if (line.find("Email: " + email) != string::npos) {
                getline(loginFile, line);
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
    ifstream loginFile("users/login_info.txt");
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
        string usersFolder = "users";

        if (!filesystem::exists(usersFolder)) {
            if (!filesystem::create_directory(usersFolder)) {
                cout << "Error: Unable to create users folder!" << endl;
                return;
            }
        }


        ofstream loginFile("users/login_info.txt", ios_base::app);
        if (loginFile.is_open()) {
            loginFile << "Email: " << email << endl;
            loginFile << "Password: " << password << endl;
            cout << "Account registered successfully!" << endl;
            loginFile.close();
        }
        else {
            cout << "Error: Unable to open login_info.txt for writing!" << endl;
            return;
        }

        string folderName = "users/" + email;
        if (!filesystem::create_directory(folderName)) {
            cout << "Error: Unable to create folder for account: " << email << endl;
            return;
        }
        string gradesFileName = folderName + "/grades.txt";
        ofstream gradesFile(gradesFileName);
        if (gradesFile.is_open()) {
            gradesFile << "Grades for account: " << email << endl;
            gradesFile << "----------------------------------------" << endl;
            cout << "Grades file created successfully for account: " << email << endl;
            gradesFile.close();
        }
        else {
            cout << "Error: Unable to create grades file for account: " << email << endl;
        }


        string absencesFileName = folderName + "/absences.txt";
        ofstream absencesFile(absencesFileName);
        if (absencesFile.is_open()) {
            absencesFile << "Absences for account: " << email << endl;
            absencesFile << "----------------------------------------" << endl;

            cout << "Absences file created successfully for account: " << email << endl;
            absencesFile.close();
        }
        else {
            cout << "Error: Unable to create absences file for account: " << email << endl;
        }

        string remarksFileName = folderName + "/remarks.txt";
        ofstream remarksFile(remarksFileName);
        if (remarksFile.is_open()) {
            remarksFile << "Remarks for account: " << email << endl;
            remarksFile << "----------------------------------------" << endl;

            cout << "Remarks file created successfully for account: " << email << endl;
            remarksFile.close();
        }
        else {
            cout << "Error: Unable to create remarks file for account: " << email << endl;
        }
    }
}

vector<string> Login::GetGrades(const string& email) {
    vector<string> grades;
    string fileName = "users/" + email + "/grades.txt";
    ifstream gradesFile(fileName);

    if (gradesFile.is_open()) {
        string line;
        while (getline(gradesFile, line)) {
            grades.push_back(line);
        }
        gradesFile.close();
    }
    else {
        cout << "Error: Unable to open grades file for account: " << email << endl;
    }

    return grades;
}

string Login::GetLoggedInUserEmail() const {
    return loggedInUserEmail;
}

bool Login::IsLoggedIn() const {
    return isLoggedIn;
}