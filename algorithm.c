/*
 -------------------------------------
 File:    algorithm.c
 Project: a10
 file description
 -------------------------------------
 Author:  Filip Stanojcic
 ID:      169035905
 Email:   stan5905@mylaurier.ca
 Version  2024-03-26
 -------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "algorithm.h"

/**
 * Compute and return MST by Prim's algorithm using priority queue (min-heap)
 * @param g     - graph by reference
 * @param start - the root node of MST
 * @return      - pointer of edge list of shortest path tree
 */
EDGELIST* mst_prim(GRAPH *g, int start) {
	if (g == NULL)
		return NULL;

	int i, heapindex, n = g->order, T[n], parent[n];
	for (i = 0; i < n; i++) {
		T[i] = 0; // T[] represents nodes in the current tree
		parent[i] = -1; // parent[i] represents the parent of i
	}

	HEAP *h = new_heap(4); // Heap for finding the minimum weight edge
	T[start] = 1; // Set the first node of the current tree T from start

	// For each neighbor, add the corresponding heap node into the heap
	ADJNODE *temp = g->nodes[start]->neighbor;
	while (temp) {
		HEAPDATA hn;
		hn.key = temp->weight;
		hn.value = temp->nid;
		heap_insert(h, hn);
		parent[temp->nid] = start;
		temp = temp->next;
	}

	// Create EDGELIST object to hold MST
	EDGELIST *mst = new_edgelist();

	// The main loop of Prim's algorithm
	while (h->size > 0) {
		HEAPDATA hn = heap_extract_min(h); // Get the minimum node
		i = hn.value;
		T[i] = 1; // Add i to the current tree
		insert_edge_end(mst, parent[i], i, hn.key); // Add to MST

		// Update the keys of neighbors of the newly added node
		temp = g->nodes[i]->neighbor;
		while (temp) {
			heapindex = heap_search_data(h, temp->nid);
			if (heapindex >= 0) {
				if (T[temp->nid] == 0 && temp->weight < h->hda[heapindex].key) {
					heap_change_key(h, heapindex, temp->weight);
					parent[temp->nid] = i;
				}
			} else {
				if (T[temp->nid] == 0) {
					HEAPDATA hn;
					hn.key = temp->weight;
					hn.value = temp->nid;
					heap_insert(h, hn);
					parent[temp->nid] = i;
				}
			}
			temp = temp->next;
		}
	}

	return mst;
}

/**
 * Compute shortest path tree as edge list by Dijkstra's algorithm using priority queue (min-heap)
 * @param g     - graph by reference
 * @param start - the root node of shortest path tree
 * @return      - pointer of edge list of shortest path tree
 */
EDGELIST* spt_dijkstra(GRAPH *g, int start) {
	if (!g)
		return NULL;
	EDGELIST *spt = new_edgelist();
	int i, heapindex, u, n = g->order;
	int T[n], parent[n], label[n];
	HEAPDATA hn;

	for (i = 0; i < n; i++) {
		T[i] = 0;
		label[i] = INFINITY;
		parent[i] = -1;
	}

	HEAP *h = new_heap(4);
	label[start] = 0;
	hn.key = 0;
	hn.value = start;
	heap_insert(h, hn);
	while (h->size > 0) {
		hn = heap_extract_min(h);
		u = hn.value;
		if (T[u])
			continue;
		T[u] = 1;
		if (parent[u] != -1) {
			insert_edge_end(spt, parent[u], u, label[u] - label[parent[u]]);
		}
		ADJNODE *temp = g->nodes[u]->neighbor;
		while (temp) {
			if (!T[temp->nid] && label[u] + temp->weight < label[temp->nid]) {
				label[temp->nid] = label[u] + temp->weight;
				parent[temp->nid] = u;
				heapindex = heap_search_data(h, temp->nid);
				if (heapindex >= 0) {
					heap_change_key(h, heapindex, label[temp->nid]);
				} else {
					HEAPDATA hnNew;
					hnNew.key = label[temp->nid];
					hnNew.value = temp->nid;
					heap_insert(h, hnNew);
				}
			}
			temp = temp->next;
		}
	}

	return spt;
}

/**
 * Compute shortest path as edge list by Dijkstra's algorithm using priority queue (min-heap)
 * @param g     - graph by reference
 * @param start - the start node of shortest path
 * @param end   - the end node of shortest path
 * @return      - pointer of edge list of shortest path
 */
EDGELIST* sp_dijkstra(GRAPH *g, int start, int end) {
	if (!g)
		return NULL;

	EDGELIST *sp = new_edgelist();
	int i, heapindex, u, n = g->order;
	int T[n], parent[n], label[n];

	for (i = 0; i < n; i++) {
		T[i] = 0;
		label[i] = INFINITY;
		parent[i] = -1;
	}

	HEAP *h = new_heap(4);
	label[start] = 0;
	HEAPDATA hn;
	hn.key = 0;
	hn.value = start;
	heap_insert(h, hn);

	while (h->size > 0) {
		hn = heap_extract_min(h);
		u = hn.value;
		if (u == end)
			break;
		if (T[u])
			continue;
		T[u] = 1;
		ADJNODE *temp = g->nodes[u]->neighbor;
		while (temp) {
			if (!T[temp->nid] && label[u] + temp->weight < label[temp->nid]) {
				label[temp->nid] = label[u] + temp->weight;
				parent[temp->nid] = u;
				heapindex = heap_search_data(h, temp->nid);
				if (heapindex >= 0) {
					heap_change_key(h, heapindex, label[temp->nid]);
				} else {
					HEAPDATA hnNew;
					hnNew.key = label[temp->nid];
					hnNew.value = temp->nid;
					heap_insert(h, hnNew);
				}
			}
			temp = temp->next;
		}
	}

	i = end;
	while (1) {
		if (i == start)
			break;
		insert_edge_start(sp, parent[i], i, label[i] - label[parent[i]]);
		i = parent[i];
	}

	return sp;
}
