#include "stack.h"

#ifdef DEBUG
#define DEBUG_ACTIVE 1
#else
#define DEBUG_ACTIVE 0
#endif

#define STACK_OK(stack) if ((error = StackIsOk(stack, __FILE__, __func__, __LINE__)) != STACK_SUCCESS) \
                        { \
                            return error; \
                        }

StackErr_t StackCtor(Stack_t* stack, size_t capacity)
{
    if (stack == NULL)
    {
        fprintf(stderr, "<Stack is null pointer>");
        return NULL_STACK;
    }
    stack->capacity = capacity;

    if (stack->capacity > SIZE_LIMIT)
    {
        return CAPACITY_EXCEEDS_LIMIT;
    }

    item_t* data = (item_t*) calloc(capacity + 2, sizeof(item_t));
    if (data == NULL)
    {
        fprintf(stderr, "Memory allocation failed");
        return CALLOC_ERROR;
    }
    data[0] = CANARY_VALUE;
    data[capacity + 1] = CANARY_VALUE;
    stack->size = 0;

    for (size_t i = 1; i < capacity + 1; i++)
    {
        data[i] = POISON;
    }

    stack->data = data;

    StackErr_t error = STACK_SUCCESS;
    if (DEBUG_ACTIVE)
    {
        STACK_OK(stack);
    }

    return STACK_SUCCESS;
}

StackErr_t StackRealloc(Stack_t* stack)
{
    StackErr_t error = STACK_SUCCESS;
    if (DEBUG_ACTIVE)
    {
        STACK_OK(stack);
    }

    stack->capacity = stack->capacity * 3 / 2 + 1;
    item_t* data = stack->data;

    data = (item_t*) realloc(data, stack->capacity + 2);
    if (data == NULL)
    {
        fprintf(stderr, "Memory reallocation failed");
        return REALLOC_ERROR;
    }
    data[0] = CANARY_VALUE;
    data[stack->capacity + 1] = CANARY_VALUE;

    if (DEBUG_ACTIVE)
    {
        STACK_OK(stack);
    }

    return STACK_SUCCESS;
}

StackErr_t StackPush(Stack_t* stack, item_t item)
{
    StackErr_t error = STACK_SUCCESS;
    if (DEBUG_ACTIVE)
    {
        STACK_OK(stack);
    }

    // fprintf(stderr, "stack size = %zu\n", stack->size);
    if (stack->size == stack->capacity)
    {
        if ((error = StackRealloc(stack)) != STACK_SUCCESS)
        {
            return error;
        }
    }
    stack->data[1 + stack->size++] = item;
    // fprintf(stderr, "pushed = %d\n", item);
    // fprintf(stderr, "stack size = %zu\n", stack->size);

    if (DEBUG_ACTIVE)
    {
        STACK_OK(stack);
    }

    return STACK_SUCCESS;
}

StackErr_t StackPop(Stack_t* stack, item_t* item)
{
    StackErr_t error = STACK_SUCCESS;
    if (DEBUG_ACTIVE)
    {
        STACK_OK(stack);
    }

    // fprintf(stderr, "stack size = %zu\n", stack->size);
    if (stack->size == 0)
    {
        fprintf(stderr, "<Pop is impossible with zero size>");
        return SIZE_IS_ZERO;
    }
    *item = stack->data[--stack->size + 1];
    stack->data[stack->size + 1] = POISON;
    // fprintf(stderr, "stack size = %zu\n", stack->size);

    if (DEBUG_ACTIVE)
    {
        STACK_OK(stack);
    }

    return STACK_SUCCESS;
}

StackErr_t StackDtor(Stack_t* stack)
{
    free(stack->data);
    stack->data = NULL;
    return STACK_SUCCESS;
}

StackErr_t StackIsOk(Stack_t* stack,
                     const char* file_name,
                     const char* function,
                     int line)
{
    stack->VarInfo.file_name = file_name;
    stack->VarInfo.function = function;
    stack->VarInfo.line = line;

    StackErr_t error = STACK_SUCCESS;
    StackErr_t dump_return = STACK_SUCCESS;
    if ((error = StackVerify(stack)) != STACK_SUCCESS)
    {
        fprintf(stderr, "<Error occurred>");
        if ((dump_return = StackDump(stack, error)) != STACK_SUCCESS)
        {
            return dump_return;
        }
    }
    return error;
}

