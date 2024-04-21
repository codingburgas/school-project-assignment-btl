#include "home.h"
#include "exam.h"

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
    DrawRectangleRec(homeButton, homeButtonHovered ? WHITE : LIME);
    DrawText("Home", homeButton.x + 35, homeButton.y + 23, 50, WHITE);
    if (!displayGrades && !displayRankings && !displayRemarks) {
        DrawRectangleRec(gradesButton, gradesButtonHovered ? DARKBLUE:BLUE);
        DrawTextEx(sansSerifBold, "Grades", gradesTextPos, 35, 0, WHITE);

        DrawRectangleRec(rankingsButton, rankingsButtonHovered ? MAROON:RED);
        DrawTextEx(sansSerifBold, "Rankings", rankingsTextPos, 35, 0, WHITE);

        DrawRectangleRec(examsButton, examsButtonHovered ? ORANGE:GOLD);
        DrawTextEx(sansSerifBold, "Exams", examsTextPos, 35, 0, WHITE);
    }

    if (displayTestMenu) {
        DrawTestMenu();
    }

    if (displayGrades) {
        DisplayUserGrades();
    }

    if (displayRankings) {
        DisplayUserRankings();
    }

    if (displayRemarks) {
        DisplayUserRemarks();
    }
}



void Home::HandleInput() {
    Vector2 mousePos = GetMousePosition();
    gradesButtonHovered = CheckCollisionPointRec(mousePos, gradesButton);
    rankingsButtonHovered = CheckCollisionPointRec(mousePos, rankingsButton);
    examsButtonHovered = CheckCollisionPointRec(mousePos, examsButton);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (gradesButtonHovered) {
            displayGrades = !displayGrades;
        }
        if (rankingsButtonHovered) {
            displayRankings = !displayRankings;
        }
        if (examsButtonHovered) {
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
            isTesting = true;
        }

        buttonPosition.y += buttonSpacing;
    }
}

void Home::StartTest(const string& subject) {
    cout << "Starting test for subject: " << subject << endl;
}

void Home::HandleTestMenuInput(Vector2 mousePos) {

}

float Home::CalculateAverageGrade(const string& email) {
    vector<string> grades = loginRef.GetGrades(email);
    if (grades.empty()) {
        return 0.0f;
    }
    float total = 0.0f;
    for (const auto& grade : grades) {
        total += stof(grade);
    }
    return total / grades.size();
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
    vector<string> grades = loginRef.GetGrades(email);
    if (!grades.empty()) {
        float averageGrade = CalculateAverageGrade(email);
        int yOffset = 200;
        for (const auto& grade : grades) {
            const char* gradeText = grade.c_str();
            DrawText(gradeText, 440, yOffset, 20, BLACK); 
            yOffset += 30;
        }
        string averageText = "Average Grade: ";
        averageText += to_string(averageGrade);
        size_t decimalPos = averageText.find('.') + 3;
        if (averageText.size() > decimalPos)
            averageText.erase(decimalPos);
        const char* averageTextChar = averageText.c_str();
        DrawText(averageTextChar, 440, yOffset, 20, BLACK);
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

    string rankingText = "Your Ranking: " + to_string(ranking);
    DrawText(rankingText.c_str(), 400, 200, 20, BLACK);

    string highestText = "Highest: " + highestUser + " (Average Grade: " + formatFloat(highestAverage) + ")";
    DrawText(highestText.c_str(), 400, 230, 20, BLACK);

    string lowestText = "Lowest: " + lowestUser + " (Average Grade: " + formatFloat(lowestAverage) + ")";
    DrawText(lowestText.c_str(), 400, 260, 20, BLACK);
}

string Home::formatFloat(float value) {
    string result = to_string(value);
    size_t decimalPos = result.find('.');
    if (decimalPos != string::npos && result.length() > decimalPos + 3) {
        result = result.substr(0, decimalPos + 3);
    }

    return result;
}