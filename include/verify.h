#ifndef VERIFY_H
#define VERIFY_H

#include "struct.h"
#include "libs.h"

StackErr_t StackIsOk(Stack_t* stack,
                     const char* file_name,
                     const char* function,
                     int line);

int StackErrToStr(StackErr_t error, const char* line[]);

int StackDump(Stack_t* stack, StackErr_t error);

StackErr_t StackVerify(Stack_t* stack);

#endif /* VERIFY_H */
