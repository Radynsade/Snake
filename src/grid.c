typedef struct GridPoint {
	int x;
	int y;
} GridPoint;

GridPoint *newGridPoint(
	int x,
	int y
) {
	GridPoint *point = malloc(sizeof(GridPoint));
	point->x = x;
	point->y = y;

	return point;
}

void destroyGridPoint(GridPoint *point) {
	free(point);
}

GridPoint* gridPointFromListAt(
	List *points,
	int x,
	int y
) {
	ListNode *node = points->head;
	GridPoint *point;

	while (node) {
		point = (GridPoint*)node->value;

		if (point->x == x && point->y == y) {
			return point;
		}

		node = node->next;
	}

	return NULL;
}

typedef struct GridCell {
	int x;
	int y;
	void *owner;
} GridCell;

typedef struct Grid {
	int width;
	int height;
	int isCyclic;
	GridCell **matrix;
} Grid;

Grid* newGrid(
	int width,
	int height,
	int isCyclic
) {
	Grid *grid = malloc(sizeof(Grid));
	grid->width = width;
	grid->height = height;
	grid->isCyclic = isCyclic;
	// Allocate memory for pointers to GridCell objects.
	grid->matrix = malloc(width * sizeof(GridCell*));

	for (int i = 0; i < width; i++) {
		// Allocate memory for GridCell objects.
		grid->matrix[i] = malloc(height * sizeof(GridCell));

		for (int j = 0; j < height; j++) {
			grid->matrix[i][j].owner = NULL;
			grid->matrix[i][j].x = i;
			grid->matrix[i][j].y = j;
		}
	}

	return grid;
}

void destroyGrid(Grid *grid) {
	for (int i = 0; i < grid->height; i++) {
		free(grid->matrix[i]);
	}

	free(grid->matrix);
	free(grid);
}

void cleanGrid(Grid *grid) {
	for (int y = 0; y < grid->height; y++) {
		for (int x = 0; x < grid->width; x++) {
			grid->matrix[x][y].owner = NULL;
		}
	}
}

void cleanGridAt(
	Grid *grid,
	int x,
	int y
) {
	grid->matrix[x][y].owner = NULL;
}

int gridHasPoint(
	Grid *grid,
	int x,
	int y
) {
	return x < grid->width && x >= 0 && y < grid->height && y >= 0;
}

void moveInGrid(
	Grid *grid,
	int oldX,
	int oldY,
	int newX,
	int newY
) {
	void *owner = grid->matrix[oldX][oldY].owner;

	if (!owner) {
		return;
	}

	if (gridHasPoint(grid, oldX, oldY)) {
		grid->matrix[oldX][oldY].owner = NULL;
	}

	if (gridHasPoint(grid, newX, newY)) {
		grid->matrix[newX][newY].owner = owner;
	}
}

void printGridOwners(Grid *grid) {
	printf("\n");

	for (int y = 0; y < grid->height; y++) {
		for (int x = 0; x < grid->width; x++) {
			if (grid->matrix[x][y].owner) {
				printf("X");
			} else {
				printf(".");
			}
		}
		printf("\n");
	}

	printf("\n");
}

typedef struct GridDrawOptions {
	int cellSize;
	int gap;
	int yOffset;
	int xOffset;
	Color cellColor;
	Color backgroundColor;
} GridDrawOptions;

void drawGridCell(
	GridDrawOptions options,
	int x,
	int y,
	Color color
) {
	DrawRectangle(
		x * options.cellSize + options.gap * (x + 1) + options.xOffset,
		y * options.cellSize + options.gap * (y + 1) + options.yOffset,
		options.cellSize,
		options.cellSize,
		color
	);
}

void drawGrid(
	GridDrawOptions options,
	Grid *grid
) {
	ClearBackground(options.backgroundColor);

	for (int x = 0; x < grid->width; x++) {
		for (int y = 0; y < grid->height; y++) {
			drawGridCell(options, x, y, options.cellColor);
		}
	}
}
