#include "home.h"
#include "exam.h"
#include <sstream>
Home::Home(Login& login) : loginRef(login) {
    homeButton = { 0, 0, 200, 120 };
    gradesButton = { 0, 120, 200, 200 };
    rankingsButton = { 0, 320, 200, 200 };
    examsButton = { 0, 520, 200, 200 };
    buttonColor = GRAY;
    gradesButtonHovered = false;
    rankingsButtonHovered = false;
    examsButtonHovered = false;
    homeButtonHovered = false;
    displayGrades = false;
    displayRankings = false;
    displayRemarks = false;
    displayTestMenu = false;
    selectedSubject = "";

    //Fonts
    sansSerifBold = LoadFontEx("../fonts/sansSerif/OpenSans_Condensed-Bold.ttf", 35, 0, 0);
    sansSerif = LoadFontEx("../fonts/sansSerif/OpenSans_Condensed-Regular.ttf", 35, 0, 0);
    sansSerifSemiBold = LoadFontEx("../fonts/sansSerif/OpenSans_Condensed-SemiBold.ttf", 35, 0, 0);

    //Text positions
    gradesTextPos = { 50, 135 };
    rankingsTextPos = {30, 335};
    examsTextPos = {40, 535};

    //Font Filters
    SetTextureFilter(sansSerifBold.texture, TEXTURE_FILTER_TRILINEAR);
    SetTextureFilter(sansSerif.texture, TEXTURE_FILTER_TRILINEAR);
    SetTextureFilter(sansSerifSemiBold.texture, TEXTURE_FILTER_TRILINEAR);
}

void Home::Update() {
    HandleInput();
}

void Home::Draw() {
    ClearBackground(RAYWHITE);

    Color sidebarColor = ColorFromNormalized({ 0.18f, 0.46f, 0.71f, 1.0f });

    DrawRectangle(0, 0, 200, GetScreenHeight(), sidebarColor);

    DrawText("BTL", 50, 20, 50, WHITE);

    Vector2 textOffset = { 20, 10 };
    DrawText("Grades", gradesButton.x + gradesButton.width / 2 - MeasureText("Grades", 30) / 2,
        gradesButton.y + gradesButton.height / 2 - 15, 30, BLACK);
    DrawText("Rankings", rankingsButton.x + rankingsButton.width / 2 - MeasureText("Rankings", 30) / 2,
        rankingsButton.y + rankingsButton.height / 2 - 15, 30, BLACK);
    DrawText("Exams", examsButton.x + examsButton.width / 2 - MeasureText("Exams", 30) / 2,
        examsButton.y + examsButton.height / 2 - 15, 30, BLACK);

    // Highlight the hovered button
    if (CheckCollisionPointRec(GetMousePosition(), gradesButton)) {
        DrawRectangleRec(gradesButton, Fade(LIGHTGRAY, 0.6f));
        DrawText("Grades", gradesButton.x + gradesButton.width / 2 - MeasureText("Grades", 30) / 2,
            gradesButton.y + gradesButton.height / 2 - 15, 30, BLACK);
    }
    else if (CheckCollisionPointRec(GetMousePosition(), rankingsButton)) {
        DrawRectangleRec(rankingsButton, Fade(LIGHTGRAY, 0.6f));
        DrawText("Rankings", rankingsButton.x + rankingsButton.width / 2 - MeasureText("Rankings", 30) / 2,
            rankingsButton.y + rankingsButton.height / 2 - 15, 30, BLACK);
    }
    else if (CheckCollisionPointRec(GetMousePosition(), examsButton)) {
        DrawRectangleRec(examsButton, Fade(LIGHTGRAY, 0.6f));
        DrawText("Exams", examsButton.x + examsButton.width / 2 - MeasureText("Exams", 30) / 2,
            examsButton.y + examsButton.height / 2 - 15, 30, BLACK);
    }

    // Draw the content based on the active section
    switch (activeSection) {
    case Section::Grades:
        DrawRectangle(250, 100, 700, 500, LIGHTGRAY);
        DisplayUserGrades();
        break;
    case Section::Rankings:
        DrawRectangle(250, 100, 700, 500, LIGHTGRAY);
        DisplayUserRankings();
        break;
    case Section::Exams:
        DrawTestMenu();
        break;
    default:
        break;
    }
}

void Home::HandleInput() {
    Vector2 mousePos = GetMousePosition();

    // Check for button clicks
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePos, gradesButton)) {
            activeSection = Section::Grades;
        }
        else if (CheckCollisionPointRec(mousePos, rankingsButton)) {
            activeSection = Section::Rankings;
        }
        else if (CheckCollisionPointRec(mousePos, examsButton)) {
            activeSection = Section::Exams;
        }
    }
}

void Home::DrawTestMenu() {
    DrawRectangle(365, 100, 800, 530, SKYBLUE);
    DrawText("Select a Subject", 420, 120, 80, BLACK);

    vector<string> subjects = { "Biology", "Math", "Chemistry", "Geography", "History" };

    Vector2 buttonPosition = { 650, 220 };
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

        DrawText(subject.c_str(), buttonRect.x + 50, buttonRect.y + 8, 20, BLACK);

        if (buttonHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            selectedSubject = subject;
            isTesting = true;
        }

        buttonPosition.y += buttonSpacing;
    }
}

