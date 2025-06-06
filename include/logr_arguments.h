// Written by: Christopher Gholmieh
// Guards:
#ifndef __LOGR_ARGUMENTS_H__
#define __LOGR_ARGUMENTS_H__

// Headers:
#include <stdlib.h>

// Functions:
void* allocate_memory(size_t size);

char** parse_argument_vector(int argument_count, char* argument_vector[]);


// Guard:
#endif /** __LOGR_ARGUMENTS_H__ */
