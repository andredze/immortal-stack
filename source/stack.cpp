#include "stack.h"

StackErr_t StackCtor(Stack_t* stack, size_t capacity)
{
    assert(stack != NULL);

    stack->capacity = capacity;
    stack->data = (item_t*) calloc(capacity, sizeof(item_t));

    return STACK_SUCCESS;
}

StackErr_t StackDtor(Stack_t* stack)
{
    free(stack);
    return STACK_SUCCESS;
}