int StackErrToStr(StackErr_t error, const char** line)
{
    switch (error)
    {
        case STACK_SUCCESS:
            *line = "No errors occurred";
            break;
        case NULL_STACK:
            *line = "Pointer to the stack structure is NULL";
            break;
        case NULL_DATA:
            *line = "Pointer to the stack data is NULL";
            break;
        case ZERO_CAPACITY:
            *line = "Capacity equals zero";
            break;
        case SIZE_EXCEEDS_LIMIT:
            *line = "Size exceeded limit (possibly set to the negative value)";
            break;
        case CAPACITY_EXCEEDS_LIMIT:
            *line = "Capacity exceeded limit (possibly set to the negative value)";
            break;
        case SIZE_EXCEEDS_CAPACITY:
            *line = "Size exceeded capacity";
            break;
        case CALLOC_ERROR:
            *line = "Memory allocation with calloc failed";
            break;
        case REALLOC_ERROR:
            *line = "Memory reallocation failed";
            break;
        case WENT_BEYOND_START:
            *line = "Start boundary value was changed";
            break;
        case WENT_BEYOND_END:
            *line = "End boundary value was changed";
            break;
        case FILE_OPENNING_ERROR:
            *line = "Opening the log file failed";
            break;
        case SIZE_IS_ZERO:
            *line = "Size equals zero";
            break;
        default:
            return 1;
    }
    return 0;
}

StackErr_t StackDump(Stack_t* stack, StackErr_t error)
{
    FILE* stream = fopen("stack.log", "w");
    if (stream == NULL)
    {
        fprintf(stderr, "Can not open stream: stack.log");
        return FILE_OPENNING_ERROR;
    }

    const char* error_str = NULL;
    StackErrToStr(error, &error_str);

    if (error == NULL_STACK)
    {
        fprintf(stream, "%s\n", error_str);
        return STACK_SUCCESS;
    }

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

    if (error == NULL_DATA)
    {
        fprintf(stream, "\t----------------"
                        "\t}"
                        "}");
        return STACK_SUCCESS;
    }

    size_t size = stack->size;
    size_t capacity = stack->capacity;
    item_t* data = stack->data;
    // if size/capacity is wrong, output only 20 first elements
    if (error == SIZE_EXCEEDS_LIMIT)
    {
        size = 10;
    }
    if (error == CAPACITY_EXCEEDS_LIMIT)
    {
        capacity = size;
    }
    if (error == SIZE_EXCEEDS_CAPACITY)
    {
        size = capacity - 1;
    }

    fprintf(stream, "\t\t [0] = %d (CANARY);\n", stack->data[0]);
    for (size_t i = 1; i < size; i++)
    {
        if (data[i] == POISON)
        {
            size = i;
            break;
        }
        fprintf(stream, "\t\t*[%zu] = %d;\n",
                        i, stack->data[i]);
    }
    for (size_t j = size; j < capacity + 1; j++)
    {
        fprintf(stream, "\t\t [%zu] = %d (POISON);\n",
                        j, stack->data[j]);
    }
    fprintf(stream, "\t\t [%zu] = %d (CANARY);\n"
                    "\t}\n"
                    "}",
                    capacity + 1, stack->data[capacity + 1]);
    fclose(stream);

    return STACK_SUCCESS;
}

StackErr_t StackVerify(Stack_t* stack)
{
    if (stack == NULL)
    {
        return NULL_STACK;
    }
    if (stack->data == NULL)
    {
        return NULL_DATA;
    }
    size_t size = stack->size;
    size_t capacity = stack->capacity;
    item_t* data = stack->data;

    // if (capacity == 0)
    // {
    //     return ZERO_CAPACITY;
    // }
    if (size > SIZE_LIMIT)
    {
        return SIZE_EXCEEDS_LIMIT;
    }
    if (capacity > SIZE_LIMIT)
    {
        return CAPACITY_EXCEEDS_LIMIT;
    }
    if (size > capacity)
    {
        return SIZE_EXCEEDS_CAPACITY;
    }
    if (data[0] != CANARY_VALUE)
    {
        return WENT_BEYOND_START;
    }
    if (data[capacity + 1] != CANARY_VALUE)
    {
        return WENT_BEYOND_END;
    }
    return STACK_SUCCESS;
}
