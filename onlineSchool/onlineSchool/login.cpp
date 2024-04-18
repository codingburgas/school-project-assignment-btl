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
    emailBoxClicked = false;
    passwordBoxClicked = false;
    loginClicked = false;
    registerClicked = false;
    isPasswordHidden = true;
    isLoggedIn = false;

    // Initialize outline colors
    emailBoxOutlineColor = BLACK;
    passwordBoxOutlineColor = BLACK;

    //Fonts
    sansSerifBold = LoadFontEx("../fonts/sansSerif/OpenSans_Condensed-Bold.ttf", 35, 0, 0);
    sansSerif= LoadFontEx("../fonts/sansSerif/OpenSans_Condensed-Regular.ttf", 35, 0, 0);
    sansSerifSemiBold= LoadFontEx("../fonts/sansSerif/OpenSans_Condensed-SemiBold.ttf", 35, 0, 0);

    //Text positions
    loginButtonTextPos = { 470, 405 };
    registerButtonTextPos = { 740, 405 };
    emailBoxTextPos = { 405, 257 };
    passwordBoxTextPos = { 405, 327 };

    //Font Filters
    SetTextureFilter(sansSerifBold.texture, TEXTURE_FILTER_TRILINEAR);
    SetTextureFilter(sansSerif.texture, TEXTURE_FILTER_TRILINEAR);
    SetTextureFilter(sansSerifSemiBold.texture, TEXTURE_FILTER_TRILINEAR);
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
    DrawTextEx(sansSerif, email.c_str(), emailBoxTextPos, 35, 0, BLACK);
    if (!emailBoxClicked&&email.empty()) {
        DrawTextEx(sansSerif, "Email/username", emailBoxTextPos, 35, 0, DARKGRAY);
    }

    // Password box
    DrawRectangleLines(passwordBox.x, passwordBox.y, passwordBox.width, passwordBox.height, passwordBoxOutlineColor);
   
    if (!passwordBoxClicked&&password.empty()) {
        DrawTextEx(sansSerif, "Password", passwordBoxTextPos, 35, 0, DARKGRAY);
    }
    if (isPasswordHidden) {
        string hiddenPassword(password.length(), '*');
        DrawTextEx(sansSerif, hiddenPassword.c_str(), passwordBoxTextPos, 35, 0, BLACK);
    }
    else {
        DrawTextEx(sansSerif, password.c_str(), passwordBoxTextPos, 35, 0, BLACK);
    }

    // Login button
    DrawRectangle(loginButton.x, loginButton.y, loginButton.width, loginButton.height, loginButtonHovered ? BLUE:DARKBLUE);
    DrawTextEx(sansSerifBold, "Login", loginButtonTextPos, 35, 0, WHITE);
    if (!loginButtonHovered) {
        
    }

    // Register button
    DrawRectangle(registerButton.x, registerButton.y, registerButton.width, registerButton.height, registerButtonHovered ? BLUE:DARKBLUE);
        DrawTextEx(sansSerifBold, "Register", registerButtonTextPos, 35, 0, WHITE);
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
            if (!email.empty() && emailBoxClicked) {
                email.pop_back();
            }
            else if (!password.empty() && passwordBoxClicked) {
                password.pop_back();
            }
        }
        else if ((key >= 32 && key <= 125) && (email.length() < MAX_EMAIL_LENGTH || password.length() < MAX_PASSWORD_LENGTH)) {
            if ((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT) || capsLockEnabled) && key >= 'a' && key <= 'z') {
                key -= 32;
            }

            if (emailBoxClicked && email.length() < MAX_EMAIL_LENGTH) {
                email += static_cast<char>(key);
            }
            else if (passwordBoxClicked && password.length() < MAX_PASSWORD_LENGTH) {
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
    if (emailBoxHovered) {
        emailBoxOutlineColor = DARKBLUE;
    }
    else {
        emailBoxOutlineColor = BLACK;
    }

    if (passwordBoxHovered) {
        passwordBoxOutlineColor = DARKBLUE;
    }
    else {
        passwordBoxOutlineColor = BLACK;
    }

    //make it able to diselect a box by clicking outside of it
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (!emailBoxHovered && !passwordBoxHovered) {
            emailBoxClicked = false;
            passwordBoxClicked = false;
        }
    }

    //Check if the eamil box has been clicked on
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (emailBoxHovered) {
            emailBoxClicked = true;
            passwordBoxClicked = false;
        }
    }

    //Check if the password box has been clicked on
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (passwordBoxHovered) {
            emailBoxClicked = false;
            passwordBoxClicked = true;
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (loginButtonHovered) {
            loginClicked = true;
            // Check if the email exists and if the password matches
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