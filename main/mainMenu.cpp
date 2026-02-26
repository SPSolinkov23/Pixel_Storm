
#include <iostream>
#include "raylib.h"
#include <string>	
using namespace std;

struct Line {
    struct Event {
        int year;
        string name;
        string type; 
        string result; 
	};
};

struct Event {
    int year;
    string name;
    string type; 
    string result; 
};

struct Node {
    Event data;
    Node* next;
};

void AddEvent(Node*& head, int year, const string& name, const string& type, const string& result)
{
    Event newEv = { year, name, type, result };
    Node* newNode = new Node{ newEv, nullptr };
    if (!head || year < head->data.year) {
        newNode->next = head;
        head = newNode;
        return;
    }
    Node* current = head;
    while (current->next && current->next->data.year < year) {
        current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
}

void showTimeline(Node* head) {
    if (!head) {
        cout << "(timeline is empty)\n";
        return;
    }
    Node* temp = head;
    while (temp) {
        cout << temp->data.year << " " << temp->data.name << " (" << temp->data.type << ") ";
        if (temp->data.type == "Politics") cout << endl;
        if (temp->data.type == "Battle") cout << "- Result: " << temp->data.result << endl;
        temp = temp->next;
    }
}

void SortByYear(Node*& head) {
    Node* sorted = nullptr;

    while (head) 
    {
        Node* current = head;
        head = head->next;
        if (!sorted || current->data.year < sorted->data.year) 
        {
            current->next = sorted;
            sorted = current;
        }
        else 
        {
            Node* temp = sorted;
            while (temp->next && temp->next->data.year < current->data.year) 
            {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
    }
    head = sorted;
}




int main() {
    int screenWidth = 1366;
    int screenHeight = 768;
    int buttonWidth = 200;
    int buttonHeight = 50;
    Node* head = nullptr;

    InitWindow(screenWidth, screenHeight, "Storyliner");

    bool running = true;
    bool newStoryMode = false;
    bool timelineMode = false;
    float timelineScroll = 0.0f;
    int activeField = 0;
    string inputYear, inputName, inputType, inputResult;

    while (running && !WindowShouldClose()) {
        int buttonX = screenWidth / 2 - buttonWidth / 2;
        int newY = screenHeight / 2;
        int loadY = (int)(screenHeight * 0.6);
        int exitY = (int)(screenHeight * 0.7);


        int inputBoxW = 600;
        int inputBoxH = 40;
        int inputX = screenWidth / 2 - inputBoxW / 2;
        int inputStartY = screenHeight / 2 - 150;


        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            Rectangle rectNew = { (float)buttonX, (float)newY, (float)buttonWidth, (float)buttonHeight };
            Rectangle rectLoad = { (float)buttonX, (float)loadY, (float)buttonWidth, (float)buttonHeight };
            Rectangle rectExit = { (float)buttonX, (float)exitY, (float)buttonWidth, (float)buttonHeight };

            if (!newStoryMode && CheckCollisionPointRec(mouse, rectNew)) {

                newStoryMode = true;
                activeField = 0;
                inputYear.clear(); inputName.clear(); inputType.clear(); inputResult.clear();
            }
            else if (!newStoryMode && CheckCollisionPointRec(mouse, rectLoad)) {

                timelineMode = true;
                timelineScroll = 0.0f;
            }
            else if (!newStoryMode && CheckCollisionPointRec(mouse, rectExit)) {
                CloseWindow();
                running = false;
            }

            if (newStoryMode) {
                Rectangle boxYear = { (float)inputX, (float)inputStartY, (float)inputBoxW, (float)inputBoxH };
                Rectangle boxName = { (float)inputX, (float)(inputStartY + 60), (float)inputBoxW, (float)inputBoxH };
                Rectangle boxType = { (float)inputX, (float)(inputStartY + 120), (float)inputBoxW, (float)inputBoxH };
                Rectangle boxResult = { (float)inputX, (float)(inputStartY + 180), (float)inputBoxW, (float)inputBoxH };
                Rectangle btnSave = { (float)(screenWidth / 2 - 120), (float)(inputStartY + 260), 100.0f, 40.0f };
                Rectangle btnCancel = { (float)(screenWidth / 2 + 20), (float)(inputStartY + 260), 100.0f, 40.0f };

                if (CheckCollisionPointRec(mouse, boxYear)) activeField = 0;
                else if (CheckCollisionPointRec(mouse, boxName)) activeField = 1;
                else if (CheckCollisionPointRec(mouse, boxType)) activeField = 2;
                else if (CheckCollisionPointRec(mouse, boxResult)) activeField = 3;
                else if (CheckCollisionPointRec(mouse, btnSave)) {

                    try {
                        int y = stoi(inputYear);
                        AddEvent(head, y, inputName, inputType, inputResult);
                    }
                    catch (...) {

                    }
                    newStoryMode = false;
                }
                else if (CheckCollisionPointRec(mouse, btnCancel)) {
                    newStoryMode = false;
                }
            }

            if (timelineMode) {
                Rectangle backBtn = { 20.0f, 20.0f, 100.0f, 40.0f };
                if (CheckCollisionPointRec(mouse, backBtn)) {
                    timelineMode = false;
                }
            }
        }

        if (newStoryMode) {
            int key = GetCharPressed();
            while (key > 0) {
                if (key >= 32 && key <= 125) {
                    char c = (char)key;
                    if (activeField == 0) {

                        if (isdigit(c)) inputYear.push_back(c);
                    }
                    else if (activeField == 1) inputName.push_back(c);
                    else if (activeField == 2) inputType.push_back(c);
                    else if (activeField == 3) inputResult.push_back(c);
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (activeField == 0 && !inputYear.empty()) inputYear.pop_back();
                else if (activeField == 1 && !inputName.empty()) inputName.pop_back();
                else if (activeField == 2 && !inputType.empty()) inputType.pop_back();
                else if (activeField == 3 && !inputResult.empty()) inputResult.pop_back();
            }

            if (IsKeyPressed(KEY_ENTER)) {
                try {
                    int y = stoi(inputYear);
                    AddEvent(head, y, inputName, inputType, inputResult);
                }
                catch (...) {}
                newStoryMode = false;
            }
        }

        BeginDrawing();
        ClearBackground(BLUE);

        if (!newStoryMode && !timelineMode) {
            DrawText("Welcome to Storyliner", screenWidth / 2 - 275, screenHeight / 4, 50, BLACK);

            DrawRectangle(buttonX, newY, buttonWidth, buttonHeight, DARKBLUE);
            DrawText("New Story", screenWidth / 2 - 50, newY + 20, 20, BLACK);

            DrawRectangle(buttonX, loadY, buttonWidth, buttonHeight, ORANGE);
            DrawText("Load Story", screenWidth / 2 - 50, loadY + 10, 20, BLACK);

            DrawRectangle(buttonX, exitY, buttonWidth, buttonHeight, RED);
            DrawText("Exit", screenWidth / 2 - 25, exitY + 10, 20, BLACK);
        }

        if (newStoryMode) {
            Rectangle boxYear = { (float)inputX, (float)inputStartY, (float)inputBoxW, (float)inputBoxH };
            Rectangle boxName = { (float)inputX, (float)(inputStartY + 60), (float)inputBoxW, (float)inputBoxH };
            Rectangle boxType = { (float)inputX, (float)(inputStartY + 120), (float)inputBoxW, (float)inputBoxH };
            Rectangle boxResult = { (float)inputX, (float)(inputStartY + 180), (float)inputBoxW, (float)inputBoxH };
            Rectangle btnSave = { (float)(screenWidth / 2 - 120), (float)(inputStartY + 260), 100.0f, 40.0f };
            Rectangle btnCancel = { (float)(screenWidth / 2 + 20), (float)(inputStartY + 260), 100.0f, 40.0f };

            DrawRectangleRec(boxYear, LIGHTGRAY);
            DrawRectangleRec(boxName, LIGHTGRAY);
            DrawRectangleRec(boxType, LIGHTGRAY);
            DrawRectangleRec(boxResult, LIGHTGRAY);

            if (activeField == 0) DrawRectangleLines((int)boxYear.x, (int)boxYear.y, (int)boxYear.width, (int)boxYear.height, RED);
            if (activeField == 1) DrawRectangleLines((int)boxName.x, (int)boxName.y, (int)boxName.width, (int)boxName.height, RED);
            if (activeField == 2) DrawRectangleLines((int)boxType.x, (int)boxType.y, (int)boxType.width, (int)boxType.height, RED);
            if (activeField == 3) DrawRectangleLines((int)boxResult.x, (int)boxResult.y, (int)boxResult.width, (int)boxResult.height, RED);

            DrawText("Year:", inputX - 80, inputStartY + 10, 20, BLACK);
            DrawText(inputYear.c_str(), inputX + 8, inputStartY + 10, 20, BLACK);

            DrawText("Name:", inputX - 80, inputStartY + 70, 20, BLACK);
            DrawText(inputName.c_str(), inputX + 8, inputStartY + 70, 20, BLACK);

            DrawText("Type:", inputX - 80, inputStartY + 130, 20, BLACK);
            DrawText(inputType.c_str(), inputX + 8, inputStartY + 130, 20, BLACK);

            DrawText("Result:", inputX - 80, inputStartY + 190, 20, BLACK);
            DrawText(inputResult.c_str(), inputX + 8, inputStartY + 190, 20, BLACK);

            DrawRectangleRec(btnSave, GREEN);
            DrawRectangleRec(btnCancel, RED);
            DrawText("Save", (int)btnSave.x + 20, (int)btnSave.y + 10, 20, BLACK);
            DrawText("Cancel", (int)btnCancel.x + 10, (int)btnCancel.y + 10, 20, BLACK);
        }

        if (timelineMode) {
            Rectangle backBtn = { 20.0f, 20.0f, 100.0f, 40.0f };
            DrawRectangleRec(backBtn, LIGHTGRAY);
            DrawText("Back", 50, 30, 20, BLACK);

            float wheel = GetMouseWheelMove();
            if (wheel != 0) timelineScroll += wheel * 20.0f;

            int drawX = 100;
            int y = (int)(80 + timelineScroll);
            Node* temp = head;
            while (temp) {
                std::string line = std::to_string(temp->data.year) + " " + temp->data.name + " (" + temp->data.type + ")";
                DrawText(line.c_str(), drawX, y, 20, BLACK);
                y += 24;
                if (temp->data.type == "Battle") {
                    std::string res = "   - Result: " + temp->data.result;
                    DrawText(res.c_str(), drawX, y, 20, DARKGRAY);
                    y += 24;
                }
                temp = temp->next;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
