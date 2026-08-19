#ifndef CODE_WRITER_H
#define CODE_WRITER_H

#include "parser.h"
#include <stdio.h>

typedef struct {
    FILE *file;
    int comparison_id;
    char file_name[MAX_FILE_LENGTH];
} CodeWriter;

int code_writer_open(CodeWriter *code_writer, const char *filename);
void code_writer_close(CodeWriter *code_writer);
void code_writer_push_pop(
    CodeWriter *code_writer, 
    command_type type,
    const char *segment,
    int index
);
void code_writer_file_name (
    CodeWriter *code_writer,
    const char *intput_path
);

void code_writer_arithmetic(
    CodeWriter *code_writer,
    const char *command
);

#endif
