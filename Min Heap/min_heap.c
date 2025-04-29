/**
 * -------------------------------------
 * @file  min_heap.c
 * Minimum Heap Source Code File
 * -------------------------------------
 * @author Filip Stanojcic
 *
 * @version 2024-03-22
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "min_heap.h"

#define STRING_SIZE 80

// local functions

/**
 * Swaps two data values.
 *
 * @param a pointer to data.
 * @param b pointer to data.
 */
static void heap_swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
	return;
}

/**
 * Moves the last value in source until it is in its correct location
 * in source.
 *
 * @param source - pointer to a heap
 */
static void heapify_up(min_heap *source) {
	int i = source->count - 1;
	int val = source->values[i];
	while (i > 0) {
		int pi = (i - 1) >> 1;
		if (source->values[pi] <= val) {
			break;
		} else {
			heap_swap(&source->values[i], &source->values[pi]);
			i = pi;
		}
	}
	return;
}

/**
 * Moves a value down source to its correct position.
 *
 * @param source - pointer to a heap
 */
static void heapify_down(min_heap *source) {
	int i = 0;
	int val = source->values[i];
	int ci = (i << 1) + 1;
	int n = source->count;
	while (ci <= n) {
		if ((ci < n) && (source->values[ci] > source->values[ci + 1])) {
			ci++;
		}
		if (source->values[ci] >= val) {
			break;
		} else {
			heap_swap(&source->values[i], &source->values[ci]);
			i = ci;
			ci = (i << 1) + 1;
		}
	}
	return;
}

// Public minimum heap functions

min_heap* min_heap_initialize(int capacity) {
	min_heap *source = malloc(sizeof *source);
	source->values = malloc(capacity * sizeof *source->values);
	source->capacity = capacity;
	source->count = 0;
	return source;
}

void min_heap_free(min_heap **source) {
	free((*source)->values);
	free(*source);
	(*source)->values = NULL;
	(*source)->count = 0;
	(*source)->capacity = 0;
	*source = NULL;
	return;
}

void min_heap_heapify(min_heap *source, int *keys, int count) {
	for (int i = 0; i < count; i++) {
		min_heap_insert(source, keys[i]);
	}
	return;
}

int min_heap_empty(const min_heap *source) {
	return (source->count == 0);
}

int min_heap_full(const min_heap *source) {
	return (source->count == source->capacity);
}

int min_heap_count(const min_heap *source) {
	return (source->count);
}

void min_heap_insert(min_heap *source, const int value) {
	// Add new value to end of the heap.
	source->values[source->count] = value;
	source->count++;
	// Fix the heap.
	heapify_up(source);
	return;
}

int min_heap_peek(const min_heap *source) {
	return (source->values[0]);
}

int min_heap_remove(min_heap *source) {
	int value = source->values[0];
	source->count--;

	if (source->count > 0) {
		// Move last value to top of heap.
		source->values[0] = source->values[source->count];
		// Fix the heap.
		heapify_down(source);
	}
	return value;
}

/**
 * Determines if a heap is valid: i.e. all values are >= parent
 * values.
 *
 * @param source - pointer to heap
 * @return - 1 if heap is valid, 0 otherwise
 */
int min_heap_valid(const min_heap *source) {
	int i = 0;
	int valid = 1;
	while (i <= (source->count - 1) / 2) {
		if (i * 2 + 1 <= source->count - 1
				&& source->values[i] > source->values[i * 2 + 1]) {
			valid = 0;
		} else if (i * 2 + 2 <= source->count - 1
				&& source->values[i] > source->values[i * 2 + 2]) {
			valid = 0;
		}
		i++;
	}
	return valid;
}

int min_heap_replace(min_heap *source, int replacement) {
	int value = source->values[0];
	source->values[0] = replacement;
	heapify_down(source);
	return value;
}

int compare(const void *a, const void *b) {
	return (*(int*) a - *(int*) b);
}

/**
 * Sorts an array of integers.
 *
 * @param values - array of values to sort
 * @param count - number of values in values
 */
void heap_sort(int *values, int count) {
	qsort(values, count, sizeof(int), compare);
	return;
}

// for testing
void min_heap_print(const min_heap *source) {
	printf("{");
	for (int i = 0; i < source->count; i++) {
		printf("%d, ", source->values[i]);
	}
	printf("}\n");
	return;
}
