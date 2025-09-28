#include "stack.h"

#ifdef DEBUG
#define STACK_OK(stack, reason) \
        do { \
            StackErr_t error = STACK_SUCCESS; \
            if ((error = StackIsOk(stack, __FILE__, __func__, __LINE__, reason)) \
                       != STACK_SUCCESS) \
            { \
                return error; \
            } \
        } while (0);
#else
#define STACK_OK(stack, reason) if (StackCheckHash(stack) != STACK_SUCCESS) \
                                { \
                                    return STACK_HASH_CHANGED; \
                                }
#endif

StackErr_t StackCtor(Stack_t* stack, size_t capacity)
{
    if (stack == NULL)
    {
        DPRINTF("<Stack is null pointer>");
        return STACK_IS_NULL;
    }
    stack->capacity = capacity;

    if (stack->capacity > STACK_SIZE_LIMIT)
    {
        return STACK_CAPACITY_EXCEEDS_LIMIT;
    }

    item_t* data = (item_t*) calloc(capacity + 2, sizeof(item_t));
    if (data == NULL)
    {
        DPRINTF("Memory allocation failed");
        return STACK_CALLOC_ERROR;
    }
    data[0] = CANARY_VALUE;
    data[capacity + 1] = CANARY_VALUE;
    stack->size = 0;

    for (size_t i = 1; i < capacity + 1; i++)
    {
        data[i] = POISON;
    }

    stack->data = data;
    stack->hash = Hash(stack->data);

    STACK_OK(stack, reason_end);

    return STACK_SUCCESS;
}

StackErr_t StackRealloc(Stack_t* stack)
{
    STACK_OK(stack, reason_start);

    size_t old_capacity = stack->capacity;
    stack->capacity = old_capacity * 2;
    item_t* data = stack->data;

    data = (item_t*) realloc(data, stack->capacity + 2);
    if (data == NULL)
    {
        DPRINTF("Memory reallocation failed");
        return STACK_REALLOC_ERROR;
    }

    data[0] = CANARY_VALUE;
    data[stack->capacity + 1] = CANARY_VALUE;

    for (size_t i = old_capacity + 1; i < stack->capacity + 1; i++)
    {
        data[i] = POISON;
    }

    stack->hash = Hash(stack->data);
    STACK_OK(stack, reason_end);

    return STACK_SUCCESS;
}

StackErr_t StackPush(Stack_t* stack, item_t item)
{
    STACK_OK(stack, reason_start);

    if (stack->size == stack->capacity)
    {
        if (StackRealloc(stack) != STACK_SUCCESS)
        {
            return STACK_REALLOC_ERROR;
        }
    }
    stack->data[1 + stack->size++] = item;

    DPRINTF("pushed = " SPEC "\n", item);

    stack->hash = Hash(stack->data);
    STACK_OK(stack, reason_end);

    return STACK_SUCCESS;
}

StackErr_t StackPop(Stack_t* stack, item_t* item)
{
    STACK_OK(stack, reason_start);

    if (stack->size == 0)
    {
        DPRINTF("<Pop is impossible with zero size>\n");
        return STACK_SIZE_IS_ZERO;
    }
    *item = stack->data[--stack->size + 1];
    stack->data[stack->size + 1] = POISON;

    DPRINTF("poped = " SPEC "\n", *item);

    stack->hash = Hash(stack->data);
    STACK_OK(stack, reason_end);

    return STACK_SUCCESS;
}

StackErr_t StackDtor(Stack_t* stack)
{
    STACK_OK(stack, reason_start);

    free(stack->data);
    stack->data = NULL;

    return STACK_SUCCESS;
}

#ifdef DEBUG
StackErr_t StackIsOk(Stack_t* stack,
                     const char* file_name,
                     const char* function,
                     int line,
                     const char* reason_of_calling)
{
    stack->var_info.file_name = file_name;
    stack->var_info.function = function;
    stack->var_info.line = line;

    StackErr_t error = STACK_SUCCESS;
    StackErr_t dump_return = STACK_SUCCESS;
    if ((error = StackVerify(stack)) != STACK_SUCCESS)
    {
        DPRINTF("<Error occurred>");
        if ((dump_return = StackDump(stack, error, reason_of_calling)) \
                         != STACK_SUCCESS)
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
        case STACK_IS_NULL:
            *line = "Pointer to the stack structure is NULL";
            break;
        case STACK_DATA_IS_NULL:
            *line = "Pointer to the stack data is NULL";
            break;
        case STACK_SIZE_EXCEEDS_LIMIT:
            *line = "Size exceeded limit (possibly set to the negative value)";
            break;
        case STACK_CAPACITY_EXCEEDS_LIMIT:
            *line = "Capacity exceeded limit (possibly set to the negative value)";
            break;
        case STACK_SIZE_EXCEEDS_CAPACITY:
            *line = "Size exceeded capacity";
            break;
        case STACK_CALLOC_ERROR:
            *line = "Memory allocation with calloc failed";
            break;
        case STACK_REALLOC_ERROR:
            *line = "Memory reallocation failed";
            break;
        case STACK_START_CANARY_RUINED:
            *line = "Start boundary value was changed";
            break;
        case STACK_END_CANARY_RUINED:
            *line = "End boundary value was changed";
            break;
        case STACK_FILE_OPENNING_ERROR:
            *line = "Opening the log file failed";
            break;
        case STACK_SIZE_IS_ZERO:
            *line = "Size equals zero";
            break;
        case STACK_HASH_CHANGED:
            *line = "Hash changed it's value";
            break;
        default:
            return 1;
    }
    return 0;
}

