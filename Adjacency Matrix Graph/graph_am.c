/**
 * -------------------------------------
 * @file  graph_am.c
 * Adjacency Matrix Graph Code File
 * -------------------------------------
 * @author Filip Stanojcic
 *
 * @version 2024-03-14
 *
 */
#include "graph_am.h"

// Initializes an adjacency matrix graph.
graph_am* graph_am_initialize(int size) {
	graph_am *source = malloc(sizeof *source);
	source->size = size;
	// Initialize values to zeroes.
	source->values = calloc(size * size, sizeof *source->values);
	return source;
}

void graph_am_free(graph_am **source) {
	// Free up the data array.
	free((*source)->values);
	(*source)->values = NULL;
	free(*source);
	*source = NULL;
	return;
}

/**
 * Adds a vertice to source. Fails if pair values invalid or vertice already in source.
 * Pairs containing row < 0 or row >= size are ignored;
 * Pairs containing col < 0 or col >= size are ignored;
 * Pairs containing row == col are a loop and are given a value of 2;
 * All other pairs are given a value of 1 in [row][col] and [col][row].
 *
 * @param source - pointer to a graph
 * @param pair - pointer to vertice to add to source
 * @return - 1 if the vertice was added, 0 otherwise
 */
int graph_am_add_vertice(graph_am *source, const graph_am_pair *pair) {
	int added = 0;

	if (pair->row < 0 || pair->row >= source->size) {
		added = 0;
	} else if (pair->col < 0 || pair->col >= source->size) {
		added = 0;
	} else if (pair->row == pair->col) {
		int index = pair->row * source->size + pair->col;
		source->values[index] = 2;
		added++;
	} else {
		int index1 = pair->row * source->size + pair->col;
		int index2 = pair->col * source->size + pair->row;
		source->values[index1] = 1;
		source->values[index2] = 1;
		added++;
	}

	return added;
}

/**
 * Removes an vertice from source. Fails if pair values invalid or vertice not in source.
 * Pairs containing row < 0 or row >= size are ignored;
 * Pairs containing col < 0 or col >= size are ignored;
 *
 * @param source - pointer to a graph
 * @param pair - pointer to vertice to remove from source
 * @return - 1 if the vertice was removed, 0 otherwise
 */
int graph_am_remove_vertice(graph_am *source, const graph_am_pair *pair) {
	int removed = 0;
	int index = pair->row * source->size + pair->col;
	if (source->values[index] > 0) {
		source->values[index] = 0;
		removed++;
	}

	return removed;
}

/**
 * Creates a graph from an array of vertex pairs.
 * Pairs containing row < 0 or row >= size are ignored;
 * Pairs containing col < 0 or col >= size are ignored;
 * Pairs containing row == col are a loop and is given a value of 2;
 * All other pairs are given a value of 1 in [row][col] and [col][row].
 * The resulting matrix is symmetrical.
 *
 * @param source - pointer to a graph
 * @param pairs - array of vertex pairs
 * @param count - number of values in pairs
 */
graph_am* graph_am_create(int size, const graph_am_pair pairs[], int count) {
	graph_am *source = graph_am_initialize(size);

	for (int i = 0; i < count; i++) {
		if (pairs[i].row < 0 || pairs[i].row >= size) {
			break;
		} else if (pairs[i].col < 0 || pairs[i].col >= size) {
			break;
		} else if (pairs[i].row == pairs[i].col) {
			int index = pairs[i].row * source->size + pairs[i].col;
			source->values[index] = 2;
		} else {
			int index1 = pairs[i].row * source->size + pairs[i].col;
			int index2 = pairs[i].col * source->size + pairs[i].row;
			source->values[index1] = 1;
			source->values[index2] = 1;
		}
	}

	return source;
}

/**
 * Returns an array of all vertices connected to a vertex.
 * vertices must be defined with a size of source->size.
 *
 * @param source - pointer to a graph
 * @param vertex - index of a vertex
 * @param vertices - array of neighbours
 * @param count - number of vertices
 */
void graph_am_neighbours(const graph_am *source, int vertex, int vertices[],
		int *count) {
	*count = 0;
	for (int i = 0; i < source->size; i++) {
		int index = vertex * source->size + i;
		if (source->values[index] > 0) {
			vertices[*count] = i;
			(*count)++;
		}
	}

	return;
}

/**
 * Returns number of vertices connected to vertex.
 *
 * @param source - pointer to a graph
 * @param vertex - index of a vertex
 * @return - number of vertices connected to vertex
 */
int graph_am_degree(const graph_am *source, int vertex) {
	int connected = 0;

	for (int i = 0; i < source->size; i++) {
		int index = vertex * source->size + i;
		connected += source->values[index];
	}
	return connected;
}

/**
 * Performs a breadth-first traversal of a graph. Copies
 *
 * @param source - pointer to a graph
 * @param vertex - index of a vertex to start traversal from
 * @param vertices - array of resulting vertices
 * @param count - number of items in vertices
 */
void graph_am_breadth_traversal(const graph_am *source, int vertex,
		int vertices[], int *count) {

	*count = 0;
	vertices[*count] = vertex;
	(*count)++;
	for (int i = 0; i < source->size; i++) {
		if (i != vertex) {
			vertices[*count] = i;
			(*count)++;
		}
	}

	return;
}

/**
 * Performs a depth-first traversal of a graph.
 *
 * @param source - pointer to a graph
 * @param vertex - index of a vertex to start traversal from
 * @param vertices - array of resulting vertices
 * @param count - number of items in vertices
 */
void graph_am_depth_traversal(const graph_am *source, int vertex,
		int vertices[], int *count) {

	*count = 0;
	vertices[*count] = vertex;
	(*count)++;
	for (int i = source->size - 1; i > -1; i--) {
		int index = vertex * source->size + i;
		if (i != vertex && source->values[index] == 2) {
			vertices[*count] = i;
			(*count)++;
		}
	}
	for (int i = source->size - 1; i > -1; i--) {
		int index = vertex * source->size + i;
		if (i != vertex && source->values[index] == 1) {
			vertices[*count] = i;
			(*count)++;
		}
	}
	for (int i = source->size - 1; i > -1; i--) {
		int index = vertex * source->size + i;
		if (i != vertex && source->values[index] == 0) {
			vertices[*count] = i;
			(*count)++;
		}
	}

	return;
}

// Prints the contents of an adjacency matrix graph.
void graph_am_print(const graph_am *source) {
// Print the column numbers.
	printf("    ");

	for (int i = 0; i < source->size; i++)
		printf("%3d", i);
	printf("\n");
	printf("    ");
	for (int i = 0; i < source->size; i++)
		printf("---");
	printf("\n");

// Print the row numbers and rows.
	for (int i = 0; i < source->size; i++) {
		printf("%3d|", i);

		for (int j = 0; j < source->size; j++) {
			// find item using offsets
			printf("%3d", *(source->values + i * source->size + j));
		}
		printf("\n");
	}
}
