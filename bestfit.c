/*
 * bestfit.c
 *
 *  Created on: Nov. 20, 2024
 *      Author: fstan
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int start;
	int size;
} Hole;

typedef struct {
	int process_id;
	int start;
	int size;
} Allocation;

#define MEMORY_SIZE 1048576

Hole holes[100];
int hole_count = 1;
Allocation allocations[100];
int allocation_count = 0;

void initialize_memory() {
	holes[0].start = 0;
	holes[0].size = MEMORY_SIZE;
}

void request_memory(int process_id, int size) {
	int best_fit_index = -1;
	int best_fit_size = MEMORY_SIZE + 1;
	for (int i = 0; i < hole_count; i++) {
		if (holes[i].size >= size && holes[i].size < best_fit_size) {
			best_fit_index = i;
	        best_fit_size = holes[i].size;
	    }
	}
	if (best_fit_index == -1) {
	    printf("No hole of sufficient size\n");
	    return;
	}
	Allocation new_allocation = {
		.process_id = process_id,
	    .start = holes[best_fit_index].start,
	    .size = size
	};
	allocations[allocation_count++] = new_allocation;
	holes[best_fit_index].start += size;
	holes[best_fit_index].size -= size;
	if (holes[best_fit_index].size == 0) {
		for (int i = best_fit_index; i < hole_count - 1; i++) {
	        holes[i] = holes[i + 1];
	    }
	    hole_count--;
	}
	printf("Successfully allocated %d to process P%d\n", size, process_id);
}

void release_memory(int process_id) {
	int allocation_index = -1;
	for (int i = 0; i < allocation_count; i++) {
	    if (allocations[i].process_id == process_id) {
	        allocation_index = i;
            break;
	    }
	}
	if (allocation_index == -1) {
		printf("Error: Process P%d not found\n", process_id);
	    return;
	}
	printf("Releasing memory for process P0\n");
	Hole new_hole = {
		.start = allocations[allocation_index].start,
	    .size = allocations[allocation_index].size
	};
	holes[hole_count++] = new_hole;
	for (int i = allocation_index; i < allocation_count - 1; i++) {
	    allocations[i] = allocations[i + 1];
	}
	allocation_count--;
	for (int i = 0; i < hole_count - 1; i++) {
	    for (int j = i + 1; j < hole_count; j++) {
	        if (holes[i].start + holes[i].size == holes[j].start) {
	            holes[i].size += holes[j].size;
	            for (int k = j; k < hole_count - 1; k++) {
	                holes[k] = holes[k + 1];
	            }
	            hole_count--;
	            j--;
	        } else if (holes[j].start + holes[j].size == holes[i].start) {
	            holes[j].size += holes[i].size;
	            holes[j].start = holes[i].start;
	            for (int k = i; k < hole_count - 1; k++) {
	                holes[k] = holes[k + 1];
	            }
	            hole_count--;
	            i--;
	        }
	    }
	}
	printf("Successfully released memory for process P%d\n", process_id);
}

void print_status() {
    int total_allocated = 0, total_free = 0;
    printf("Partitions [Allocated memory = ");
    for (int i = 0; i < allocation_count; i++) {
        total_allocated += allocations[i].size;
    }
    printf("%d]:\n", total_allocated);
    for (int i = 0; i < allocation_count; i++) {
        printf("Address [%d:%d] Process P%d\n",
               allocations[i].start,
               allocations[i].start + allocations[i].size - 1,
               allocations[i].process_id);
    }
    if (allocation_count == 0) {
        printf("No allocated memory.\n");
    }
    printf("\nHoles [Free memory = ");
    for (int i = 0; i < hole_count; i++) {
        total_free += holes[i].size;
    }
    printf("%d]:\n", total_free);
    for (int i = 0; i < hole_count; i++) {
        printf("Address [%d:%d] len = %d\n",
               holes[i].start,
               holes[i].start + holes[i].size - 1,
               holes[i].size);
    }
    if (hole_count == 0) {
        printf("No available memory holes.\n");
    }
}

void compact_memory() {
	int current_address = 0;
	for (int i = 0; i < allocation_count; i++) {
		allocations[i].start = current_address;
		current_address += allocations[i].size;
	}
	hole_count = 1;
	holes[0].start = current_address;
	holes[0].size = MEMORY_SIZE - current_address;
	printf("Compaction process is successful\n");
}

int main(int argc, char *argv[]) {
	initialize_memory();
	printf("Here, the Best Fit approach has been implemented and the allocated %s bytes of memory.\n", argv[1]);
	char input[256];
	while (1) {
		printf("allocator>");
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = 0;
		if (strcmp(input, "Exit") == 0) {
			break;
		}
		if (strncmp(input, "RQ", 2) == 0) {
			int process_id;
			int size;
			char algorithm_check;
			int scanned = sscanf(input, "RQ P%d %d %c", &process_id, &size, &algorithm_check);
			if (scanned < 3) {
				printf("Invalid input for RQ, try again.\n");
			} else if (algorithm_check != 'B') {
				printf("Invalid algorithm for RQ, try again.\n");
			} else {
				request_memory(process_id, size);
			}
		} else if (strncmp(input, "RL", 2) == 0) {
			int process_id;
			int scanned = sscanf(input, "RL P%d", &process_id);
			if (scanned < 1) {
				printf("Invalid input for RL, try again.\n");
			} else {
				release_memory(process_id);
			}
		} else if (strcmp(input, "Status") == 0) {
			print_status();
		} else if (strcmp(input, "C") == 0) {
			compact_memory();
		} else {
			printf("Invalid input, use one of RQ, RL, Status, C, Exit.\n");
		}
	}
	return 0;
}
