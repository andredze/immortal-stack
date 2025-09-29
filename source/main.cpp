#include "stack.h"
#include "maths.h"
#include "calculator.h"
#include "examples.h"

int main()
{
    // RunRightUsageExample();
    // RunCalculatorProgrammExample();

    typedef int (*CreateStackError_t) (Stack_t*);

    CreateStackError_t functions_creating_err[] = {CreateStackIsNullError,
                                                   CreateStackDataIsNullError,
                                                   CreateStackSizeExceedsLimitError,
                                                   CreateStackCapacityExceedsLimitError,
                                                   CreateStackSizeExceedsCapacityError,
                                                   CreateStartCanaryIsRuinedError,
                                                   CreateEndCanaryIsRuinedError,
                                                   CreateHashIsRuinedError};
    int return_value = 0;
    Stack_t stack_for_errors = {};
    int error_code = 0;

    while (1)
    {
        printf("Enter the error_code to test: ");
        if (scanf("%d", &error_code) != 1)
        {
            break;
        }
        if (error_code == EOF)
        {
            break;
        }
        stack_for_errors = RunStartOfExampleForErrors();
        return_value = functions_creating_err[error_code](&stack_for_errors);
        printf("return_value = %d\n", return_value);
        StackDtor(&stack_for_errors);
    }

    return EXIT_SUCCESS;
}
