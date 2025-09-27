#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "maths.h"
#include "input.h"
#include "stack.h"

const int MAX_COMMAND_LEN = 200;

typedef enum CalculatorErr {
    CALC_SUCCESS,
    ERROR_WITH_READING_FILE,
    CALC_STACK_ERROR,
    OUTPUT_FILE_OPENNING_ERROR,
    UNKNOWN_CALC_COMMAND,
    MATH_ERROR
} CalculatorErr_t;

typedef enum Command {
    PUSH,
    ADD,
    SUB,
    MUL,
    DIV,
    OUT,
    HLT
} Command_t;

CalculatorErr_t RunCalculator(Context_t* CommandsData);

int get_command(char* line, Command_t* command, int* value);

int run_command(Stack_t* CalcStack, Command_t command,
                int value, FILE* output_stream);

#endif /* CALCULATOR_H */
