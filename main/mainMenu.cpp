#include <iostream>
#include "raylib.h"
using namespace std;

int main() {
	int screenWidth = 1366;
	int screenHeight = 768;
	int buttonWidth = 200;
	int buttonHeight = 50;
	int menuChoice = 0;

	InitWindow(screenWidth, screenHeight, "Storyliner");

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLUE);

		DrawText("Welcome to Storyliner", screenWidth / 2 - 100, screenHeight / 2 - 50, 20, BLACK);

		DrawRectangle(screenWidth / 2 - buttonWidth / 2, screenHeight / 2, buttonWidth, buttonHeight, DARKBLUE);
		DrawText("New Story", screenWidth / 2 - 50, screenHeight / 2 + 10, 20, BLACK);

		DrawRectangle(screenWidth / 2 - buttonWidth / 2, screenHeight * 0.4, buttonWidth, buttonHeight, ORANGE);
		DrawText("Load Story", screenWidth / 2 - 50, screenHeight * 0.4 + 10, 20, BLACK);

		DrawRectangle(screenWidth / 2 - buttonWidth / 2, screenHeight * 0.3, buttonWidth, buttonHeight, RED);
		DrawText("Exit", screenWidth / 2 - 25, screenHeight * 0.3 + 10, 20, BLACK);

		EndDrawing();
	}

	switch (menuChoice) {
	case 1:
		cout << "New Story selected!" << endl;
		break;
	case 2:
		cout << "Load Story selected!" << endl;
		break;
	case 3:
		cout << "Close Storyliner" << endl;
		CloseWindow();
		return 0;
		break;
	}
}