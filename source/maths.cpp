#include "maths.h"

MathErr_t ApplyMathOperation(Stack_t* stack,
                             MathErr_t (* Calculate) (CalcData_t* CalcData))
{
    assert(stack != NULL);
    assert(stack->data != NULL);
    assert(Calculate != NULL);
    assert(stack->size >= 2);

    CalcData_t CalcData = {};

    if (StackPop(stack, &CalcData.number1) != STACK_SUCCESS)
    {
        return STACK_ERROR;
    }
    if (StackPop(stack, &CalcData.number2) != STACK_SUCCESS)
    {
        return STACK_ERROR;
    }

    MathErr_t error = MATH_SUCCESS;
    if ((error = Calculate(&CalcData)) != MATH_SUCCESS)
    {
        return error;
    }
    if (StackPush(stack, CalcData.result) != STACK_SUCCESS)
    {
        return STACK_ERROR;
    }

    return MATH_SUCCESS;
}

MathErr_t Add(CalcData_t* CalcData)
{
    assert(CalcData != NULL);
    assert(CalcData->result == 0);

    CalcData->result = CalcData->number1 + CalcData->number2;

    return MATH_SUCCESS;
}

MathErr_t Sub(CalcData_t* CalcData)
{
    assert(CalcData != NULL);
    assert(CalcData->result == 0);

    CalcData->result = CalcData->number1 - CalcData->number2;

    return MATH_SUCCESS;
}

MathErr_t Mul(CalcData_t* CalcData)
{
    assert(CalcData != NULL);
    assert(CalcData->result == 0);

    CalcData->result = CalcData->number1 * CalcData->number2;

    return MATH_SUCCESS;
}

MathErr_t Div(CalcData_t* CalcData)
{
    assert(CalcData != NULL);
    assert(CalcData->result == 0);

    if (CalcData->number2 == 0)
    {
        printf("Can not divide by zero\n");
        return DIVISION_BY_ZERO;
    }
    CalcData->result = CalcData->number1 / CalcData->number2;

    return MATH_SUCCESS;
}

MathErr_t Out(Stack_t* CalcStack)
{
    int answer = 0;

    if (StackPop(CalcStack, &answer) != STACK_SUCCESS)
    {
        return STACK_ERROR;
    }
    printf("ANSWER = %d", answer);

    return MATH_SUCCESS;
}
