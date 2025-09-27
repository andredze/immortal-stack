#include "stack.h"
#include "maths.h"

#ifdef DEBUG
#define INIT_STACK(name) Stack_t name = {.VarInfo = {#name, __FILE__, __func__, __LINE__}}
#else
#define INIT_STACK(name) Stack_t name = {}
#endif

int main()
{
//     INIT_STACK(stack);
//
//     if (StackCtor(&stack, 5) != STACK_SUCCESS)
//     {
//         return EXIT_FAILURE;
//     }
//
//     if (StackPush(&stack, 10) != STACK_SUCCESS)
//     {
//         return EXIT_FAILURE;
//     }
//     if (StackPush(&stack, 20) != STACK_SUCCESS)
//     {
//         return EXIT_FAILURE;
//     }
//     if (StackPush(&stack, 30) != STACK_SUCCESS)
//     {
//         return EXIT_FAILURE;
//     }
//     if (StackPush(&stack, 40) != STACK_SUCCESS)
//     {
//         return EXIT_FAILURE;
//     }
//     if (StackPush(&stack, 50) != STACK_SUCCESS)
//     {
//         return EXIT_FAILURE;
//     }
//
//     item_t poped_item = 0;
//     for (int i = 0; i < 3; i++)
//     {
//         if (StackPop(&stack, &poped_item) != STACK_SUCCESS)
//         {
//             return EXIT_FAILURE;
//         }
//         printf("poped_item = " SPEC "\n", poped_item);
//     }
//
//     stack.size = 100;
//     if (StackPush(&stack, 50) != STACK_SUCCESS)
//     {
//         return EXIT_FAILURE;
//     }
//
    // StackDtor(&stack);
    // fprintf(stderr, "\n<Programm ran without errors>");

    INIT_STACK(CalcStack);

    if (StackCtor(&CalcStack, 5) != STACK_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    if (StackPush(&CalcStack, 50) != STACK_SUCCESS)
    {
        return EXIT_FAILURE;
    }
    if (StackPush(&CalcStack, 100) != STACK_SUCCESS)
    {
        return EXIT_FAILURE;
    }
    if (StackPush(&CalcStack, 200) != STACK_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    if (ApplyMathOperation(&CalcStack, Add) != 0)
    {
        return EXIT_FAILURE;
    }
    if (ApplyMathOperation(&CalcStack, Add) != 0)
    {
        return EXIT_FAILURE;
    }

    int answer = 0;
    if (StackPop(&CalcStack, &answer) != STACK_SUCCESS)
    {
        return EXIT_FAILURE;
    }
    printf("answer = %d\n", answer);

    return EXIT_SUCCESS;
}
