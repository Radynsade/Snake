typedef enum SnakeDirection{
	UP,
	DOWN,
	LEFT,
	RIGHT
} SnakeDirection;

bool isUpPressed() {
	return IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP);
}

bool isDownPressed() {
	return IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN);
}

bool isLeftPressed() {
	return IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT);
}

bool isRightPressed() {
	return IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT);
}

typedef enum SnakeSpeedUpMechanism{
	MULTIPLYING,
	ADDING
} SnakeSpeedUpMechanism;

typedef struct Snake {
	SnakeDirection direction;
	SnakeSpeedUpMechanism mechanism;
	int speedUpAfterFoods;
	int speedUpFoodCounter;
	float speedUpUnit;
	float initStepDelay;
	float minStepDelay;
	float stepDelay;
	List* parts;
	int isDead;
} Snake;

Snake* newSnake(
	SnakeDirection direction,
	SnakeSpeedUpMechanism mechanism,
	int speedUpAfterFoods,
	float speedUpUnit,
	float initStepDelay,
	float minStepDelay,
	Grid *grid,
	int x,
	int y
) {
	List* parts = newList();
	ListNode* head = newGridPoint(x, y);
	appendToList(parts, head);
	Snake* snake = malloc(sizeof(Snake));
	snake->direction = direction;
	snake->mechanism = mechanism;
	snake->speedUpAfterFoods = speedUpAfterFoods;
	snake->stepDelay = initStepDelay;
	snake->initStepDelay = initStepDelay;
	snake->minStepDelay = minStepDelay;
	snake->parts = parts;
	snake->isDead = 0;
	snake->speedUpFoodCounter = 0;
	snake->speedUpUnit = speedUpUnit;
	grid->matrix[x][y].owner = head;

	return snake;
}

void destroySnake(Snake *snake) {
	ListNode *node = snake->parts->head;

	while (node) {
		destroyGridPoint((GridPoint*)node->value);
		node = node->next;
	}

	destroyList(snake->parts);
	free(snake);
}

void setSnakeDirection(Snake *snake) {
	if (isUpPressed()) {
		snake->direction = UP;
	}

	if (isDownPressed()) {
		snake->direction = DOWN;
	}

	if (isLeftPressed()) {
		snake->direction = LEFT;
	}

	if (isRightPressed()) {
		snake->direction = RIGHT;
	}
}

void increaseSnake(Snake *snake, Grid *grid, int x, int y) {
	GridPoint *part = newGridPoint(x, y);
	appendToList(snake->parts, part);
	grid->matrix[x][y].owner = part;
	snake->speedUpFoodCounter += 1;
}

void doSnakeStep(
	Snake *snake,
	List *foods,
	Grid *grid
) {
	int previousX, previousY, newX, newY, newPartX, newPartY;
	ListNode *node = snake->parts->head;
	GridPoint *part = NULL;
	GridPoint *food = NULL;

	while (node) {
		part = (GridPoint*)node->value;

		if (node == snake->parts->head) {
			newX = part->x;
			newY = part->y;
			previousX = part->x;
			previousY = part->y;

			switch (snake->direction) {
				case UP:
					newY = part->y - 1;

					if (grid->isCyclic && newY < 0) {
						newY = grid->height - 1;
					}
				break;
				case DOWN:
					newY = part->y + 1;

					if (grid->isCyclic && newY >= grid->height) {
						newY = 0;
					}
				break;
				case LEFT:
					newX = part->x - 1;

					if (grid->isCyclic && newX < 0) {
						newX = grid->width - 1;
					}
				break;
				case RIGHT:
					newX = part->x + 1;

					if (grid->isCyclic && newX >= grid->width) {
						newX = 0;
					}
				break;
			}

			if (gridPointFromListAt(snake->parts, newX, newY)) {
				snake->isDead = 1;
			}

			if (!food) {
				food = gridPointFromListAt(foods, newX, newY);
			}
		} else {
			newX = previousX;
			newY = previousY;
			previousX = part->x;
			previousY = part->y;
		}

		part->x = newX;
		part->y = newY;

		moveInGrid(
			grid,
			previousX,
			previousY,
			part->x,
			part->y
		);

		node = node->next;
	}

	if (food) {
		// On moment when snake eats food there is no pointer to food in grid
		// because it was replaced by pointer to snake's head.

		removeFromList(foods, food);
		destroyGridPoint(food);
		food = NULL;
		increaseSnake(snake, grid, previousX, previousY);
		spawnFood(foods, grid);

		if (
			snake->speedUpAfterFoods > 0 &&
			snake->speedUpFoodCounter >= snake->speedUpAfterFoods
		) {
			snake->speedUpFoodCounter = 0;

			if (snake->stepDelay <= snake->minStepDelay) {
				return;
			}

			if (snake->mechanism == MULTIPLYING) {
				snake->stepDelay = snake->stepDelay / snake->speedUpUnit;
			} else {
				snake->stepDelay = snake->stepDelay - snake->speedUpUnit;
			}

			if (snake->stepDelay < snake->minStepDelay) {
				snake->stepDelay = snake->minStepDelay;
			}
		}
	}
}

int snakeTick(
	Snake* snake,
	float dt
) {
	if (snake->isDead) {
		return 0;
	}

	static float fromPreviousTick = 0;
	fromPreviousTick += dt;

	if (fromPreviousTick >= snake->stepDelay) {
		fromPreviousTick = 0;
		return 1;
	}

	return 0;
}

typedef struct SnakeDrawOptions {
	Color headColor;
	Color partColor;
} SnakeDrawOptions;

void drawSnake(
	Snake *snake,
	SnakeDrawOptions snakeOptions,
	GridDrawOptions gridOptions
) {
	ListNode *node = snake->parts->head;
	GridPoint *part;
	Color *color;

	while (node) {
		part = (GridPoint*)node->value;

		if (snake->isDead) {
			color = &RED;
		} else {
			if (node == snake->parts->head) {
				color = &snakeOptions.headColor;
			} else {
				color = &snakeOptions.partColor;
			}
		}


		drawGridCell(
			gridOptions,
			part->x,
			part->y,
			*color
		);

		node = node->next;
	}
}
