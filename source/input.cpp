#include "input.h"

int read_and_parse_file(Context_t* Context)
{
    if (open_file(&Context->InputFileInfo, "rb"))
    {
        return 0;
    }
    if (read_text(Context))
    {
        DPRINTF("<Error during parsing text>\n");
        return 0;
    }
    fclose(Context->InputFileInfo.stream);

    if (parse_text(Context))
    {
        DPRINTF("<ptr_data is a NULL pointer>\n");
        return 0;
    }

    return 0;
}

int open_file(FileInfo_t* FileInfo, const char* mode)
{
    assert(FileInfo != NULL);
    assert(FileInfo->filepath != NULL);

    FileInfo->stream = fopen(FileInfo->filepath, mode);
    if (FileInfo->stream == NULL)
    {
        DPRINTF("\n<Can not open the file %s>\n", FileInfo->filepath);
        return 1;
    }

    return 0;
}

int count_size(Context_t* Context)
{
    assert(Context != NULL);
    assert(Context->InputFileInfo.filepath != NULL);

    struct stat fileinfo = {};

    if (stat(Context->InputFileInfo.filepath, &fileinfo) == -1)
    {
        DPRINTF("\n<Error occured with stat()>\n");
        return 1;
    }
    Context->InputFileInfo.size = fileinfo.st_size;
    DPRINTF("size = %zu\n", Context->InputFileInfo.size);

    return 0;
}

int read_text(Context_t* Context)
{
    assert(Context != NULL);

    if (count_size(Context))
    {
        return 1;
    }
    if (allocate_buffer(Context))
    {
        return 1;
    }
    if (fill_buffer(Context))
    {
        return 1;
    }

    return 0;
}

int allocate_buffer(Context_t* Context)
{
    assert(Context != NULL);

    char* buffer = (char*) calloc(Context->InputFileInfo.size / sizeof(char) + 1,
                                  sizeof(char));
    if (buffer == NULL)
    {
        DPRINTF("<Memory allocation for buffer failed>");
        return 1;
    }
    Context->BufferData.buffer = buffer;

    return 0;
}

int fill_buffer(Context_t* Context)
{
    assert(Context != NULL);
    assert(Context->InputFileInfo.stream != NULL);

    size_t fread_return = fread(Context->BufferData.buffer,
                                sizeof(char),
                                Context->InputFileInfo.size,
                                Context->InputFileInfo.stream);
    DPRINTF("fread return = %llu\n", fread_return);

    if (fread_return != Context->InputFileInfo.size)
    {
        if (ferror(Context->InputFileInfo.stream))
        {
            DPRINTF("\n<Error with reading the file>\n");
            DPRINTF("<fread_return = %zu>\n", fread_return);
            return 1;
        }
        if (feof(Context->InputFileInfo.stream))
        {
            DPRINTF("\n<EOF reached>\n");
            return 1;
        }
    }

    DPRINTF("BUFFER:\n");
#ifdef DEBUG
    puts(Context->BufferData.buffer);
#endif

    // adding NULL-terminator
    Context->BufferData.buffer[Context->InputFileInfo.size] = '\0';

    return 0;
}

int parse_text(Context_t* Context)
{
    assert(Context != NULL);

    if (count_lines(Context))
    {
        return 1;
    }
    if (allocate_ptrdata(Context))
    {
        return 1;
    }
    if (fill_ptrdata(Context))
    {
        return 1;
    }

    return 0;
}

int count_lines(Context_t* Context)
{
    assert(Context != NULL);

    char* ptr = Context->BufferData.buffer;
    char* endptr = strchr(ptr, '\0');
    int lines_count = 0;

    while (ptr < endptr)
    {
        assert(ptr != NULL);

        ptr = strchr(ptr, '\n');

        assert(ptr != NULL);

        ptr++; // skip '\n'
        lines_count += 1;
    }

    Context->BufferData.lines_count = lines_count;
    Context->PtrDataParams.lines_count = lines_count;

    return 0;
}

int allocate_ptrdata(Context_t* Context)
{
    assert(Context != NULL);

    Context->PtrDataParams.ptrdata = (char**) calloc(Context->BufferData.lines_count,
                                                     sizeof(char*));
    if (Context->PtrDataParams.ptrdata == NULL)
    {
        DPRINTF("\n<Memory allocation for ptrdata failed>\n");
        return 1;
    }

    return 0;
}

int fill_ptrdata(Context_t* Context)
{
    assert(Context != NULL);

    // fill ptrdata and switch \r and \n to \0
    char* ptr = Context->BufferData.buffer;

    for (int i = 0; i < Context->BufferData.lines_count; i++)
    {
        Context->PtrDataParams.ptrdata[i] = ptr;

        assert(ptr != NULL);

        ptr = strchr(ptr, '\n') + 1;
        if (ptr == NULL)
        {
            DPRINTF("\n<Can not find \\n in buffer>\n");
            return 1;
        }
        add_null_terminators(ptr);
    }

    return 0;
}

void add_null_terminators(char* ptr)
{
    assert(ptr != NULL);

    *(ptr - 1) = '\0'; // switch \n to \0
    if (*(ptr - 2) == '\r') // if windows file (\r\n at end)
    {
        *(ptr - 2) = '\0'; // switch \r to \0
    }
}
