#include <raylib.h>
#include <time.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include "SnakeConfig.h"
#include "datatypes/list.c"
#include "random.c"
#include "clock.c"
#include "grid.c"
#include "food.c"
#include "snake.c"

void restartGame(
	Grid *grid,
	List *foods,
	Snake *snake
) {
	cleanGrid(grid);
	destroyAllInList(snake->parts);
	destroyAllInList(foods);

	int x = randomInt(0, grid->width - 1);
	int y = randomInt(0, grid->height - 1);
	snake->direction = UP;
	snake->isDead = 0;
	snake->speedUpFoodCounter = 0;
	snake->stepDelay = snake->initStepDelay;

	ListNode *head = newGridPoint(x, y);
	appendToList(snake->parts, head);
	spawnFood(foods, grid);
}

int main() {
	srand(time(NULL));

	// Options

	const char *WINDOW_TITLE = "Snake";
	const int GRID_WIDTH = 16;
	const int GRID_HEIGHT = 16;
	const int GRID_IS_CYCLIC = 1;
	const int GRID_CELL_SIZE = 20;
	const int GRID_CELL_GAP = 1;
	const int GRID_DRAW_WIDTH = GRID_WIDTH * (GRID_CELL_SIZE + GRID_CELL_GAP) + GRID_CELL_GAP;
	const int GRID_DRAW_HEIGHT = GRID_HEIGHT * (GRID_CELL_SIZE + GRID_CELL_GAP) + GRID_CELL_GAP;
	const Color GRID_CELL_COLOR = (Color) {60, 60, 60, 255};
	const Color GRID_BACKGROUND_COLOR = (Color) {40, 40, 40, 255};
	const Color SNAKE_PART_COLOR = (Color) {255, 200, 0, 255};
	const Color SNAKE_HEAD_COLOR = WHITE;
	const float INITIAL_SNAKE_DELAY = 0.2F;
	const float MIN_SNAKE_DELAY = 0.03F;
	const int SPEED_UP_AFTER_FOOD = 5;
	const SnakeSpeedUpMechanism SPEED_UP_MECHANISM = MULTIPLYING;
	const float SPEED_UP_UNIT = 1.1F;
	const Color FOOD_COLOR = (Color) {14, 237, 126, 255};
	const int HUD_HEIGHT = 38;
	const int WINDOW_WIDTH = GRID_DRAW_WIDTH;
	const int WINDOW_HEIGHT = GRID_DRAW_HEIGHT + HUD_HEIGHT;
	const int FONT_SIZE = 40;
	const char* PRESS_ENTER_TEXT = "PRESS ENTER";
	const int PRESS_ENTER_WIDTH = 10 * (FONT_SIZE - 10) + (20 - 10);

	// Game objects init

	Grid *grid = newGrid(GRID_WIDTH, GRID_HEIGHT, GRID_IS_CYCLIC);
	List *foods = newList();
	int x = randomInt(0, GRID_WIDTH - 1);
	int y = randomInt(0, GRID_HEIGHT - 1);

	Snake *snake = newSnake(
		UP,
		SPEED_UP_MECHANISM,
		SPEED_UP_AFTER_FOOD,
		SPEED_UP_UNIT,
		INITIAL_SNAKE_DELAY,
		MIN_SNAKE_DELAY,
		grid,
		x,
		y
	);

	SnakeDrawOptions snakeDrawOptions = (SnakeDrawOptions) {
		.headColor = SNAKE_HEAD_COLOR,
		.partColor = SNAKE_PART_COLOR
	};

	GridDrawOptions gridDrawOptions = (GridDrawOptions) {
		.backgroundColor = GRID_BACKGROUND_COLOR,
		.cellColor = GRID_CELL_COLOR,
		.cellSize = GRID_CELL_SIZE,
		.gap = GRID_CELL_GAP,
		.xOffset = 0,
		.yOffset = HUD_HEIGHT
	};

	// Raylib init

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
	InitWindow(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		WINDOW_TITLE
	);
	SetTargetFPS(60);
	SetExitKey(KEY_ESCAPE);

	// Clock

	int64_t previousMicros = currentMicroseconds();
	int64_t currentMicros;
	int64_t deltaMicros;
	float deltaSeconds;

	// Game loop

	spawnFood(foods, grid);

	while (!WindowShouldClose()) {
		// Time handling

		currentMicros = currentMicroseconds();
		deltaMicros = currentMicros - previousMicros;
		deltaSeconds = microsecondsToSeconds(deltaMicros);
		previousMicros = currentMicros;

		// Game logic

		setSnakeDirection(snake);

		if (snakeTick(snake, deltaSeconds)) {
			doSnakeStep(snake, foods, grid);
		} else if (snake->isDead && IsKeyPressed(KEY_ENTER)) {
			restartGame(grid, foods, snake);
		}

		// Drawing

		BeginDrawing();
		drawGrid(gridDrawOptions, grid);
		drawSnake(snake, snakeDrawOptions, gridDrawOptions);
		drawFood(gridDrawOptions, FOOD_COLOR, foods);

		// HUD

		if (snake->isDead) {
			DrawText(
				PRESS_ENTER_TEXT,
				(GRID_DRAW_WIDTH - PRESS_ENTER_WIDTH) / 2,
				GRID_DRAW_HEIGHT / 2 + HUD_HEIGHT - 20,
				FONT_SIZE,
				WHITE
			);
		}

		DrawText(TextFormat("FOODS: %d", snake->parts->size - 1), 4, 1, 40, WHITE);

		EndDrawing();
	}

	// Game objects free

	destroyGrid(grid);
	destroyList(foods);
	destroySnake(snake);

	return 0;
}
