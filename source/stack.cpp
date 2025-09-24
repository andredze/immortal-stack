#include "stack.h"

StackErr_t StackCtor(Stack_t* stack, size_t capacity)
{
    stack->capacity = capacity;

    if (capacity > SIZE_LIMIT)
    {
        return CAPACITY_EXCEEDS_LIMIT;
    }

    item_t* data = (item_t*) calloc(capacity, sizeof(item_t));
    stack->data = data;
    for (size_t i = 0; i < capacity; i++)
    {
        data[i] = POISON;
    }

    StackErr_t error = STACK_SUCCESS;
    if ((error = StackIsOk(stack, __FILE__, __func__, __LINE__)) != STACK_SUCCESS)
    {
        return error;
    }

    return STACK_SUCCESS;
}

StackErr_t StackDtor(Stack_t* stack)
{
    free(stack->data);
    stack->data = NULL;
    return STACK_SUCCESS;
}
