#include "home.h"

Home::Home(Login& login) : loginRef(login) {
    homeButton = { 0, 0, 200, 120 };
    gradesButton = { 0, 120, 200, 200 };
    absencesButton = { 0, 320, 200, 200 };
    remarksButton = { 0, 520, 200, 200 };
    buttonColor = GRAY;
    gradesButtonHovered = false;
    absencesButtonHovered = false;
    remarksButtonHovered = false;
    homeButtonHovered = false;
    displayGrades = false;
    displayAbsences = false;
    displayRemarks = false;
    displayTestMenu = false;
    selectedSubject = "";
}

void Home::Update() {
    HandleInput();
}

void Home::Draw() {
    ClearBackground(RAYWHITE);
    DrawRectangleRec(homeButton, homeButtonHovered ? WHITE : LIME);
    DrawText("Home", homeButton.x + 35, homeButton.y + 23, 50, WHITE);
    if (!displayGrades && !displayAbsences && !displayRemarks) {
        DrawRectangleRec(gradesButton, gradesButtonHovered ? SKYBLUE : BLUE);
        DrawText("Grades", gradesButton.x + 50, gradesButton.y + 15, 20, WHITE);

        DrawRectangleRec(absencesButton, absencesButtonHovered ? SKYBLUE : BLUE);
        DrawText("Absences", absencesButton.x + 30, absencesButton.y + 15, 20, WHITE);

        DrawRectangleRec(remarksButton, remarksButtonHovered ? SKYBLUE : BLUE);
        DrawText("Exams", remarksButton.x + 40, remarksButton.y + 15, 20, WHITE);
    }

    if (displayTestMenu) {
        DrawTestMenu();
    }

    if (displayGrades) {
        DisplayUserGrades();
    }

    if (displayAbsences) {
        DisplayUserAbsences();
    }

    if (displayRemarks) {
        DisplayUserRemarks();
    }
}



void Home::HandleInput() {
    Vector2 mousePos = GetMousePosition();
    gradesButtonHovered = CheckCollisionPointRec(mousePos, gradesButton);
    absencesButtonHovered = CheckCollisionPointRec(mousePos, absencesButton);
    remarksButtonHovered = CheckCollisionPointRec(mousePos, remarksButton);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (gradesButtonHovered) {
            displayGrades = !displayGrades;
        }
        if (absencesButtonHovered) {
            displayAbsences = !displayAbsences;
        }
        if (remarksButtonHovered) {
            displayRemarks = !displayRemarks;
            displayTestMenu = true;
        }
    }

    if (displayTestMenu) {
        HandleTestMenuInput(mousePos);
    }
}

void Home::DrawTestMenu() {
    DrawRectangle(300, 370, 400, 230, GRAY);
    DrawText("Select a Subject", 350, 380, 20, BLACK);

    vector<string> subjects = { "Biology", "Math", "Chemistry", "Geography", "History" };

    Vector2 buttonPosition = { 320, 420 };
    float buttonSpacing = 40.0f;

    for (const auto& subject : subjects) {
        Rectangle buttonRect = { buttonPosition.x, buttonPosition.y, 200, 30 };
        DrawRectangleRec(buttonRect, WHITE);

        bool buttonHovered = CheckCollisionPointRec(GetMousePosition(), buttonRect);

        if (buttonHovered) {
            DrawRectangleRec(buttonRect, Fade(GRAY, 0.6f));
            buttonRect.width *= 1.1f;
            buttonRect.height *= 1.1f;
            buttonRect.x -= (buttonRect.width - 200) / 2;
            buttonRect.y -= (buttonRect.height - 30) / 2;
        }

        DrawText(subject.c_str(), buttonRect.x + 10, buttonRect.y + 8, 20, BLACK);

        if (buttonHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            selectedSubject = subject;
            StartTest(subject);
        }

        buttonPosition.y += buttonSpacing;
    }
}

void Home::StartTest(const string& subject) {
    cout << "Starting test for subject: " << subject << endl;
}

void Home::HandleTestMenuInput(Vector2 mousePos) {

}

void Home::DisplayUserGrades() {
    // Retrieve the logged-in user's email
    string email = loginRef.GetLoggedInUserEmail();
    vector<string> grades = loginRef.GetGrades(email);
    if (!grades.empty()) {
        int yOffset = 200;
        for (const auto& grade : grades) {
            DrawText(grade.c_str(), 40, yOffset, 20, BLACK);
            yOffset += 30;
        }
    }
    else {
        DrawText("No grades available", 400, 200, 20, BLACK);
    }
}

void Home::DisplayUserRemarks() {
    // Retrieve the logged-in user's email
    string email = loginRef.GetLoggedInUserEmail();
    string remarksFileName = email + "_remarks.txt";
    ifstream remarksFile(remarksFileName);

    if (remarksFile.is_open()) {
        string line;
        int yOffset = 200;
        while (getline(remarksFile, line)) {
            DrawText(line.c_str(), 400, yOffset, 20, BLACK);
            yOffset += 30;
        }
        remarksFile.close();
    }
    else {
        DrawText("No remarks available", 400, 200, 20, BLACK);
    }
}

void Home::DisplayUserAbsences() {
    // Retrieve the logged-in user's email
    string email = loginRef.GetLoggedInUserEmail();
    string absencesFileName = email + "_absences.txt";
    ifstream absencesFile(absencesFileName);

    if (absencesFile.is_open()) {
        string line;
        int yOffset = 200;
        while (getline(absencesFile, line)) {
            DrawText(line.c_str(), 400, yOffset, 20, BLACK);
            yOffset += 30;
        }
        absencesFile.close();
    }
    else {
        DrawText("No absences recorded", 400, 200, 20, BLACK);
    }
}
