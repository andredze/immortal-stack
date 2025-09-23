#ifndef STRUCT_H
#define STRUCT_H

#include "libs.h"

typedef int item_t;

typedef enum StackErr {
    STACK_SUCCESS,
    ADDRESS_NULL
} StackErr_t;

typedef struct Stack {
    item_t* data;
    size_t size;
    size_t capacity;
} Stack_t;

#endif /* STRUCT_H */
