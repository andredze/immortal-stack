#ifndef INPUT_H
#define INPUT_H

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys\stat.h>

#ifdef DEBUG
    #define DPRINTF(...) fprintf(stderr, __VA_ARGS__);
#else
    #define DPRINTF(...) ;
#endif

typedef struct BufferData {
    char* buffer;
    int lines_count;
} BufferData_t;

typedef struct PtrDataParams {
    char** ptrdata;
    int lines_count;
} PtrDataParams_t;

typedef struct FileInfo {
    FILE* stream;
    const char* filepath;
    size_t size;
} FileInfo_t;

typedef struct Context {
    FileInfo_t InputFileInfo;
    BufferData_t BufferData;
    PtrDataParams_t PtrDataParams;
    FileInfo_t OutputFileInfo;
} Context_t;

int read_and_parse_file(Context_t* Context);

int open_file(FileInfo_t* FileInfo, const char* mode);

int count_size(Context_t* Context);

int read_text(Context_t* Context);

int allocate_buffer(Context_t* Context);

int fill_buffer(Context_t* Context);

int parse_text(Context_t* Context);

int count_lines(Context_t* Context);

int allocate_ptrdata(Context_t* Context);

int fill_ptrdata(Context_t* Context);

void add_null_terminators(char* ptr);

#endif /* INPUT_H */
