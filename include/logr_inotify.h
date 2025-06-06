// Written by: Christopher Gholmieh
// Macros:
#define MAXIMUM_BUFFER_CAPACITY 4096
#define MAXIMUM_ARGUMENT_CAPACITY 5

// Guards:
#ifndef __LOGR_INOTIFY_H__
#define __LOGR_INOTIFY_H__

// Headers:
#include <stdlib.h>

// Structures:
typedef struct {
    // Descriptors:
    int watch_descriptors[MAXIMUM_ARGUMENT_CAPACITY];

    // Files:
    char* watch_files[MAXIMUM_ARGUMENT_CAPACITY];

    // Length:
    size_t watch_descriptors_length;
} logr_information;

// Functions:
logr_information apply_modifier_monitors(int file_descriptor, char** parsed_argument_vector);
void initiate_logr_process(int file_descriptor, logr_information information); 

// Guard:
#endif /** __LOGR_INOTIFY_H__ */
