/*
 * banker.c
 *
 *  Created on: Nov. 20, 2024
 *      Author: fstan
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int customers = 0;
int resource_count = 0;
int available[100];
int maximum[100][100];
int allocation[100][100];
int need[100][100];

void calculateNeed() {
	for (int i = 0; i < customers; i++) {
		for (int j = 0; j < resource_count; j++) {
			need[i][j] = maximum[i][j] - allocation[i][j];
		}
	}
}

int isSafe(int safeSequence[]) {
    int work[resource_count];
    int finish[customers];
    int index = 0;
    for (int i = 0; i < resource_count; i++) {
        work[i] = available[i];
    }
    for (int i = 0; i < customers; i++) {
        finish[i] = 0;
    }
    while (1) {
        int found = 0;
        for (int i = 0; i < customers; i++) {
            if (!finish[i]) {
                int canProceed = 1;
                for (int j = 0; j < resource_count; j++) {
                    if (need[i][j] > work[j]) {
                        canProceed = 0;
                        break;
                    }
                }
                if (canProceed) {
                    for (int j = 0; j < resource_count; j++) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = 1;
                    safeSequence[index++] = i;
                    found = 1;
                }
            }
        }
        if (!found) {
            break;
        }
    }
    for (int i = 0; i < customers; i++) {
        if (!finish[i]) {
            return 0;
        }
    }
    return 1;
}

int requestResources(int customer_id, int request[]) {
	int safeSequence[customers];
	for (int i = 0; i < resource_count; i++) {
		if (request[i] > need[customer_id][i]) {
			printf("Error: Request exceeds maxiumum need.\n");
			return 0;
		}
	}
	for (int i = 0; i < resource_count; i++) {
		if (request[i] > available[i]) {
			printf("Error: Resources not available.\n");
			return 0;
		}
	}
	for (int i = 0; i < resource_count; i++) {
		available[i] -= request[i];
		allocation[customer_id][i] += request[i];
		need[customer_id][i] -= request[i];
	}
	if (isSafe(safeSequence)) {
		printf("State is safe, and request is satisfied.\n");
		return 1;
	} else {
		for (int i = 0; i < resource_count; i++) {
			available[i] += request[i];
			allocation[customer_id][i] -= request[i];
			need[customer_id][i] += request[i];
		}
		printf("State is unsafe, and request is denied.\n");
		return 0;
	}
}

void releaseResources(int customer_id, int release[]) {
	for (int i = 0; i < resource_count; i++) {
		if (release[i] > allocation[customer_id][i]) {
			printf("Error: Cannot release more resources than allocated.\n");
			return;
		}
	}
	for (int i = 0; i < resource_count; i++) {
		allocation[customer_id][i] -= release[i];
		available[i] += release[i];
	}
	calculateNeed();
	printf("The resources have been released successfully.\n");
}

void runCommand() {
    int safeSequence[customers];
    if (!isSafe(safeSequence)) {
        printf("System is not in a safe state. Cannot execute Run command.\n");
        return;
    }
    printf("Safe Sequence is: ");
    for (int i = 0; i < customers; i++) {
        printf("%d ", safeSequence[i]);
    }
    printf("\n");
    for (int i = 0; i < customers; i++) {
        int customer_id = safeSequence[i];
        printf("--> Customer/Thread %d\n", customer_id);
        printf("       Allocated resources: ");
        for (int j = 0; j < resource_count; j++) {
            printf("%d ", allocation[customer_id][j]);
        }
        printf("\n       Needed: ");
        for (int j = 0; j < resource_count; j++) {
            printf("%d ", need[customer_id][j]);
        }
        printf("\n       Available: ");
        for (int j = 0; j < resource_count; j++) {
            printf("%d ", available[j]);
        }
        printf("\n       Thread has started\n");
        printf("       Thread has finished\n");
        printf("       Thread is releasing resources\n");
        for (int j = 0; j < resource_count; j++) {
            available[j] += allocation[customer_id][j];
            allocation[customer_id][j] = 0;
            need[customer_id][j] = 0;
        }
        printf("       New Available: ");
        for (int j = 0; j < resource_count; j++) {
            printf("%d ", available[j]);
        }
        printf("\n");
    }
}


void printStatus() {
	printf("Available Resources:\n");
	for (int i = 0; i < resource_count; i++) {
		printf("%d ", available[i]);
	}
	printf("\n");
	printf("Maximum Resources:\n");
	for (int i = 0; i < customers; i++) {
	    for (int j = 0; j < resource_count; j++) {
	        printf("%d ", maximum[i][j]);
        }
        printf("\n");
	}
	printf("Allocated Resources:\n");
	for (int i = 0; i < customers; i++) {
		for (int j = 0; j < resource_count; j++) {
			printf("%d ", allocation[i][j]);
	    }
		printf("\n");
	}
	printf("Need Resources:\n");
	for (int i = 0; i < customers; i++) {
		for (int j = 0; j < resource_count; j++) {
	        printf("%d ", need[i][j]);
	    }
	    printf("\n");
	}
}

int main(int argc, char *argv[]) {
	char *inputFile = "sample_in_banker.txt";
	if (access(inputFile, F_OK) == -1) {
		printf("File does not exist.\n");
	    return 1;
	}
	FILE *fd = fopen(inputFile, "r");
	if (fd == NULL) {
		perror("Error opening file.\n");
	    return 1;
	}
	char line[256];
	while (fgets(line, sizeof(line), fd) != NULL) {
		customers++;
	}
	printf("Number of Customers: %d\n", customers);
	rewind(fd);
	resource_count = argc - 1;
	printf("Currently Available resources: ");
	for (int i = 1; i < argc; i++) {
		available[i - 1] = atoi(argv[i]);
		printf("%d ", available[i - 1]);
	}
	printf("\n");
	for (int i = 0; i < customers; i++) {
		for (int j = 0; j < resource_count; j++) {
			fscanf(fd, "%d,", &maximum[i][j]);
	        allocation[i][j] = 0;
	    }
	}
	fclose(fd);
	printf("Maximum resources from file:\n");
	for (int i = 0; i < customers; i++) {
		for (int j = 0; j < resource_count; j++) {
			printf("%d ", maximum[i][j]);
	    }
	    printf("\n");
	}
	calculateNeed();
	char input[256];

	while (1) {
		printf("Enter Command: ");
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = 0;
		if (strcmp(input, "Exit") == 0) {
			break;
		}
		if (strncmp(input, "RQ", 2) == 0) {
			int customer_id;
			int request[resource_count];
			int scanned = sscanf(input, "RQ %d %d %d %d %d", &customer_id, &request[0], &request[1], &request[2], &request[3]);
			if (scanned < 5) {
				printf("Invalid input for RQ, try again.\n");
			} else {
				requestResources(customer_id, request);
			}
		} else if (strncmp(input, "RL", 2) == 0) {
			int customer_id;
			int release[resource_count];
			int scanned = sscanf(input, "RL %d %d %d %d %d", &customer_id, &release[0], &release[1], &release[2], &release[3]);
			if (scanned < 5) {
				printf("Invalid input for RL, try again.\n");
			} else {
				releaseResources(customer_id, release);
			}
		} else if (strcmp(input, "Status") == 0) {
			printStatus();
		} else if (strcmp(input, "Run") == 0) {
			runCommand();
		} else {
			printf("Invalid input, use one of RQ, RL, Status, Run, Exit.\n");
		}
	}
	return 0;
}
