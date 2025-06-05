// Written by: Christopher Gholmieh
// Macros:

#define MAXIMUM_BUFFER_CAPACITY 4096
#define MAXIMUM_ARGUMENT_CAPACITY 5

// Headers:
#include <sys/inotify.h>
#include <unistd.h>

#include <sys/poll.h>
#include <poll.h>

#include <stdlib.h>
#include <stdio.h>

#include <signal.h>
#include <string.h>

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
void logr_log_information(char* message) {
    // Variables (Assignment):
    // Buffer:
    char buffer[512];

    // Logic:
    snprintf(buffer, sizeof(buffer), "\033[1;32m[LOGR] [INFO]: \033[0m%s\n", message);

    fprintf(stdout, "%s", buffer);
}

void logr_log_warning(char* message) {
    // Variables (Assignment):
    // Buffer:
    char buffer[512];

    // Logic:
    snprintf(buffer, sizeof(buffer), "\033[1;33m[LOGR] [WARNING]: \033[0m%s\n", message);

    fprintf(stderr, "%s", buffer);
}

void logr_log_error(char* message) {
    // Variables (Assignment):
    // Buffer:
    char buffer[512];

    // Logic:
    snprintf(buffer, sizeof(buffer), "\033[1;31m[LOGR] [ERROR]: \033[0m%s\n", message);

    fprintf(stderr, "%s", buffer);
}

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
    for (size_t iterator = 1; iterator < argument_count; iterator++) {
        if (access(argument_vector[iterator], F_OK) == 0) {
            parsed_argument_vector[argument_vector_length] = argument_vector[iterator];

            argument_vector_length++;
        }
    }

    parsed_argument_vector[argument_vector_length] = NULL;

    // Logic:
    return parsed_argument_vector;
}

logr_information apply_modifier_monitors(int file_descriptor, char** parsed_argument_vector) {
    // Variables (Assignment):
    // Descriptors:
    int watch_descriptors[MAXIMUM_ARGUMENT_CAPACITY];

    // Files:
    char* watch_files[MAXIMUM_ARGUMENT_CAPACITY];

    // Length:
    size_t watch_descriptors_length = 0;
    
    // Logic:
    for (size_t iterator = 0; iterator < MAXIMUM_ARGUMENT_CAPACITY; iterator++) {
        if (parsed_argument_vector[iterator] == NULL) {
            continue;
        }

        // Variables (Assignment):
        // Descriptor:
        int descriptor = inotify_add_watch(file_descriptor, parsed_argument_vector[iterator], IN_MODIFY | IN_DELETE | IN_ATTRIB);

        if (descriptor < 0) {
            fprintf(stderr, "[!] Unable to create a monitor for file %s!\n", parsed_argument_vector[iterator]);

            exit(EXIT_FAILURE);
        }

        watch_descriptors[watch_descriptors_length] = descriptor;
        watch_files[watch_descriptors_length] = parsed_argument_vector[iterator];
        watch_descriptors_length++;
    }

    // Variables (Assignment):
    // Information:
    logr_information information;

    // Arrays:
    for (size_t iterator = 0; iterator < watch_descriptors_length; iterator++) {
        information.watch_descriptors[iterator] = watch_descriptors[iterator];
        information.watch_files[iterator] = watch_files[iterator];
    }

    // Length:
    information.watch_descriptors_length = watch_descriptors_length;

    // Logic:
    return information;
}

void initiate_logr_process(int file_descriptor, logr_information information) {
    // Variables (Assignment):
    // Buffer:
    char buffer[MAXIMUM_BUFFER_CAPACITY];

    // Poll:
    struct pollfd poll_descriptor;
    poll_descriptor.events = POLL_IN;
    poll_descriptor.fd = file_descriptor;

    // Logic:
    while (1) {
        // Variables (Assignment):
        // Result:
        int poll_result = poll(&poll_descriptor, 1, -1);
        
        if (poll_result < 0) {
            fprintf(stderr, "[!] Error reading poll result.\n");

            continue;
        }

        // Logic:
        if (poll_descriptor.revents & POLLIN) {
            // Variables (Assignment):
            // Length:
            ssize_t length = read(file_descriptor, buffer, MAXIMUM_BUFFER_CAPACITY);

            // Logic:
            if (length < 0) {
                usleep(100000);

                continue;
            }

            for (char* pointer = buffer; pointer < buffer + length; ) {
                // Variables (Assignment):
                // Event:
                struct inotify_event* notify_event = (struct inotify_event*) pointer;
                
                // Filename:
                const char* filename = NULL;

                // Logic:
                for (size_t iterator = 0; iterator < information.watch_descriptors_length; iterator++) {
                    if (information.watch_descriptors[iterator] == notify_event->wd) {
                        filename = information.watch_files[iterator];

                        break;
                    }
                }

                if (notify_event->mask & IN_MODIFY) {
                    // Variables (Assignment):
                    // Buffer:
                    char buffer[512];

                    // Logic:
                    snprintf(buffer, sizeof(buffer), "[*] File's contents modified: %s", filename);

                    logr_log_information(buffer);
                }

                if (notify_event-> mask & IN_ATTRIB) {
                    // Variables (Assignment):
                    // Buffer:
                    char buffer[512];

                    // Logic:
                    snprintf(buffer, sizeof(buffer), "[*] File's attributes modified (ownership/permissions/deletion): %s", filename);

                    logr_log_information(buffer);
                }

                pointer += sizeof(struct inotify_event) + notify_event->len;
            }
        }
    }
}

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
