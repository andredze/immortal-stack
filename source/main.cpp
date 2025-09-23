#include "libs.h"
#include "struct.h"
#include "stack.h"
#include "commands.h"

int main()
{
    Stack_t stack = {};

    StackCtor(&stack, 5);
    StackPush(&stack, 10);
    StackPush(&stack, 20);
    StackPush(&stack, 30);

    item_t item1 = 0;
    StackPop(&stack, &item1);

    printf("item_1 = %d\n", item1);

    // StackErr_t error =
    return EXIT_SUCCESS;
}
