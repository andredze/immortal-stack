#include "calculator.h"

CalcErr_t ExecuteCalculatorProgramm(Context_t* commands_data)
{
    assert(commands_data != NULL);

    if (ReadAndParseFile(commands_data))
    {
        return CALC_ERROR_WITH_READING_FILE;
    }
    DPRINTF("lines_count = %d\n", commands_data->buffer_data.lines_count);

    if (OpenFile(&commands_data->output_file_info, "w"))
    {
        return CALC_OUTPUT_FILE_OPENNING_ERROR;
    }

    CalcErr_t error = CALC_SUCCESS;
    if ((error = ExecuteCommands(commands_data)) != CALC_SUCCESS)
    {
        return error;
    }

    free(commands_data->buffer_data.buffer);
    free(commands_data->ptrdata_params.ptrdata);
    printf("\n<End of the calculator>");

    return CALC_SUCCESS;
}

CalcErr_t ExecuteCommands(Context_t* commands_data)
{
    Command_t command = COMM_HLT;
    int value = 0;

    INIT_STACK(calc_stack);

    if (StackCtor(&calc_stack, CALC_MIN_STACK_CAPACITY) != STACK_SUCCESS)
    {
        return CALC_STACK_ERROR;
    }

    for (int i = 0; i < commands_data->ptrdata_params.lines_count; i++)
    {
        DPRINTF("\nEntering %d iteration of ptrdata for\n", i);

        if (GetCommand(commands_data->ptrdata_params.ptrdata[i],
                       &command, &value))
        {
            return CALC_UNKNOWN_COMMAND;
        }
        DPRINTF("Command = %d\n", command);

        if (command == COMM_HLT)
        {
            break;
        }
        if (RunCommand(&calc_stack, command, value,
                        commands_data->output_file_info.stream))
        {
            return CALC_MATH_ERROR;
        }
    }

    return CALC_SUCCESS;
}

int GetCommand(char* line, Command_t* command, int* value)
{
    assert(line != NULL);
    assert(command != NULL);

    char operation[CALC_MAX_COMMAND_LEN] = {};

    int arg_count = sscanf(line, "%s %d", operation, value);
    if (arg_count < 1 || arg_count > 2)
    {
        DPRINTF("Invalid number of arguments for command\n"
                        "arg_count = %d; expected = 2", arg_count);
        return 1;
    }

    CommCase_t comm_cases[] = {{"PUSH", COMM_PUSH},
                               {"ADD", COMM_ADD},
                               {"SUB", COMM_SUB},
                               {"MUL", COMM_MUL},
                               {"DIV", COMM_DIV},
                               {"OUT", COMM_OUT},
                               {"HLT", COMM_HLT}};

    size_t comm_cases_size = sizeof(comm_cases) / sizeof(comm_cases[0]);

    for (size_t i = 0; i < comm_cases_size; i++)
    {
        if (strcmp(operation, comm_cases[i].str_command) == 0)
        {
            *command = comm_cases[i].command;
            return 0;
        }
    }

    DPRINTF("Unknown calc command\n");
    return 1;
}

int RunCommand(Stack_t* calc_stack, Command_t command,
                int value, FILE* output_stream)
{
    assert(calc_stack != NULL);

    switch (command)
    {
        case COMM_PUSH:
            return StackPush(calc_stack, value) == STACK_SUCCESS ? 0 : 1;
        case COMM_ADD:
            return ApplyMathOperation(calc_stack, Add) == MATH_SUCCESS ? 0 : 1;
        case COMM_SUB:
            return ApplyMathOperation(calc_stack, Sub) == MATH_SUCCESS ? 0 : 1;
        case COMM_MUL:
            return ApplyMathOperation(calc_stack, Mul) == MATH_SUCCESS ? 0 : 1;
        case COMM_DIV:
            return ApplyMathOperation(calc_stack, Div) == MATH_SUCCESS ? 0 : 1;
        case COMM_OUT:
            return HandleOut(calc_stack, output_stream);
        case COMM_HLT:
            return 1;
        default:
            return 1;
    }

    return 0;
}

int HandleOut(Stack_t* calc_stack, FILE* output_stream)
{
    int result = 0;
    StackErr_t pop_return = StackPop(calc_stack, &result);

    if (pop_return == STACK_SIZE_IS_ZERO)
    {
        return 0;
    }
    if (pop_return != STACK_SUCCESS)
    {
        return 1;
    }

    fprintf(output_stream, "ANSWER = %d\n", result);
    return 0;
}
