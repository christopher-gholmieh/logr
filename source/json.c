// Written by: Christopher Gholmieh
// Headers:
#include <stdlib.h>
#include <stdio.h>

#include "json.h"

// Functions:
logr_json_node* logr_json_create_dictionary(void) {
    // Variables (Assignment):
    // JSON:
    logr_json_node* json_node = (logr_json_node*) malloc(sizeof(logr_json_node));

    if (json_node == NULL) {
        // Error:
        fprintf(stderr, "[!] Unable to allocate memory for JSON node.\n");

        // Logic:
        return NULL;
    }

    // Dictionary:
    logr_json_dictionary* json_dictionary = (logr_json_dictionary*) malloc(sizeof(logr_json_dictionary));

    if (json_dictionary == NULL) {
        // Error:
        fprintf(stderr, "[!] Unable to allocate memory for JSON dictionary.\n");

        // Logic:
        free(json_node);

        return NULL;
    }

    // Logic:
    json_dictionary->capacity = 4;
    json_dictionary->length = 0;

    json_dictionary->values = malloc(sizeof(logr_json_node*) * json_dictionary->capacity);

    if (json_dictionary->values == NULL) {
        // Error:
        fprintf(stderr, "[!] Unable to allocate memory for dictionary's values.\n");

        // Logic:
        free(json_dictionary);
        free(json_node);

        return NULL;
    }

    json_dictionary->keys = malloc(sizeof(char*) * json_dictionary->capacity);

    if (json_dictionary->keys == NULL) {
        // Error:
        fprintf(stderr, "[!] Unable to allocate memory for dictionary's keys.\n");

        // Logic:
        free(json_dictionary->values);
        free(json_dictionary);
        free(json_node);

        return NULL;
    }

    json_node->json_type = JSON_DICTIONARY;
    json_node->pointer = (void*) json_dictionary;

    return json_node;
}

int logr_json_dictionary_append(logr_json_dictionary* dictionary, char* key, logr_json_node* value) {
    // Validation:
    if (dictionary == NULL) {
        fprintf(stderr, "[!] Unable to append key-value pair to NULL dictionary.\n");

        return LOGR_UNSUCCESSFUL_OPERATION;
    }

    if (key == NULL) {
        fprintf(stderr, "[!] Unable to append NULL key to dictionary.\n");

        return LOGR_UNSUCCESSFUL_OPERATION;
    }

    if (value == NULL) {
        fprintf(stderr, "[!] Unable to append NULL value to dictionary.\n");

        return LOGR_UNSUCCESSFUL_OPERATION;
    }

    if (dictionary->length == dictionary->capacity) {
        // Variables (Assignment):
        // Capacity:
        size_t capacity = dictionary->capacity * 2;

        // Values:
        logr_json_node** values = realloc(dictionary->values, sizeof(logr_json_node*) * capacity);

        // Keys:
        char** keys = realloc(dictionary->keys, sizeof(char*) * capacity);

        // Validation:
        if (values == NULL || keys == NULL) {
            // Error:
            fprintf(stderr, "[!] Unable to reallocate memory for values or keys.\n");

            // Logic:
            if (values != NULL) {
                free(values);
            }

            if (keys != NULL) {
                free(keys);
            }

            return LOGR_UNSUCCESSFUL_OPERATION;
        }

        // Logic:
        dictionary->capacity = capacity;

        dictionary->values = values;
        dictionary->keys = keys;
    }

    // Variables (Assignment):
    // Key:
    char* duplicated_key = strdup(key);
    
    if (duplicated_key == NULL) {
        fprintf(stderr, "[!] Unable to duplicate key.\n");

        return LOGR_UNSUCCESSFUL_OPERATION;
    }

    // Logic:
    dictionary->keys[dictionary->length] = duplicated_key;
    dictionary->values[dictionary->length] = value;

    dictionary->length++;

    return LOGR_SUCCESSFUL_OPERATION;
}

