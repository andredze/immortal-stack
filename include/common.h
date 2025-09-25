#ifndef STRUCT_H
#define STRUCT_H

#include "libs.h"

#define STACK_OK(stack) if ((error = StackIsOk(stack, __FILE__, __func__, __LINE__)) != STACK_SUCCESS) \
                        { \
                            return error; \
                        }

const int CANARY_VALUE = 12285766;
const int POISON = INT_MIN;
const size_t SIZE_LIMIT = SIZE_MAX / 32 * 31;
const size_t MAX_LINE_LEN = 200;

typedef int item_t;

typedef enum StackErr {
    STACK_SUCCESS = 0,
    NULL_STACK = 1,
    NULL_DATA = 2,
    ZERO_CAPACITY = 3,
    SIZE_EXCEEDS_LIMIT = 4,
    CAPACITY_EXCEEDS_LIMIT = 5,
    SIZE_EXCEEDS_CAPACITY = 6,
    CALLOC_ERROR = 7,
    REALLOC_ERROR = 8,
    WENT_BEYOND_START = 9,
    WENT_BEYOND_END = 10,
    FILE_OPENNING_ERROR = 11
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
