#include "stack.h"

StackErr_t StackCtor(Stack_t* stack, size_t capacity)
{
    stack->capacity = capacity;

    if (capacity == 0)
    {
        return ZERO_CAPACITY;
    }
    if (capacity > SIZE_LIMIT)
    {
        return CAPACITY_EXCEEDS_LIMIT;
    }

    stack->data = (item_t*) calloc(capacity, sizeof(item_t));

    StackErr_t error = STACK_SUCCESS;
    if ((error = StackVerify(stack)) != STACK_SUCCESS)
    {
        // StackDump(stack)
        return error;
    }

    return STACK_SUCCESS;
}

StackErr_t StackDtor(Stack_t* stack)
{
    free(stack);
    return STACK_SUCCESS;
}
