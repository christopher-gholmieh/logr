// Written by: Christopher Gholmieh
// Macros:
#define MAXIMUM_ARGUMENT_CAPACITY 5

// Headers:
#include "logr_arguments.h"
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>

// Functions:
void* allocate_memory(size_t size) {
    // Variables (Assignment):
    // Pointer:
    void* pointer = malloc(size);

    // Logic:
    if (pointer == NULL) {
        fprintf(stderr, "[!] Error allocating memory.\n");

        exit(EXIT_FAILURE);
    }

    return pointer;
}

char** parse_argument_vector(int argument_count, char* argument_vector[]) {
    // Variables (Assignment):
    // Output:
    char** parsed_argument_vector = (char**) allocate_memory(sizeof(char*) * (MAXIMUM_ARGUMENT_CAPACITY + 1));

    // Length:
    size_t argument_vector_length = 0;

    // Logic:
    for (int iterator = 1; iterator < argument_count; iterator++) {
        if (access(argument_vector[iterator], F_OK) == 0) {
            parsed_argument_vector[argument_vector_length] = argument_vector[iterator];

            argument_vector_length++;
        }
    }

    parsed_argument_vector[argument_vector_length] = NULL;

    // Logic:
    return parsed_argument_vector;
}
