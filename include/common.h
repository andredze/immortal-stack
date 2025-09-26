#ifndef STRUCT_H
#define STRUCT_H

#include "libs.h"

const int CANARY_VALUE_INT = 12285766;
const int POISON_INT = INT_MIN;
const size_t SIZE_LIMIT = SIZE_MAX / 32 * 31;
const size_t MAX_LINE_LEN = 200;

typedef int item_t;
const item_t CANARY_VALUE = CANARY_VALUE_INT;
const item_t POISON = POISON_INT;
#define SPEC "%d"

typedef enum StackErr {
    STACK_SUCCESS = 0,
    NULL_STACK = 1,
    NULL_DATA = 2,
    SIZE_EXCEEDS_LIMIT = 3,
    CAPACITY_EXCEEDS_LIMIT = 4,
    SIZE_EXCEEDS_CAPACITY = 5,
    CALLOC_ERROR = 6,
    REALLOC_ERROR = 7,
    WENT_BEYOND_START = 8,
    WENT_BEYOND_END = 9,
    FILE_OPENNING_ERROR = 10,
    SIZE_IS_ZERO = 11
} StackErr_t;

typedef struct VarInfo {
    const char* struct_name;
    const char* file_name;
    const char* function;
    int line;
} VarInfo_t;

typedef struct Stack {
    item_t* data;
    size_t size;
    size_t capacity;
    VarInfo_t VarInfo;
} Stack_t;

#endif /* STRUCT_H */
