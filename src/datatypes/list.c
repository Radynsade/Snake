typedef struct ListNode {
	void* value;
	struct ListNode* next;
	struct ListNode* previous;
} ListNode;

ListNode* newListNode(
	void* value,
	ListNode* next,
	ListNode* previous
) {
	ListNode* newNode;
	newNode = malloc(sizeof(ListNode));
	newNode->value = value;
	newNode->next = next;
	newNode->previous = previous;

	return newNode;
}

typedef struct List {
	int size;
	struct ListNode* head;
	struct ListNode* tail;
} List;

List* newList() {
	List* list = malloc(sizeof(List));
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;

	return list;
}

void printList(List *list) {
	printf("Printing list!\n");
	ListNode *node = list->head;

	while (node) {
		printf("Node's value address = %d\n", node->value);
		node = node->next;
	}
}

void appendToList(
	List* list,
	void* value
) {
	ListNode* newNode = newListNode(value, NULL, list->tail);

	if (list->size >= 1 && list->tail) {
		list->tail->next = newNode;
	}

	if (!list->head) {
		list->head = newNode;
	}

	list->tail = newNode;
	list->size += 1;
}

void prependToList(
	List* list,
	void* value
) {
	ListNode* newNode = newListNode(value, list->head, NULL);
	
	if (list->size >= 1 && list->head) {
		list->head->previous = newNode;
	}

	list->head = newNode;
	list->size += 1;
}

void cleanList(List *list) {
	ListNode *current = list->head;
	ListNode *previous = NULL;
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;

	while (current) {
		previous = current;
		free(previous);
		current = current->next;
	}
}

void destroyAllInList(List *list) {
	ListNode *current = list->head;
	ListNode *previous = NULL;
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;

	while (current) {
		previous = current;
		free(previous->value);
		free(previous);
		current = current->next;
	}
}

void removeFromList(
	List *list,
	void *pointer
) {
	ListNode *current = list->head;

	while (current) {
		if (current->value == pointer) {
			list->size -= 1;

			ListNode *next = current->next;
			ListNode *previous = current->previous;
			free(current);

			if (list->size == 0) {
				list->head = NULL;
				list->tail = NULL;

				break;
			}

			if (list->size == 1) {
				if (next) {
					list->head = next;
					list->tail = next;
				} else if (previous) {
					list->head = previous;
					list->tail = previous;
				}

				list->head->next = NULL;
				list->head->previous = NULL;

				break;
			}

			if (previous && next) {
				previous->next = next;
				next->previous = previous;
			}

			if (previous && !next) {
				previous->next = NULL;
				list->tail = previous;
			}

			if (next && !previous) {
				next->previous = NULL;
				list->head = next;
			}
		}

		current = current->next;
	}
}

void loopList(
	List* list,
	void (*callback)(ListNode* node, int index)
) {
	ListNode* current = list->head;
	int index = 0;

	while (current) {
		callback(current, index);
		current = current->next;
		index++;
	}
}

// Deallocates all the list's members and the list itself.
void destroyList(List* list) {
	ListNode* current = list->head;

	while (current) {
		if (!current->previous) {
			current = current->next;
			continue;
		}

		free(current->previous);

		if (!current->next) {
			free(current);
			break;
		}

		current = current->next;
	}

	free(list);
}

int isListValid(List* list) {
	if (list->size == 0) {
		// Empty list cannot contain head and tail.
		return !list->head && !list->tail;
	}

	if (list->size == 1) {
		// Head and tail must have the same addresses if size of list is one.
		return list->head == list->tail &&
			!list->head->next &&
			!list->head->previous;
	}

	if (
		!list->head
		|| !list->tail
		|| list->head->previous
		|| !list->head->next
		|| list->tail->next
		|| !list->tail->previous
	) {
		return 0;
	}

	ListNode *current = list->head;
	int amount = 0;

	while (current->next) {
		amount++;

		if (
			amount > list->size
			|| (amount < list->size && !current->next)
		) {
			return 0;
		}

		if (
			current != list->head
			&& current != list->tail
			&& (!current->next || !current->previous)
		) {
			return 0;
		}

		current = current->next;
	}

	return 1;
}
