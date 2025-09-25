#ifndef STACK_H
#define STACK_H

#include "common.h"
#include "libs.h"

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
