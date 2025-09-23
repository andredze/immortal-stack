#include "commands.h"

StackErr_t StackPush(Stack_t* stack, item_t item)
{
    StackErr_t error = STACK_SUCCESS;
    if ((error = StackVerify(stack)) != STACK_SUCCESS)
    {
        // StackDump(stack)
        return error;
    }

    stack->data[stack->size++] = item;

    if ((error = StackVerify(stack)) != STACK_SUCCESS)
    {
        // StackDump(stack)
        return error;
    }

    return STACK_SUCCESS;
}

StackErr_t StackPop(Stack_t* stack, item_t* item)
{
    StackErr_t error = STACK_SUCCESS;
    if ((error = StackVerify(stack)) != STACK_SUCCESS)
    {
        // StackDump(stack)
        return error;
    }

    *item = stack->data[--stack->size];

    if ((error = StackVerify(stack)) != STACK_SUCCESS)
    {
        // StackDump(stack)
        return error;
    }

    return STACK_SUCCESS;
}
