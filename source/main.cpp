#include "stack.h"
#include "maths.h"
#include "calculator.h"

int main()
{
    INIT_STACK(stack);

    if (StackCtor(&stack, 4) != STACK_SUCCESS)
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
    StackErr_t pop_return = STACK_SUCCESS;

    for (int i = 0; i < 1; i++)
    {
        if ((pop_return = StackPop(&stack, &poped_item)) != STACK_SUCCESS &&
             pop_return != SIZE_IS_ZERO)
        {
            return EXIT_FAILURE;
        }
        else if (pop_return != SIZE_IS_ZERO)
        {
            printf("poped_item = " SPEC "\n", poped_item);
        }
    }

    // stack.data[1] = 10;
    if (StackPush(&stack, 50) != STACK_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    // stack.size = 100;
    if (StackPush(&stack, 50) != STACK_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    StackDtor(&stack);
    DPRINTF("\n<Programm ran without errors>\n");

    // calculator
    Context_t commands_data = {.input_file_info =  {.filepath = "commands.txt"},
                              .output_file_info = {.filepath = "answers.txt"}};

    if (RunCalculator(&commands_data) != CALC_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
