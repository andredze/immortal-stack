#include "verify.h"

StackErr_t StackIsOk(Stack_t* stack,
                     const char* file_name,
                     const char* function,
                     int line)
{
    stack->VarInfo.file_name = file_name;
    stack->VarInfo.function = function;
    stack->VarInfo.line = line;
    StackErr_t error = STACK_SUCCESS;
    if ((error = StackVerify(stack)) != STACK_SUCCESS)
    {
        StackDump(stack, error);
    }
    return error;
}

int StackErrToStr(StackErr_t error, const char** line)
{
    switch (error)
    {
        case 0:
            *line = "No errors occurred";
            break;
        case 1:
            *line = "Pointer to the stack structure is NULL";
            break;
        case 2:
            *line = "Pointer to the stack data is NULL";
            break;
        case 3:
            *line = "Capacity equals zero";
            break;
        case 4:
            *line = "Size exceeded limit (possibly set to the negative value)";
            break;
        case 5:
            *line = "Capacity exceeded limit (possibly set to the negative value)";
            break;
        case 6:
            *line = "Size exceeded capacity";
            break;
        default:
            return 1;
    }
    return 0;
}

int StackDump(Stack_t* stack, StackErr_t error)
{
    FILE* stream = fopen("stack.log", "w");

    const char* error_str = NULL;
    StackErrToStr(error, &error_str);

    fprintf(stream, "from %s at %s:%d\n"
                    "ERROR %d: %s\n"
                    "%s [%p]\n{\n"
                    "\tsize = %zu;\n"
                    "\tcapacity = %zu;\n"
                    "\tdata [%p];\n\t{\n",
                    stack->VarInfo.function,
                    stack->VarInfo.file_name,
                    stack->VarInfo.line,
                    error, error_str,
                    stack->VarInfo.struct_name, stack,
                    stack->size,
                    stack->capacity,
                    stack->data);
    size_t size = stack->size;
    size_t capacity = stack->capacity;
    for (size_t i = 0; i < size; i++)
    {
        fprintf(stream, "\t\t*[%zu] = %d;\n", i, stack->data[i]);
    }
    for (size_t j = size; j < capacity; j++)
    {
        fprintf(stream, "\t\t [%zu] = %d;\n", j, stack->data[j]);
    }
    fprintf(stream, "\t}\n"
                    "}");
    return 0;
}

StackErr_t StackVerify(Stack_t* stack)
{
    size_t size = stack->size;
    size_t capacity = stack->capacity;

    if (stack == NULL)
    {
        return NULL_STACK;
    }
    if (stack->data == NULL)
    {
        return NULL_DATA;
    }
    if (capacity == 0)
    {
        return ZERO_CAPACITY;
    }
    if (size > SIZE_LIMIT)
    {
        return SIZE_EXCEEDS_LIMIT;
    }
    if (capacity > SIZE_LIMIT)
    {
        return CAPACITY_EXCEEDS_LIMIT;
    }
    // if (size > capacity)
    // {
    //     return SIZE_EXCEEDS_CAPACITY;
    // }
    return STACK_SUCCESS;
}
