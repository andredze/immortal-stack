#include "verify.h"

StackErr_t StackVerify(Stack_t* stack)
{
    size_t size = stack->size;
    size_t capacity = stack->capacity;

    if (stack == NULL)
    {
        return NULL_STACK;
    }
    if (stack->data == NULL)
    {
        return NULL_DATA;
    }
    if (size > capacity)
    {
        return SIZE_EXCEEDS_CAPACITY;
    }
    if (capacity == 0)
    {
        return ZERO_CAPACITY;
    }
    if (size > SIZE_LIMIT)
    {
        return SIZE_EXCEEDS_LIMIT;
    }
    if (capacity > SIZE_LIMIT)
    {
        return CAPACITY_EXCEEDS_LIMIT;
    }
    return STACK_SUCCESS;
}
