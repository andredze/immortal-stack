#include "stack.h"
#include "maths.h"
#include "calculator.h"
#include "examples.h"

int main()
{
    RunRightUsageExample();
    // RunCalculatorProgrammExample();

    Stack_t stack_for_errors = RunStartOfExampleForErrors();

    // CreateStackIsNullError(&stack_for_errors);
    // CreateStackDataIsNullError(&stack_for_errors);
    // CreateStackSizeExceedsLimitError(&stack_for_errors);
    // CreateStackCapacityExceedsLimitError(&stack_for_errors);
    // CreateStackSizeExceedsCapacityError(&stack_for_errors);
    // CreateStartCanaryIsRuinedError(&stack_for_errors);
    // CreateEndCanaryIsRuinedError(&stack_for_errors);
    // CreateHashIsRuinedError(&stack_for_errors);
    StackDtor(&stack_for_errors);

    return EXIT_SUCCESS;
}
