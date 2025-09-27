#include "calculator.h"

CalculatorErr_t RunCalculator(Context_t* CommandsData)
{
    assert(CommandsData != NULL);

    if (read_and_parse_file(CommandsData))
    {
        return ERROR_WITH_READING_FILE;
    }
    DPRINTF("lines_count = %d\n", CommandsData->BufferData.lines_count);

    INIT_STACK(CalcStack);

    if (StackCtor(&CalcStack, 32) != STACK_SUCCESS)
    {
        return CALC_STACK_ERROR;
    }

    int lines_count = CommandsData->PtrDataParams.lines_count;
    char** ptrdata = CommandsData->PtrDataParams.ptrdata;

    Command_t command = HLT;
    int value = 0;

    if (open_file(&CommandsData->OutputFileInfo, "w"))
    {
        return OUTPUT_FILE_OPENNING_ERROR;
    }

    for (int i = 0; i < lines_count; i++)
    {
        DPRINTF("\nEntering %d iteration of ptrdata for\n", i);
        if (get_command(ptrdata[i], &command, &value))
        {
            return UNKNOWN_CALC_COMMAND;
        }
        DPRINTF("Command = %d\n", command);
        if (command == HLT)
        {
            break;
        }
        if (run_command(&CalcStack, command, value,
                        CommandsData->OutputFileInfo.stream))
        {
            return MATH_ERROR;
        }
    }

    free(CommandsData->BufferData.buffer);
    free(CommandsData->PtrDataParams.ptrdata);
    printf("\n<End of the calculator>");

    return CALC_SUCCESS;
}

int get_command(char* line, Command_t* command, int* value)
{
    assert(line != NULL);
    assert(command != NULL);

    char operation[MAX_COMMAND_LEN] = {};

    int arg_count = sscanf(line, "%s %d", operation, value);
    if (arg_count < 1 || arg_count > 2)
    {
        DPRINTF("Invalid number of arguments for command\n"
                        "arg_count = %d; expected = 2", arg_count);
        return 1;
    }
    if (strcmp(operation, "PUSH") == 0)
    {
        *command = PUSH;
    }
    else if (strcmp(operation, "OUT") == 0)
    {
        *command = OUT;
    }
    else if (strcmp(operation, "ADD") == 0)
    {
        *command = ADD;
    }
    else if (strcmp(operation, "SUB") == 0)
    {
        *command = SUB;
    }
    else if (strcmp(operation, "MUL") == 0)
    {
        *command = MUL;
    }
    else if (strcmp(operation, "DIV") == 0)
    {
        *command = DIV;
    }
    else if (strcmp(operation, "HLT") == 0)
    {
        *command = HLT;
    }
    else
    {
        DPRINTF("Unknown calc command\n");
        return 1;
    }

    return 0;
}

int run_command(Stack_t* CalcStack, Command_t command,
                int value, FILE* output_stream)
{
    assert(CalcStack != NULL);

    switch (command)
    {
        case PUSH: {
            if (StackPush(CalcStack, value) != STACK_SUCCESS)
            {
                return 1;
            }
            break;
        }
        case ADD: {
            if (ApplyMathOperation(CalcStack, Add) != 0)
            {
                return 1;
            }
            break;
        }
        case SUB: {
            if (ApplyMathOperation(CalcStack, Sub) != 0)
            {
                return 1;
            }
            break;
        }
        case MUL: {
            if (ApplyMathOperation(CalcStack, Mul) != 0)
            {
                return 1;
            }
            break;
        }
        case DIV: {
            if (ApplyMathOperation(CalcStack, Div) != 0)
            {
                return 1;
            }
            break;
        }
        case OUT: {
            int result = 0;
            StackErr_t pop_return = StackPop(CalcStack, &result);

            if (pop_return == SIZE_IS_ZERO)
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
        case HLT:
            return 1;
        default:
            return 1;
    }

    return 0;
}
