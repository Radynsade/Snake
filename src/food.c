void printFoods(List *foods) {
	ListNode *node = foods->head;
	GridPoint *food;

	while (node) {
		food = (GridPoint*)node->value;

		printf("food at x = %d, y = %d, address = %d\n", food->x, food->y, food);

		node = node->next;
	}
}

void spawnFood(
	List *foods,
	Grid *grid
) {
	int x, y;

	do {
		x = randomInt(0, grid->width - 1);
		y = randomInt(0, grid->height - 1);
	} while (grid->matrix[x][y].owner);

	GridPoint *food = newGridPoint(x, y);
	appendToList(foods, food);
	grid->matrix[x][y].owner = food;
}

void drawFood(
	GridDrawOptions gridOptions,
	Color color,
	List *foods
) {
	ListNode *node = foods->head;
	GridPoint *food;

	while (node) {
		food = (GridPoint*)node->value;
		drawGridCell(gridOptions, food->x, food->y, color);
		node = node->next;
	}
}
