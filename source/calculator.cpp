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
    if (strcmp(operation, "PUSH") == 0)
    {
        *command = COMM_PUSH;
    }
    else if (strcmp(operation, "OUT") == 0)
    {
        *command = COMM_OUT;
    }
    else if (strcmp(operation, "ADD") == 0)
    {
        *command = COMM_ADD;
    }
    else if (strcmp(operation, "SUB") == 0)
    {
        *command = COMM_SUB;
    }
    else if (strcmp(operation, "MUL") == 0)
    {
        *command = COMM_MUL;
    }
    else if (strcmp(operation, "DIV") == 0)
    {
        *command = COMM_DIV;
    }
    else if (strcmp(operation, "HLT") == 0)
    {
        *command = COMM_HLT;
    }
    else
    {
        DPRINTF("Unknown calc command\n");
        return 1;
    }

    return 0;
}

int RunCommand(Stack_t* calc_stack, Command_t command,
                int value, FILE* output_stream)
{
    assert(calc_stack != NULL);

    switch (command)
    {
        case COMM_PUSH: {
            if (StackPush(calc_stack, value) != STACK_SUCCESS)
            {
                return 1;
            }
            break;
        }
        case COMM_ADD: {
            if (ApplyMathOperation(calc_stack, Add) != 0)
            {
                return 1;
            }
            break;
        }
        case COMM_SUB: {
            if (ApplyMathOperation(calc_stack, Sub) != 0)
            {
                return 1;
            }
            break;
        }
        case COMM_MUL: {
            if (ApplyMathOperation(calc_stack, Mul) != 0)
            {
                return 1;
            }
            break;
        }
        case COMM_DIV: {
            if (ApplyMathOperation(calc_stack, Div) != 0)
            {
                return 1;
            }
            break;
        }
        case COMM_OUT: {
            int result = 0;
            StackErr_t pop_return = StackPop(calc_stack, &result);

            if (pop_return == STACK_SIZE_IS_ZERO)
            {
                break;
            }
            if (pop_return != STACK_SUCCESS)
            {
                return 1;
            }
            fprintf(output_stream, "ANSWER = %d\n", result);
            break;
        }
        case COMM_HLT:
            return 1;
        default:
            return 1;
    }

    return 0;
}
