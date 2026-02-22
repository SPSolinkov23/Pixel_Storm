
#include <iostream>
#include "raylib.h"
#include <string>	
using namespace std;
struct Event {
	int year;
	string name;
	string type; //битка,мир/създаване на държава
	string result; //победа загуба неутрално


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

int main() {
	int screenWidth = 1366;
	int screenHeight = 768;
	int buttonWidth = 200;
	int buttonHeight = 50;
	int menuChoice = 0;
	Node* head = nullptr;

	InitWindow(screenWidth, screenHeight, "Storyliner");

	bool running = true;
	while (running && !WindowShouldClose()) {
		// button positions
		int buttonX = screenWidth / 2 - buttonWidth / 2;
		int newY = screenHeight / 2;
		int loadY = (int)(screenHeight * 0.4);
		int exitY = (int)(screenHeight * 0.3);

		BeginDrawing();
		ClearBackground(BLUE);

		DrawText("Welcome to Storyliner", screenWidth / 2 - 100, screenHeight / 2 - 50, 20, BLACK);

		DrawRectangle(buttonX, newY, buttonWidth, buttonHeight, DARKBLUE);
		DrawText("New Story", screenWidth / 2 - 50, newY + 10, 20, BLACK);

		DrawRectangle(buttonX, loadY, buttonWidth, buttonHeight, ORANGE);
		DrawText("Load Story", screenWidth / 2 - 50, loadY + 10, 20, BLACK);

		DrawRectangle(buttonX, exitY, buttonWidth, buttonHeight, RED);
		DrawText("Exit", screenWidth / 2 - 25, exitY + 10, 20, BLACK);

		EndDrawing();

		// Check for mouse clicks on buttons after drawing
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			Vector2 mouse = GetMousePosition();
			Rectangle rectNew = { (float)buttonX, (float)newY, (float)buttonWidth, (float)buttonHeight };
			Rectangle rectLoad = { (float)buttonX, (float)loadY, (float)buttonWidth, (float)buttonHeight };
			Rectangle rectExit = { (float)buttonX, (float)exitY, (float)buttonWidth, (float)buttonHeight };

			if (CheckCollisionPointRec(mouse, rectNew)) {
				// New Story selected - use console input to add an event
				cout << "New Story selected!" << endl;
				int y;
				string n, t, r;

				cout << "Year: ";
				cin >> y;
				cin.ignore();

				cout << "Name: ";
				getline(cin, n);

				cout << "Type (Battle/Politics): ";
				getline(cin, t);

				cout << "Result (Victory/Loss/None): ";
				getline(cin, r);

				AddEvent(head, y, n, t, r);
			}
			else if (CheckCollisionPointRec(mouse, rectLoad)) {
				cout << "Load Story selected!" << endl;
				showTimeline(head);
			}
			else if (CheckCollisionPointRec(mouse, rectExit)) {
				cout << "Close Storyliner" << endl;
				CloseWindow();
				running = false;
			}
		}
	}

	switch (menuChoice) {
	case 1: {
		cout << "New Story selected!" << endl;
		int y;
		string n, t, r;

		cout << "Year: ";
		cin >> y;
		cin.ignore();

		cout << "Name: ";
		getline(cin, n);

		cout << "Type (Battle/Politics): ";
		getline(cin, t);

		cout << "Result (Victory/Loss/None): ";
		getline(cin, r);

		AddEvent(head, y, n, t, r);
		break;
	}
	case 2:
		cout << "Load Story selected!" << endl;
		showTimeline(head);
		break;
	case 3: {
		cout << "Close Storyliner" << endl;
		CloseWindow();
		return 0;
		break;
	}
	}
}