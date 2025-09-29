#ifndef EXAMPLES_H
#define EXAMPLES_H

#include "stack.h"
#include "maths.h"
#include "calculator.h"

typedef int (*CreateStackError_t) (Stack_t*);

const int STACK_PUSH_EXAMPLE_VALUE = 50;

int RunErrorTests();

int RunCalculatorProgrammExample();

int RunExampleOfPush(Stack_t* stack);

int RunExampleOfPop(Stack_t* stack);

int RunRightUsageExample();

Stack_t RunStartOfExampleForErrors();

int CreateStackIsNullError(Stack_t* stack);

int CreateStackDataIsNullError(Stack_t* stack);

int CreateStackSizeExceedsLimitError(Stack_t* stack);

int CreateStackCapacityExceedsLimitError(Stack_t* stack);

int CreateStackSizeExceedsCapacityError(Stack_t* stack);

int CreateStartCanaryIsRuinedError(Stack_t* stack);

int CreateEndCanaryIsRuinedError(Stack_t* stack);

int CreateHashIsRuinedError(Stack_t* stack);

#endif /* EXAMPLES_H */
