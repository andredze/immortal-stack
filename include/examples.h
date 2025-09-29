#ifndef EXAMPLES_H
#define EXAMPLES_H

#include "stack.h"
#include "maths.h"
#include "calculator.h"

typedef int (*SimulateStackError_t) (Stack_t*);

const int STACK_PUSH_EXAMPLE_VALUE = 50;

int RunErrorTests();

int RunCalculatorProgrammExample();

int RunExampleOfPush(Stack_t* stack);

int RunExampleOfPop(Stack_t* stack);

int RunRightUsageExample();

Stack_t RunStartOfExampleForErrors();

int SimulateStackIsNullError(Stack_t* stack);

int SimulateStackDataIsNullError(Stack_t* stack);

int SimulateStackSizeExceedsLimitError(Stack_t* stack);

int SimulateStackCapacityExceedsLimitError(Stack_t* stack);

int SimulateStackSizeExceedsCapacityError(Stack_t* stack);

int SimulateStartCanaryIsRuinedError(Stack_t* stack);

int SimulateEndCanaryIsRuinedError(Stack_t* stack);

int SimulateHashIsRuinedError(Stack_t* stack);

#endif /* EXAMPLES_H */