logr_json_node* logr_json_create_array(void) {
    // Variables (Assignment):
    // JSON:
    logr_json_node* json_node = (logr_json_node*) malloc(sizeof(logr_json_node));

    if (json_node == NULL) {
        // Error:
        fprintf(stderr, "[!] Unable to allocate memory for JSON node.\n");

        // Logic:
        return NULL;
    }

    // Array:
    logr_json_array* json_array = (logr_json_array*) malloc(sizeof(logr_json_array));

    if (json_array == NULL) {
        // Error:
        fprintf(stderr, "[!] Unable to allocate memory for JSON array.\n");

        // Logic:
        free(json_node);

        return NULL;
    }

    // Logic:
    json_array->capacity = 4;
    json_array->length = 0;

    json_array->values = malloc(sizeof(logr_json_node*) * json_array->capacity);

    // Validation:
    if (json_array->values == NULL) {
        // Error:
        fprintf(stderr, "[!] Unable to allocate memory for initial array values.\n");

        // logic:
        free(json_array);
        free(json_node);

        return NULL;
    }

    // Logic:
    json_node->json_type = JSON_ARRAY;
    json_node->pointer = (void*) json_array;

    return json_node;
}

int logr_json_array_append(logr_json_array* array, logr_json_node* node) {
    // Validation:
    if (array == NULL) {
        fprintf(stderr, "[!] Can't append a value to a NULL array!\n");

        return LOGR_UNSUCCESSFUL_OPERATION;
    }

    if (node == NULL) {
        fprintf(stderr, "[!] Can't append a NULL node to an array!\n");

        return LOGR_UNSUCCESSFUL_OPERATION;
    }

    if (array->length == array->capacity) {
        // Variables (Assignment):
        // Capacity:
        size_t capacity = array->capacity * 2;

        // Values:
        logr_json_node** values = realloc(array->values, sizeof(logr_json_node*) * capacity);

        if (values == NULL) {
            fprintf(stderr, "[!] Unable to initialize/resize value block for array.\n");

            return LOGR_UNSUCCESSFUL_OPERATION;
        }

        // Logic:
        array->capacity = capacity;
        array->values = values;
    }

    array->values[array->length] = node;
    array->length++;

    return LOGR_SUCCESSFUL_OPERATION;
}

logr_json_node* logr_json_create_boolean(int value) {
    // Variables (Assignment):
    // JSON:
    logr_json_node* json_node = (logr_json_node*) malloc(sizeof(logr_json_node));

    if (json_node == NULL) {
        // Error:
        fprintf(stderr, "[!] Unable to allocate memory for JSON node.\n");

        // Logic:
        return NULL;
    }

    // Boolean:
    logr_json_boolean* json_boolean = (logr_json_boolean*) malloc(sizeof(logr_json_boolean));

    if (json_boolean == NULL) {
        // Error:
        fprintf(stderr, "[!] Unable to allocate memory for JSON node.\n");

        // Logic:
        free(json_node);

        return NULL;
    }

    // Logic:
    json_boolean->value = value;

    json_node->json_type = JSON_BOOLEAN;
    json_node->pointer = (void*) json_boolean;

    return json_node;
}

logr_json_node* logr_json_create_number(double value) {
    // Variables (Assignment):
    // JSON:
    logr_json_node* json_node = (logr_json_node*) malloc(sizeof(logr_json_node));

    if (json_node == NULL) {
        // Error:
        fprintf(stderr, "[!] Unable to allocate memory for JSON node.\n");

        // Logic:
        return NULL;
    }

    // Number:
    logr_json_number* json_number = (logr_json_number*) malloc(sizeof(logr_json_number));

    if (json_number == NULL) {
        // Error:
        fprintf(stderr, "[!] Unable to allocate memory for JSON number.\n");

        // Logic:
        free(json_node);

        return NULL;
    }

    // Logic:
    json_number->value = value;

    json_node->json_type = JSON_NUMBER;
    json_node->pointer = (void*) json_number;

    return json_node;
}

logr_json_node* logr_json_create_string(const char* value) {
    // Variables (Assignment):
    // JSON:
    logr_json_node* json_node = (logr_json_node*) malloc(sizeof(logr_json_node));

    if (json_node == NULL) {
        // Error:
        fprintf(stderr, "[!] Unable to allocate memory for JSON node.\n");

        // Logic:
        return NULL;
    }

    // String:
    logr_json_string* json_string = (logr_json_string*) malloc(sizeof(logr_json_string));

    if (json_string == NULL) {
        // Error:
        fprintf(stderr, "[!] Unable to allocate memory for JSON string.\n");

        // Logic:
        free(json_node);

        return NULL;
    }

    // Logic:
    json_string->value = strdup(value);

    if (!json_string->value) {
        // Error:
        fprintf(stderr, "[!] Unable to duplicate string for JSON string.\n");

        // Logic:
        free(json_string);
        free(json_node);

        return NULL;
    }

    json_node->json_type = JSON_STRING;
    json_node->pointer = (void*) json_string;

    return json_node;
}

