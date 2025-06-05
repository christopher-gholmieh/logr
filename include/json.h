// Written by: Christopher Gholmieh
// Guards:
#ifndef __LOGR_JSON_H__
#define __LOGR_JSON_H__

// Macros:
#define LOGR_UNSUCCESSFUL_OPERATION -1
#define LOGR_SUCCESSFUL_OPERATION 1

// Headers:
#include <stdlib.h>
#include <stdio.h>

#include <string.h>

// Enumerations:
typedef enum {
    // Dictionary:
    JSON_DICTIONARY,

    // Boolean:
    JSON_BOOLEAN,

    // Number:
    JSON_NUMBER,

    // String:
    JSON_STRING,

    // Array:
    JSON_ARRAY,
} logr_json_type;

// Structures:
typedef struct {
    // Pointer:
    void* pointer;

    // JSON:
    logr_json_type json_type;
} logr_json_node;

typedef struct {
    // Keys:
    char** keys;

    // Values:
    logr_json_node** values;

    // Capacity:
    size_t capacity;

    // Length:
    size_t length;
} logr_json_dictionary;

typedef struct {
    int value;
} logr_json_boolean;

typedef struct {
    double value;
} logr_json_number;

typedef struct {
    char* value;
} logr_json_string;

typedef struct {
    // Values:
    logr_json_node** values;

    // Capacity:
    size_t capacity;

    // Length:
    size_t length;
} logr_json_array;

// Functions:
logr_json_node* logr_json_create_dictionary(void);
int logr_json_dictionary_append(logr_json_dictionary* dictionary, char* key, logr_json_node* value);

logr_json_node* logr_json_create_array(void);
int logr_json_array_append(logr_json_array* array, logr_json_node* node);

logr_json_node* logr_json_create_boolean(int value);
logr_json_node* logr_json_create_number(double value);

logr_json_node* logr_json_create_string(const char* value);

void logr_json_destroy_node(logr_json_node* node);

void logr_json_print_indent(int indent);
void logr_json_print_tree(logr_json_node* head, int indent);

// Guard:
#endif /** __LOGR_JSON_H__ */
