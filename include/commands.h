#ifndef COMMANDS_H
#define COMMANDS_H

#include "struct.h"
#include "libs.h"
#include "verify.h"

StackErr_t StackPush(Stack_t* stack, item_t item);

StackErr_t StackPop(Stack_t* stack, item_t* item);

#endif /* COMMANDS_H */
