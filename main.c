// Written by: Christopher Gholmieh
// Macros:
#define MAXIMUM_BUFFER_CAPACITY 4096
#define MAXIMUM_ARGUMENT_CAPACITY 5

// Headers:
#include <sys/inotify.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>

#include "logr_arguments.h"
#include "logr_inotify.h"
#include "logr_hashmap.h"
#include "logr_logger.h"

#include <string.h>

// Main:
int main(int argument_count, char* argument_vector[]) {
    // Validation:
    if ((argument_count - 1) > MAXIMUM_ARGUMENT_CAPACITY) {
        fprintf(stderr, "[!] Only a maximum of %d arguments can be provided.\n", MAXIMUM_ARGUMENT_CAPACITY);

        return 1;
    }

    // Variables (Assignment):
    // Vector:
    char** parsed_argument_vector = parse_argument_vector(argument_count, argument_vector);

    // Length:
    size_t argument_vector_length = 0;

    for (size_t iterator = 0; iterator < MAXIMUM_ARGUMENT_CAPACITY; iterator++) {
        if (parsed_argument_vector[iterator] == NULL) {
            continue;
        }

        argument_vector_length++;
    }

    if (argument_vector_length == 0) {
        fprintf(stderr, "[!] None of the files passed as command-line arguments were valid.\n");

        return 1;
    }

    // Descriptor:
    const int file_descriptor = inotify_init1(IN_NONBLOCK);

    if (file_descriptor < 0) {
        fprintf(stderr, "[!] Error upon initializing the inotify file descriptor.\n");

        return 1;
    }

    // Variables (Assignment):
    // Information:
    logr_information information = apply_modifier_monitors(file_descriptor, parsed_argument_vector);

    // Logic:
    initiate_logr_process(file_descriptor, information);

    return 0;
}
