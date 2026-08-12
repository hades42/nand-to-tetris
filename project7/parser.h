#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

#define MAX_LINE_LENGTH 256
#define MAX_FILE_LENGTH 1024

typedef struct {
    FILE *file;
    char current_instruction[MAX_LINE_LENGTH];
} Parser;

typedef enum {
    C_ARITHMETIC,
    C_PUSH,
    C_POP,
    C_LABEL,
    C_GOTO,
    C_IF,
    C_FUNCTION,
    C_RETURN,
    C_CALL
} command_type;

int parser_open(Parser *parser, const char *filename);
int parser_advance(Parser *parser);
void parser_close(Parser *parser);
command_type parser_command_type(char *line);
char *parser_arg1(Parser *parser, char *arg, size_t arg_size);
int parser_arg2(Parser *parser);

#endif