void Home::StartTest(const string& subject) {
    cout << "Starting test for subject: " << subject << endl;
}

float Home::CalculateAverageGrade(const string& email) {
    vector<string> gradeLines = loginRef.GetGrades(email);
    if (gradeLines.empty()) {
        return 0.0f;
    }

    float total = 0.0f;
    int count = 0;

    for (const auto& line : gradeLines) {
        size_t separatorPos = line.find(':');
        if (separatorPos != string::npos && separatorPos + 1 < line.length()) {
            string gradesStr = line.substr(separatorPos + 1);
            stringstream ss(gradesStr);
            string grade;
            while (getline(ss, grade, ',')) {
                try {
                    float individualGrade = stof(grade);
                    total += individualGrade;
                    count++;
                }
                catch (...) {
                    // Handle error when there are no grades available
                    cerr << "Error parsing grade: " << grade << endl;
                }
            }
        }
    }
    if (count > 0) {
        return total / count;
    }
    else {
        return 0.0f;
    }
}

string Home::FindUserWithHighestAverage() {
    vector<string> allUsers = loginRef.GetAllUserEmails();
    string highestUser = "";
    float highestAverage = 0.0f;
    for (const auto& user : allUsers) {
        float average = CalculateAverageGrade(user);
        if (average > highestAverage) {
            highestAverage = average;
            highestUser = user;
        }
    }
    return highestUser;
}

string Home::FindUserWithLowestAverage() {
    vector<string> allUsers = loginRef.GetAllUserEmails();
    string lowestUser = "";
    float lowestAverage = numeric_limits<float>::max();
    for (const auto& user : allUsers) {
        float average = CalculateAverageGrade(user);
        if (average < lowestAverage) {
            lowestAverage = average;
            lowestUser = user;
        }
    }
    return lowestUser;
}

int Home::FindUserRanking(const string& email) {
    vector<string> allUsers = loginRef.GetAllUserEmails();
    vector<float> allAverages;
    for (const auto& user : allUsers) {
        allAverages.push_back(CalculateAverageGrade(user));
    }
    sort(allAverages.begin(), allAverages.end(), greater<float>());
    auto it = find(allAverages.begin(), allAverages.end(), CalculateAverageGrade(email));
    if (it != allAverages.end()) {
        return distance(allAverages.begin(), it) + 1;
    }
    else {
        return -1;
    }
}

void Home::DisplayUserGrades() {
    string email = loginRef.GetLoggedInUserEmail();
    vector<string> gradeLines = loginRef.GetGrades(email);

    if (!gradeLines.empty()) {
        float averageGrade = CalculateAverageGrade(email);
        int yOffset = 200;

        DrawText("Your Grades", 400, 150, 30, BLACK);
        DrawLine(400, 180, 800, 180, BLACK);

        for (const auto& line : gradeLines) {
            // Split the line into subject and grade
            size_t separatorPos = line.find(':');
            if (separatorPos != string::npos && separatorPos + 1 < line.length()) {
                string subject = line.substr(0, separatorPos);
                string grade = line.substr(separatorPos + 1);
                DrawText(subject.c_str(), 400, yOffset, 20, BLACK);
                DrawText(grade.c_str(), 600, yOffset, 20, BLACK);
                yOffset += 40;
            }
        }

        string averageText = "Average Grade: " + formatFloat(averageGrade);
        DrawText(averageText.c_str(), 400, yOffset, 20, BLACK);
    }
    else {
        DrawText("No grades available", 400, 200, 20, BLACK);
    }
}


void Home::DisplayUserRemarks() {
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

void Home::DisplayUserRankings() {
    string email = loginRef.GetLoggedInUserEmail();
    int ranking = FindUserRanking(email);
    string highestUser = FindUserWithHighestAverage();
    float highestAverage = 0.0f;
    if (!highestUser.empty()) {
        highestAverage = CalculateAverageGrade(highestUser);
    }
    string lowestUser = FindUserWithLowestAverage();
    float lowestAverage = 0.0f;
    if (!lowestUser.empty()) {
        lowestAverage = CalculateAverageGrade(lowestUser);
    }

    DrawRectangle(400, 190, 400, 120, RAYWHITE);
    DrawText("User Rankings", 400, 150, 30, BLACK);
    DrawLine(400, 180, 800, 180, BLACK);

    string rankingText = "Your Ranking: " + to_string(ranking);
    DrawText(rankingText.c_str(), 400, 200, 20, BLACK);

    string highestText = "Highest: " + highestUser + " (Average Grade: " + formatFloat(highestAverage) + ")";
    DrawText(highestText.c_str(), 400, 230, 20, BLACK);

    string lowestText = "Lowest: " + lowestUser + " (Average Grade: " + formatFloat(lowestAverage) + ")";
    DrawText(lowestText.c_str(), 400, 260, 20, BLACK);

    DrawText(" ", 400, 290, 20, BLACK);
    DrawText(" ", 400, 320, 20, BLACK);
}

string Home::formatFloat(float value) {
    string result = to_string(value);
    size_t decimalPos = result.find('.');
    if (decimalPos != string::npos && result.length() > decimalPos + 3) {
        result = result.substr(0, decimalPos + 3);
    }

    return result;
}