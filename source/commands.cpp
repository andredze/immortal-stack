#include "commands.h"

StackErr_t StackPush(Stack_t* stack, item_t item)
{
    StackErr_t error = STACK_SUCCESS;
    if ((error = StackIsOk(stack, __FILE__, __func__, __LINE__)) != STACK_SUCCESS)
    {
        return error;
    }

    fprintf(stderr, "pushed = %d\n", item);
    stack->data[stack->size++] = item;
    fprintf(stderr, "stack size = %zu\n", stack->size);

    if ((error = StackIsOk(stack, __FILE__, __func__, __LINE__)) != STACK_SUCCESS)
    {
        return error;
    }

    return STACK_SUCCESS;
}

StackErr_t StackPop(Stack_t* stack, item_t* item)
{
    StackErr_t error = STACK_SUCCESS;
    if ((error = StackIsOk(stack, __FILE__, __func__, __LINE__)) != STACK_SUCCESS)
    {
        return error;
    }

    *item = stack->data[--stack->size];
    stack->data[stack->size] = POISON;
    fprintf(stderr, "stack size = %zu\n", stack->size);

    if ((error = StackIsOk(stack, __FILE__, __func__, __LINE__)) != STACK_SUCCESS)
    {
        return error;
    }

    return STACK_SUCCESS;
}
