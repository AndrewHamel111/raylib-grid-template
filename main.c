// Raylib Grid Game Template
// Andrew Hamel Codes
// https://github.com/AndrewHamel111/raylib-grid-template

#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#define COLS 4
#define ROWS 4
#define CELL_VARIANT 0 // xϵ[0,7]

const int screenWidth = 600;
const int screenHeight = 600;
const Vector2 screenSize = {screenWidth, screenHeight};

const float gridAreaWidth = 500;
const float gridAreaHeight = 500;
const float gridAreaOriginX = 50;
const float gridAreaOriginY = 50;
const Rectangle gridArea = {gridAreaOriginX, gridAreaOriginY, gridAreaWidth, gridAreaHeight};
Rectangle _rectGridAreaBorder;

const int cellWidth = gridAreaWidth / COLS;
const int cellHeight = gridAreaHeight / ROWS;

const char* _textGameOver = "GAME OVER!";
const char* _textPressToRestart = "Press 'r' to play again!";

typedef struct Cell
{
	int i;
	int j;
} Cell;

Cell grid[COLS][ROWS];

typedef enum GameState
{
	PLAYING,
	END
} GameState;

GameState _state;

float _timeGameStarted;
float _timeGameEnded;

Texture2D _atlasTile;
Texture2D _atlasDigits;

Rectangle _sourceTile;

void CellDraw(Cell);
bool IndexIsValid(int, int);
bool CellPick(int, int, Cell**);
bool TryCellPick(Vector2, Cell**);
void GridInit(void);
void GameInit(void);

void DrawNumber(int, Rectangle);
void DrawDigit(int, Rectangle, Color);

void UpdateDrawFrame(void);

int main()
{
	srand(time(0));

	InitWindow(screenWidth, screenHeight, "Raylib Grid Template by Andrew Hamel");

	// Load Resources
	_atlasTile = LoadTexture("resources/tile.png"); // _atlasTile = LoadTexture("resources/tile_thick.png");
	_atlasDigits = LoadTexture("resources/digits.png");

	_sourceTile = (Rectangle){CELL_VARIANT * _atlasTile.height, 0, _atlasTile.height, _atlasTile.height};
	_rectGridAreaBorder = (Rectangle){gridAreaOriginX - cellWidth / 32, gridAreaOriginY - cellHeight / 32, gridAreaWidth + cellWidth / 16, gridAreaHeight + cellHeight / 16};

	GameInit();

	while(!WindowShouldClose())
	{
		UpdateDrawFrame();
	}
	
	// Unload Resources

	CloseWindow();
	
	return 0;
}

void CellDraw(Cell cell)
{
	const int tileSize = _atlasTile.height;
	Rectangle dest = {cell.i * cellWidth + gridAreaOriginX, cell.j * cellHeight + gridAreaOriginY, cellWidth, cellHeight};
	DrawTexturePro(_atlasTile, _sourceTile, dest, (Vector2){0, 0}, 0, WHITE);
}

bool IndexIsValid(int i, int j)
{
	return i >= 0 && i < COLS && j >= 0 && j < ROWS;
}

bool TryCellPick(Vector2 mousePosition, Cell** outCell)
{
	const int i = mousePosition.x / cellWidth;
	const int j = mousePosition.y / cellHeight;

	return IndexIsValid(i, j) && CellPick(i, j, outCell);
}

bool CellPick(int i, int j, Cell** outCell)
{
	*outCell = &(grid[i][j]);
	return false;
}

void GridInit(void)
{
	for (int i = 0; i < COLS; i++)
	{
		for (int j = 0; j < ROWS; j++)
		{
			// Safe, default values for Cell
			grid[i][j] = (Cell)
			{
				.i = i,
				.j = j
			};
		}
	}
}

void GameInit(void)
{
	GridInit();
	_state = PLAYING;
	_timeGameStarted = GetTime();
}

void DrawNumber(int number, Rectangle destination)
{
	// TODO: iteratively call DrawDigit
	// TODO: decide how to handle when fitting the number in destination would require squishing
	// TODO: decide on alignment
}

void DrawDigit(int digit, Rectangle destination, Color color)
{
	int digitSourceSize = _atlasDigits.height;
	const Rectangle source = {digit * digitSourceSize, 0, digitSourceSize, digitSourceSize};
	DrawTexturePro(_atlasDigits, source, destination, (Vector2){0, 0}, 0, color);
}

void UpdateDrawFrame(void)
{
	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		Cell* cellPicked;
		Vector2 mPos = GetMousePosition();

		if (_state != END && TryCellPick(mPos, &cellPicked))
		{
			// TODO: do work on picked cell
		}
	}

	// Reset game check
	if (_state == END && IsKeyPressed(KEY_R))
	{
		GameInit();
	}

	BeginDrawing();

		ClearBackground(RAYWHITE);

		// Draw GRID
		// Optional: draw a border of the grid so the edges don't look as weird
		DrawTexturePro(_atlasTile, _sourceTile, _rectGridAreaBorder, (Vector2){0, 0}, 0, WHITE);
		for (int i = 0; i < COLS; i++)
		{
			for (int j = 0; j < ROWS; j++)
			{
				CellDraw(grid[i][j]);
			}
		}

		if (_state == END)
		{
			DrawRectangle(0, 0, screenWidth,screenHeight, Fade(WHITE, 0.8f));
			DrawText(_textGameOver, screenWidth / 2 - MeasureText(_textGameOver, 40) / 2, screenHeight / 2 - 10, 40, DARKGRAY);
			DrawText(_textPressToRestart, screenWidth / 2 - MeasureText(_textPressToRestart, 20) / 2, screenHeight * 0.75f - 10, 20, DARKGRAY);

			int minutes = (int)(_timeGameEnded - _timeGameStarted) / 60;
			int seconds = (int)(_timeGameEnded - _timeGameStarted) % 60;
			DrawText(TextFormat("Time played: %d minutes, %d seconds.", minutes, seconds), 20, screenHeight - 40, 20, DARKGRAY);
		}
		else
		{
			// TODO: Gametime HUD
		}

	EndDrawing();
}