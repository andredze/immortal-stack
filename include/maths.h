#ifndef MATHS_H
#define MATHS_H

#include "stack.h"

typedef enum MathErr {
    MATH_SUCCESS,
    DIVISION_BY_ZERO,
    STACK_ERROR
} MathErr_t;

typedef struct CalcData {
    int number1;
    int number2;
    int result;
} CalcData_t;

MathErr_t ApplyMathOperation(Stack_t* stack,
                             MathErr_t (* Calculate) (CalcData_t* CalcData));

MathErr_t Add(CalcData_t* CalcData);

MathErr_t Sub(CalcData_t* CalcData);

MathErr_t Div(CalcData_t* CalcData);

MathErr_t Mul(CalcData_t* CalcData);

MathErr_t Out(Stack_t* stack);

#endif /* MATHS_H */
