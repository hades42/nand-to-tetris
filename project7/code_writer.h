#ifndef CODE_WRITER_H
#define CODE_WRITER_H

#include "parser.h"
#include <stdio.h>

typedef struct {
    FILE *file;
} CodeWriter;

int code_writer_open(CodeWriter *code_writer, const char *filename);
void code_writer_close(CodeWriter *code_writer);
void code_writer_push_pop(
    CodeWriter *code_writer, 
    command_type type,
    const char *segment,
    int index
);

#endif