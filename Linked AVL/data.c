/**
 * -------------------------------------
 * @file  data.c
 * Data Type Source Code File
 * -------------------------------------
 * @author Filip Stanojcic
 *
 * @version 2024-02-22
 *
 */
// Includes
#include <stdio.h>

#include "data.h"

// Functions

char* data_string(char *string, size_t size, data_ptr data) {
    snprintf(string, size, "%d", *data);
    return string;
}

void data_copy(data_ptr target, data_ptr source) {
    *target = *source;
}

int data_compare(data_ptr source, data_ptr target) {
    int result = 0;

    if(*source < *target) {
        result = -1;
    } else if(*source > *target) {
        result = 1;
    }
    return (result);
}

void data_free(data_ptr *source) {
    free(*source);
    *source = NULL;
    return;
}
