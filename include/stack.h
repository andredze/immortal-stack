#ifndef STACK_H
#define STACK_H

#include "struct.h"
#include "libs.h"

StackErr_t StackCtor(Stack_t* stack, size_t capacity);

StackErr_t StackDtor(Stack_t* stack);

#endif /* STACK_H */