void logr_json_destroy_node(logr_json_node* node) {
    // Validation:
    if (node == NULL) {
        fprintf(stderr, "[!] Can't destroy a null pointer to JSON node.\n");

        return;
    }

    // Logic:
    switch (node->json_type) {
        case JSON_DICTIONARY:
            // Variables (Assignment):
            // Dictionary:
            logr_json_dictionary* json_dictionary = (logr_json_dictionary*) node->pointer;

            // Logic:
            for (size_t iterator = 0; iterator < json_dictionary->length; iterator++) {
                if (json_dictionary->keys[iterator]) {
                    free(json_dictionary->keys[iterator]);
                }

                if (json_dictionary->values[iterator]) {
                    free(json_dictionary->values[iterator]);
                }
            }

            free(json_dictionary->keys);
            free(json_dictionary->values);

            free(json_dictionary);

            break;

        case JSON_BOOLEAN:
            free((logr_json_boolean*) node->pointer);

            break;

        case JSON_NUMBER:
            free((logr_json_number*) node->pointer);

            break;

        case JSON_STRING:
            // Variables (Assignment):
            // String:
            logr_json_string* json_string = (logr_json_string*) node->pointer;

            // Logic:
            free(json_string->value);
            free(json_string);

            break;

        case JSON_ARRAY:
            // Variables (Assignment):
            // Array:
            logr_json_array* json_array = (logr_json_array*) node->pointer;

            // Logic:
            if (json_array->values) {
                for (size_t iterator = 0; iterator < json_array->length; iterator++) {
                    logr_json_destroy_node(json_array->values[iterator]);
                }

                free(json_array->values);
            }

            free(json_array);

            break;
    }

    free(node);
}

void logr_json_print_indent(int indent) {
    for (int iterator = 0; iterator < indent; iterator++) {
        printf("    ");
    }
}

void logr_json_print_tree(logr_json_node* head, int indent) {
    if (head == NULL) {
        logr_json_print_indent(indent);
        printf("null\n");

        return;
    }

    switch (head->json_type) {
        case JSON_DICTIONARY:
            // Variables (Assignment):
            // Dictionary:
            logr_json_dictionary* dictionary = (logr_json_dictionary*) head->pointer;

            // Logic:
            printf("{\n");

            for (size_t iterator = 0; iterator < dictionary->length; iterator++) {
                logr_json_print_indent(indent + 1);

                printf("\"%s\": ", dictionary->keys[iterator]);
                logr_json_print_tree(dictionary->values[iterator], indent + 1);

                if (iterator < dictionary->length - 1) {
                    printf(",\n");
                } else {
                    printf("\n");
                }
            }

            logr_json_print_indent(indent);
            printf("}");

            break;
        
        case JSON_BOOLEAN:
            // Variables (Assignment):
            // Boolean:
            logr_json_boolean* boolean = (logr_json_boolean*) head->pointer;

            // Logic:
            printf("%s", boolean->value ? "true" : "false");

            break;

        case JSON_NUMBER:
            // Variables (Assignment):
            // Number:
            logr_json_number* number = (logr_json_number*) head->pointer;

            // Logic:
            printf("%g", number->value);

            break;

        case JSON_STRING:
            // Variables (Assignment):
            // String:
            logr_json_string* string = (logr_json_string*) head->pointer;

            // Logic:
            printf("\"%s\"", string->value);

            break;

        case JSON_ARRAY:
            // Variables (Assignment):
            // Array:
            logr_json_array* array = (logr_json_array*) head->pointer;

            // Logic:
            printf("[\n");

            for (size_t iterator = 0; iterator < array->length; iterator++) {
                logr_json_print_indent(indent + 1);
                logr_json_print_tree(array->values[iterator], indent + 1);

                if (iterator < array->length - 1) {
                    printf(",\n");
                } else {
                    printf("\n");
                }
            }

            logr_json_print_indent(indent);
            printf("]");

            break;

        default:
            printf("null");
    }
}
