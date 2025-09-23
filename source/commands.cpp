#include "commands.h"

StackErr_t StackPush(Stack_t* stack, item_t item)
{
    assert(stack != NULL);
    assert(stack->data != NULL);

    stack->data[stack->size++] = item;

    return STACK_SUCCESS;
}

StackErr_t StackPop(Stack_t* stack, item_t* item)
{
    assert(stack != NULL);

    *item = stack->data[--stack->size];

    return STACK_SUCCESS;
}
