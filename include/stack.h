#ifndef STACK_H
#define STACK_H

// #include <TXLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

const size_t SIZE_LIMIT = SIZE_MAX / 32 * 31;

const int CANARY_VALUE_INT = 0xABEBADED;
const int POISON_INT = 0xDEAFBABA;

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

#ifdef DEBUG
typedef struct VarInfo {
    const char* struct_name;
    const char* file_name;
    const char* function;
    int line;
} VarInfo_t;
#endif

typedef struct Stack {
    item_t* data;
    size_t size;
    size_t capacity;
#ifdef DEBUG
    VarInfo_t VarInfo;
#endif
} Stack_t;

StackErr_t StackCtor(Stack_t* stack, size_t capacity);

StackErr_t StackRealloc(Stack_t* stack);

StackErr_t StackPush(Stack_t* stack, item_t item);

StackErr_t StackPop(Stack_t* stack, item_t* item);

StackErr_t StackDtor(Stack_t* stack);

StackErr_t StackIsOk(Stack_t* stack,
                     const char* file_name,
                     const char* function,
                     int line);

int StackErrToStr(StackErr_t error, const char* line[]);

StackErr_t StackDump(Stack_t* stack, StackErr_t error);

StackErr_t StackVerify(Stack_t* stack);

#endif /* STACK_H */
