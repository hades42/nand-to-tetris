#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

#define MAX_LINE_LENGTH 256

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
command_type parser_command_type(Parser *parser);
char *parser_arg1(char *current_command);
char *parser_arg2(char *current_command);

#endif
