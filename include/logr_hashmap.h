// Written by: Christopher Gholmieh
// Macros:
#define UNSUCCESSFUL_OPERATION -1
#define SUCCESSFUL_OPERATION 1

// Guards:
#ifndef __LOGR_HASHMAP_H__
#define __LOGR_HASHMAP_H__

// Headers:
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    // Keys:
    char** keys;

    // Values:
    void** values;

    // Capacity:
    size_t capacity;

    // Length:
    size_t length;
} logr_hashmap;

// Functions:
logr_hashmap* logr_hashmap_create(void);
uint64_t logr_hashmap_hash(const char* key);


int logr_hashmap_resize(logr_hashmap* hashmap, size_t updated_capacity);
int logr_hashmap_append(logr_hashmap* hashmap, char* key, void* pointer);
void* logr_hashmap_search(logr_hashmap* hashmap, char* key);

int logr_hashmap_destroy(logr_hashmap* hashmap);

// Guard:
#endif /** __LOGR_HASHMAP_H__ */
