/**
 * -------------------------------------
 * @file  queue_linked.c
 * Linked Queue Source Code File
 * -------------------------------------
 * @author Filip Stanojcic
 *
 * @version 2024-02-28
 *
 */
// Includes
#include "queue_linked.h"

// Functions

/**
 * Initializes a queue.
 *
 * @return - pointer to a new queue
 */
queue_linked* queue_initialize() {
	queue_linked *source = malloc(sizeof source);
	source->front = NULL;
	source->rear = NULL;
	source->count = 0;
	return source;
}

/**
 * Frees queue memory.
 *
 * @param source - pointer to a queue
 */
void queue_free(queue_linked **source) {
	while ((*source)->front != NULL) {
		queue_node *temp = (*source)->front;
		data_free(&temp->item);
		(*source)->front = (*source)->front->next;
		free(temp);
		temp = NULL;
	}
	free(*source);
	(*source)->rear = NULL;
	*source = NULL;
	return;
}

/**
 * Determines if a queue is empty.
 *
 * @param source - pointer to a queue
 * @return - true if source is empty, false otherwise
 */
BOOLEAN queue_empty(const queue_linked *source) {
	return (source->front == NULL);
}

/**
 * Returns the number of items in a queue.
 *
 * @param source - pointer to a queue
 * @return - the number of items in source
 */
int queue_count(const queue_linked *source) {
	return (source->count);
}

/**
 * Pushes a copy of an item onto a queue.
 *
 * @param source - pointer to a queue
 * @param item - pointer to the item to insert
 */
void queue_insert(queue_linked *source, data_ptr item) {
	queue_node *node = malloc(sizeof *node);
	node->item = malloc(sizeof *node->item);
	data_copy(node->item, item);
	if (source->front != NULL) {
		source->rear->next = node;
		source->rear = node;
	} else {
		source->front = node;
		source->rear = node;
	}
	source->count += 1;
	return;
}

/**
 * Returns a copy of the item on the front of a queue, queue is unchanged.
 *
 * @param source - pointer to a queue
 * @param item - pointer to a copy of the item to retrieve
 * @return - true if item peeked, false otherwise (queue is empty)
 */
BOOLEAN queue_peek(const queue_linked *source, data_ptr item) {
	BOOLEAN peeked = FALSE;

	if (source->front != NULL) {
		data_copy(item, source->front->item);
		peeked = TRUE;
	}
	return peeked;
}

BOOLEAN queue_remove(queue_linked *source, data_ptr *item) {
	BOOLEAN removed = FALSE;
	if (source->front != NULL) {
		*item = source->front->item;
		queue_node *temp = source->front;
		source->front = source->front->next;
		free(temp);
		removed = TRUE;
		source->count -= 1;
		if (source->front == NULL) {
			source->rear = NULL;
		}
	}
	return removed;
}

/**
 * Prints the items in a queue from front to rear.
 * (For testing only).
 *
 * @param source - pointer to a queue
 */
void queue_print(const queue_linked *source) {
	char string[DATA_STRING_SIZE];
	queue_node *current = source->front;

	while (current != NULL) {
		printf("%s\n", data_string(string, sizeof string, current->item));
		current = current->next;
	}
}
