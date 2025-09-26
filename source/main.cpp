#include "stack.h"
#include "libs.h"
#include "common.h"

#define INIT(name) Stack_t name = {.VarInfo = {#name, __FILE__, __func__, __LINE__}}

int main()
{
    INIT(stack);

    if (StackCtor(&stack, 5) != STACK_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    if (StackPush(&stack, 10) != STACK_SUCCESS)
    {
        return EXIT_FAILURE;
    }
    if (StackPush(&stack, 20) != STACK_SUCCESS)
    {
        return EXIT_FAILURE;
    }
    if (StackPush(&stack, 30) != STACK_SUCCESS)
    {
        return EXIT_FAILURE;
    }
    if (StackPush(&stack, 40) != STACK_SUCCESS)
    {
        return EXIT_FAILURE;
    }
    if (StackPush(&stack, 50) != STACK_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    item_t poped_item = 0;
    for (int i = 0; i < 3; i++)
    {
        if (StackPop(&stack, &poped_item) != STACK_SUCCESS)
        {
            return EXIT_FAILURE;
        }
        printf("poped_item = %d\n", poped_item);
    }

    stack.size = 100;
    if (StackPush(&stack, 50) != STACK_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    fprintf(stderr, "\n<Programm ran without errors>");
    return EXIT_SUCCESS;
}