StackErr_t StackDump(Stack_t* stack, StackErr_t error,
                     const char* reason_of_calling)
{
    assert(reason_of_calling != NULL);

    FILE* stream = fopen("stack.log", "w");
    if (stream == NULL)
    {
        DPRINTF("Can not open stream: stack.log");
        return STACK_FILE_OPENNING_ERROR;
    }

    const char* error_str = NULL;
    StackErrToStr(error, &error_str);

    if (error == STACK_IS_NULL)
    {
        fprintf(stream, "%s\n", error_str);
        return STACK_SUCCESS;
    }

    fprintf(stream, "reason: %s\n"
                    "from %s at %s:%d\n"
                    "ERROR %d: %s\n"
                    "%s [%p]\n{\n"
                    "\tsize = %zu;\n"
                    "\tcapacity = %zu;\n"
                    "\tdata [%p];\n\t{\n",
                    reason_of_calling,
                    stack->var_info.function,
                    stack->var_info.file_name,
                    stack->var_info.line,
                    error, error_str,
                    stack->var_info.struct_name, stack,
                    stack->size,
                    stack->capacity,
                    stack->data);

    if (error == STACK_DATA_IS_NULL)
    {
        fprintf(stream, "\t----------------"
                        "\t}"
                        "}");
        return STACK_SUCCESS;
    }

    size_t size = stack->size;
    size_t capacity = stack->capacity;
    item_t* data = stack->data;

    // if size/capacity is wrong, output only 16 first elements
    if (error == STACK_SIZE_EXCEEDS_LIMIT)
    {
        size = 16;
    }
    if (error == STACK_CAPACITY_EXCEEDS_LIMIT)
    {
        capacity = size;
    }
    if (error == STACK_SIZE_EXCEEDS_CAPACITY)
    {
        size = capacity - 1;
    }

    fprintf(stream, "\t\t [0] = " SPEC " (CANARY);\n", data[0]);
    for (size_t i = 1; i < size; i++)
    {
        if (data[i] == POISON)
        {
            size = i;
            break;
        }
        fprintf(stream, "\t\t*[%zu] = " SPEC ";\n", i, data[i]);
    }
    for (size_t j = size; j < capacity + 1; j++)
    {
        fprintf(stream, "\t\t [%zu] = " SPEC " (POISON);\n",
                        j, stack->data[j]);
    }
    fprintf(stream, "\t\t [%zu] = " SPEC " (CANARY);\n"
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
        return STACK_IS_NULL;
    }
    if (stack->data == NULL)
    {
        return STACK_DATA_IS_NULL;
    }
    if (Hash(stack->data) != stack->hash)
    {
        return STACK_HASH_CHANGED;
    }
    if (stack->size > STACK_SIZE_LIMIT)
    {
        return STACK_SIZE_EXCEEDS_LIMIT;
    }
    if (stack->capacity > STACK_SIZE_LIMIT)
    {
        return STACK_CAPACITY_EXCEEDS_LIMIT;
    }
    if (stack->size > stack->capacity)
    {
        return STACK_SIZE_EXCEEDS_CAPACITY;
    }
    if (stack->data[0] != CANARY_VALUE)
    {
        return STACK_START_CANARY_RUINED;
    }
    if (stack->data[stack->capacity + 1] != CANARY_VALUE)
    {
        return STACK_END_CANARY_RUINED;
    }
    return STACK_SUCCESS;
}
#endif

StackErr_t StackCheckCanaries(Stack_t* stack)
{
    assert(stack != NULL);
    assert(stack->data != NULL);

    if (stack->data[0] != CANARY_VALUE)
    {
        fprintf(stderr, "Start canary has changed, stack is now ruined\n");
        return STACK_START_CANARY_RUINED;
    }
    if (stack->data[stack->capacity + 1] != CANARY_VALUE)
    {
        fprintf(stderr, "End canary has changed, stack is now ruined\n");
        return STACK_END_CANARY_RUINED;
    }

    return STACK_SUCCESS;
}

StackErr_t StackCheckHash(Stack_t* stack)
{
    assert(stack != NULL);
    assert(stack->data != NULL);

    if (stack->hash != Hash(stack->data))
    {
        fprintf(stderr, "Hash has changed, stack is now ruined\n");
        return STACK_HASH_CHANGED;
    }

    return STACK_SUCCESS;
}

// djb2
size_t Hash(item_t* data)
{
    char* str = (char*) data;
    size_t hash = 5381;
    char ch = 0;

    while ((ch = *str++) != '\0')
    {
        hash = ((hash << 5) + hash) + ch;
    }

    return hash;
}
