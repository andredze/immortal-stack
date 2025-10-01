#include "examples.h"

int RunErrorTests()
{
    SimulateStackError_t functions_creating_err[] =
        {SimulateStackIsNullError,
         SimulateStackDataIsNullError,
         SimulateStackSizeExceedsLimitError,
         SimulateStackCapacityExceedsLimitError,
         SimulateStackSizeExceedsCapacityError,
         SimulateStartCanaryIsRuinedError,
         SimulateEndCanaryIsRuinedError,
         SimulateHashIsRuinedError};

    int return_value = 0;
    Stack_t stack_for_errors = {};
    int error_code = 0;

    printf("\n-------------Enter error codes from 1 to 8; 0 to exit-------------\n\n");
    // error code from 1 to 8, other to exit
    while (1)
    {
        printf("-----<TEST STARTED>-----\n");
        printf("Enter the error_code to test: ");
        if (scanf("%d", &error_code) != 1)
        {
            break;
        }
        error_code--;
        if (error_code < 0 || error_code > 7)
        {
            break;
        }
        stack_for_errors = RunStartOfExampleForErrors();
        return_value = functions_creating_err[error_code](&stack_for_errors);
        printf("return_value = %d\n", return_value);

        // не работает на ошибке 5, т.к испорчена управл. структура
        if (error_code != 5)
        {
            printf("Destroying stack...\n");
            StackDtor(&stack_for_errors);
        }
        printf("-----<TEST ENDED>-----\n\n");
    }

    return 0;
}

int RunExampleOfPush(Stack_t* stack)
{
    if (StackPush(stack, STACK_PUSH_EXAMPLE_VALUE) != STACK_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int RunExampleOfPop(Stack_t* stack)
{
    int value = 10;

    if (StackPop(stack, &value) != STACK_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int RunRightUsageExample()
{
    DPRINTF("<Normal programm usage>\n\n");

    INIT_STACK(stack1);

    if (StackCtor(&stack1, 16) != STACK_SUCCESS)
    {
        return EXIT_FAILURE;
    }
    StackPrint(&stack1);

    int values[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    size_t values_len = sizeof(values) / sizeof(values[0]);

    for (size_t i = 0; i < values_len / 2; i++)
    {
        if (StackPush(&stack1, values[i]) != STACK_SUCCESS)
        {
            return EXIT_FAILURE;
        }
        StackPrint(&stack1);
    }

    item_t poped_item = 0;
    StackErr_t pop_return = STACK_SUCCESS;

    for (size_t i = 0; i < values_len / 2 + 1; i++)
    {
        if ((pop_return = StackPop(&stack1, &poped_item)) != STACK_SUCCESS &&
             pop_return != STACK_SIZE_IS_ZERO)
        {
            return EXIT_FAILURE;
        }
        StackPrint(&stack1);
    }

    for (size_t i = values_len / 2 + 1; i < values_len; i++)
    {
        if (StackPush(&stack1, values[i]) != STACK_SUCCESS)
        {
            return EXIT_FAILURE;
        }
        StackPrint(&stack1);
    }

    StackDtor(&stack1);
    DPRINTF("\n<Example programm ran without errors>\n");

    return EXIT_SUCCESS;
}

Stack_t RunStartOfExampleForErrors()
{
    INIT_STACK(stack1);

    if (StackCtor(&stack1, 4) != STACK_SUCCESS)
    {
            stack1.data = NULL;
            return stack1;
    }

    int values[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 89, 79, 69, 59};
    size_t values_len = sizeof(values) / sizeof(values[0]);

    for (size_t i = 0; i < values_len; i++)
    {
        if (StackPush(&stack1, values[i]) != STACK_SUCCESS)
        {
            stack1.data = NULL;
            return stack1;
        }
    }

    return stack1;
}

int SimulateStackIsNullError(Stack_t* stack)
{
    stack = NULL;
    return RunExampleOfPush(stack);
}

int SimulateStackDataIsNullError(Stack_t* stack)
{
    stack->data = NULL;
    return RunExampleOfPop(stack);
}

int SimulateStackSizeExceedsLimitError(Stack_t* stack)
{
    stack->size = -1llu;
    return RunExampleOfPush(stack);
}

int SimulateStackCapacityExceedsLimitError(Stack_t* stack)
{
    stack->capacity = 10;
    stack->capacity = stack->capacity - 20;
    return RunExampleOfPush(stack);
}

int SimulateStackSizeExceedsCapacityError(Stack_t* stack)
{
    stack->size = stack->capacity + 20;
    return RunExampleOfPush(stack);
}

int SimulateStartCanaryIsRuinedError(Stack_t* stack)
{
    // zeroing some array
    char* array = (char*) (stack->data) - 10;
    int array_len = 20;
    for (int i = 0; i < array_len; i++)
    {
        *(array + i) = 0;
    }

    return RunExampleOfPop(stack);
}

int SimulateEndCanaryIsRuinedError(Stack_t* stack)
{
    // filling some array in reversed order
    char* array = (char*) (stack->data + stack->capacity + 2) - 1;
    int array_len = 5 * sizeof(int);
    for (int i = 0; i < array_len; i++)
    {
        *(array - i) = 0;
    }

    return RunExampleOfPush(stack);
}

int SimulateHashIsRuinedError(Stack_t* stack)
{
    int* array = (int*) (stack->data);
    *(array + 2) = 100000000;

    return RunExampleOfPush(stack);
}
