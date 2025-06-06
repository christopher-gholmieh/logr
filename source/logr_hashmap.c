// Written by: Christopher Gholmieh
// Headers:
#include "logr_hashmap.h"
#include "logr_logger.h"

#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <stdint.h>

// Functions:
logr_hashmap* logr_hashmap_create(void) {
    // Variables (Assignment):
    // Hashmap:
    logr_hashmap* hashmap = (logr_hashmap*) malloc(sizeof(logr_hashmap));

    if (hashmap == NULL) {
        logr_log_error("[!] Unable to allocate memory for a hashmap.");

        return NULL;
    }

    // Capacity:
    size_t capacity = 4;

    // Keys:
    char** keys = (char**) malloc(sizeof(char*) * capacity);

    if (keys == NULL) {
        // Error:
        logr_log_error("[!] Unable to allocate memory for keys for hashmap.");

        // Logic:
        free(hashmap);

        return NULL;
    }

    // Values:
    void** values = (void**) malloc(sizeof(void*) * capacity);

    if (values == NULL) {
        // Error:
        logr_log_error("[!] Unable to allocate memory for values for hashmap.");

        // Logic:
        free(hashmap);
        free(keys);

        return NULL;
    }

    // Logic:
    hashmap->values = values;
    hashmap->keys = keys;

    hashmap->capacity = capacity;
    hashmap->length = 0;

    return hashmap;
}

uint64_t logr_hashmap_hash(const char* key) {
    // Variables (Assignment):
    // Prime:
    uint64_t fnv_prime = 1099511628211ULL;

    // Hash:
    uint64_t hash = 14695981039346656037ULL;
    
    // Logic:
    while (*key != '\0') {
        hash = hash ^ *key;
        hash = hash * fnv_prime;

        key++;
    }

    return hash;
}

int logr_hashmap_resize(logr_hashmap *hashmap, size_t updated_capacity) {
    // Variables (Assignment):
    // Keys:
    char** updated_keys = (char**) malloc(sizeof(char*) * updated_capacity);

    if (updated_keys == NULL) {
        // Error:
        logr_log_error("[!] Couldn't allocate memory block for updated keys.");

        // Logic:
        return UNSUCCESSFUL_OPERATION;
    }

    memset(updated_keys, 0, sizeof(char*) * updated_capacity);

    // Values:
    void** updated_values = (void**) malloc(sizeof(void*) * updated_capacity);

    if (updated_values == NULL) {
        // Error:
        logr_log_error("[!] Couldn't allocate memory block for updated values.");

        // Logic:
        free(updated_keys);

        return UNSUCCESSFUL_OPERATION;
    }

    memset(updated_values, 0, sizeof(void*) * updated_capacity);

    // Logic:
    for (size_t iterator = 0; iterator < hashmap->capacity; iterator++) {
        if (hashmap->keys[iterator] != NULL) {
            // Variables (Assignment):
            // Key:
            char* key = hashmap->keys[iterator];

            // Pointer:
            void* pointer = hashmap->values[iterator];

            // Hash:
            uint64_t hash = logr_hashmap_hash(key);

            // Bucket:
            size_t bucket = hash & (updated_capacity - 1);

            // Logic:
            while (updated_keys[bucket] != NULL) {
                bucket = (bucket + 1) & (updated_capacity - 1);
            }

            updated_keys[bucket] = key;
            updated_values[bucket] = pointer;
        }
    }

    free(hashmap->values);
    free(hashmap->keys);

    hashmap->capacity = updated_capacity;
    hashmap->values = updated_values;
    hashmap->keys = updated_keys;

    return SUCCESSFUL_OPERATION;
}

int logr_hashmap_append(logr_hashmap* hashmap, char* key, void* pointer) {
    // Resize:
    if (hashmap->length >= hashmap->capacity * 0.75) {
        if (logr_hashmap_resize(hashmap, hashmap->capacity * 2) != SUCCESSFUL_OPERATION) {
            return UNSUCCESSFUL_OPERATION;
        }
    }

    // Variables (Assignment):
    // Hash:
    uint64_t hash = logr_hashmap_hash(key);

    // Bucket:
    size_t bucket = hash & (hashmap->capacity - 1);

    // Key:
    char* duplicated_key = strdup(key);

    if (duplicated_key == NULL) {
        logr_log_error("[!] Unable to duplicate key for bucket.");

        return UNSUCCESSFUL_OPERATION;
    }

    while (hashmap->keys[bucket] != NULL) {
        if (strcmp(hashmap->keys[bucket], key) == 0) {
            hashmap->values[bucket] = pointer;

            return SUCCESSFUL_OPERATION;
        }

        bucket = (bucket + 1) & (hashmap->capacity - 1);
    }

    hashmap->keys[bucket] = duplicated_key;
    hashmap->values[bucket] = pointer;
    hashmap->length++;

    return SUCCESSFUL_OPERATION;
}

void* logr_hashmap_search(logr_hashmap* hashmap, char* key) {
    // Variables (Assignment):
    // Hash:
    uint64_t hash = logr_hashmap_hash(key);

    // Bucket:
    size_t bucket = hash & (hashmap->capacity - 1);

    // Logic:
    for (size_t iterator = 0; iterator < hashmap->capacity; iterator++) {
        // Variables (Assignment):
        // Index:
        size_t index = (bucket + iterator) & (hashmap->capacity - 1);

        // Logic:
        if (hashmap->keys[index] && hashmap->values[index]) {
            if (strcmp(hashmap->keys[index], key) == 0) {
                return hashmap->values[index];
            }
        }
    }

    return NULL;
}

int logr_hashmap_destroy(logr_hashmap *hashmap) {
    // Validation:
    if (hashmap == NULL) {
        logr_log_error("[!] Can't destroy a NULL hashmap.");

        return UNSUCCESSFUL_OPERATION;
    }

    // Logic:
    for (size_t iterator = 0; iterator < hashmap->capacity; iterator++) {
        if (hashmap->keys[iterator] != NULL) {
            free(hashmap->keys[iterator]);
        }

        if (hashmap->values[iterator] != NULL) {
            free(hashmap->values[iterator]);
        }
    }

    free(hashmap->values);
    free(hashmap->keys);
    free(hashmap);

    return SUCCESSFUL_OPERATION;
}
