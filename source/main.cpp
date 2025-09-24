#include "stack.h"
#include "commands.h"
#include "libs.h"
#include "struct.h"

int main()
{
    Stack_t stack = {.VarInfo = {.struct_name = "stack",
                                 .file_name = __FILE__,
                                 .function = __func__,
                                 .line = __LINE__}};

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

    item_t poped_item = 0;

    if (StackPop(&stack, &poped_item) != STACK_SUCCESS)
    {
        return EXIT_FAILURE;
    }
    printf("poped_item = %d\n", poped_item);

    if (StackPop(&stack, &poped_item) != STACK_SUCCESS)
    {
        return EXIT_FAILURE;
    }
    printf("poped_item = %d\n", poped_item);

    if (StackPop(&stack, &poped_item) != STACK_SUCCESS)
    {
        return EXIT_FAILURE;
    }
    printf("poped_item = %d\n", poped_item);

    if (StackPop(&stack, &poped_item) != STACK_SUCCESS)
    {
        printf("ERROR");
        return EXIT_FAILURE;
    }
    printf("poped_item = %d\n", poped_item);

    return EXIT_SUCCESS;
}
