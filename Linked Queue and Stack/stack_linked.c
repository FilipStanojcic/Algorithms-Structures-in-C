/**
 * -------------------------------------
 * @file  stack_linked.c
 * Linked Stack Source Code File
 * -------------------------------------
 * @author Filip Stanojcic
 *
 * @version 2024-02-22
 *
 */
// Includes
#include "stack_linked.h"

// Functions

/**
 * Initializes a stack.
 *
 * @return - pointer to a stack
 */
stack_linked* stack_initialize() {
	// Allocate memory to the stack header
	stack_linked *source = malloc(sizeof *source);
	// Initialize the stack top
	source->top = NULL;
	return source;
}

/**
 * Frees stack memory. Frees all node and data memory.
 *
 * @param source - pointer to a stack
 */
void stack_free(stack_linked **source) {
	// Free the linked data
	while ((*source)->top != NULL) {
		stack_node *temp = (*source)->top;
		// use the data_free function to free the actual data
		data_free(&temp->item);
		// update the stack top
		(*source)->top = (*source)->top->next;
		// free the stack node
		free(temp);
		temp = NULL;
	}
	// Free the stack header
	free(*source);
	*source = NULL;
	return;
}

/**
 * Determines if a stack is empty.
 *
 * @param source - pointer to a stack.
 * @return - TRUE if source is empty, FALSE otherwise
 */
BOOLEAN stack_empty(const stack_linked *source) {
	return (source->top == NULL);
}

/**
 * Pushes a copy of an item onto a stack.
 *
 * @param source - pointer to a stack
 * @param item - pointer to the item to push
 */
void stack_push(stack_linked *source, data_ptr item) {
	// allocate memory to a new stack node
	stack_node *node = malloc(sizeof *node);
	// allocate memoory for the data
	node->item = malloc(sizeof *node->item);
	// copy the data parameter to the new memory
	data_copy(node->item, item);
	// update the stack top
	node->next = source->top;
	source->top = node;
}

/**
 * Returns a copy of the item on the top of a stack, stack is unchanged.
 *
 * @param source - pointer to a stack
 * @param item - pointer to a copy of the retrieved item
 * @return - TRUE if item peeked, FALSE otherwise (stack is empty)
 */
BOOLEAN stack_peek(const stack_linked *source, data_ptr item) {
	BOOLEAN peeked = FALSE;

	if (source->top != NULL) {
		// return a copy of the data in the node
		data_copy(item, source->top->item);
		peeked = TRUE;
	}
	return peeked;
}

/**
 * Removes and returns a pointer to the item on the top of a stack.
 *
 * @param source - pointer to a stack
 * @param item - pointer the item to remove
 * @return - TRUE if item popped, FALSE otherwise (stack is empty)
 */
BOOLEAN stack_pop(stack_linked *source, data_ptr *item) {
	BOOLEAN popped = FALSE;

	if (source->top != NULL) {
		// return a pointer to the node data
		*item = source->top->item;
		stack_node *temp = source->top;
		// update the stack top and free the removed node
		source->top = source->top->next;
		free(temp);
		popped = TRUE;
	}
	return popped;
}

/**
 * Prints the items in a stack from top to bottom. (For testing only).
 *
 * @param source - pointer to a stack
 */
void stack_print(const stack_linked *source) {
	char string[DATA_STRING_SIZE];
	stack_node *current = source->top;

	while (current != NULL) {
		printf("%s\n", data_string(string, sizeof string, current->item));
		current = current->next;
	}
}
