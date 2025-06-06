// Written by: Christopher Gholmieh
// Headers:

// Logger:
#include "logr_logger.h"

// I/O:
#include <stdio.h>

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

