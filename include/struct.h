#ifndef STRUCT_H
#define STRUCT_H

#include "libs.h"

const size_t SIZE_LIMIT = SIZE_MAX / 32 * 31;

typedef int item_t;

typedef enum StackErr {
    STACK_SUCCESS,
    NULL_STACK,
    NULL_DATA,
    SIZE_EXCEEDS_CAPACITY,
    ZERO_CAPACITY,
    SIZE_EXCEEDS_LIMIT,
    CAPACITY_EXCEEDS_LIMIT
} StackErr_t;

typedef struct Stack {
    item_t* data;
    size_t size;
    size_t capacity;
} Stack_t;

#endif /* STRUCT_H */
