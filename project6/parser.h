#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

#define MAX_LINE_LENGTH 256

typedef struct {
    FILE *file;
    char current_instruction[MAX_LINE_LENGTH];
} Parser;

int parser_open(Parser *parser, const char *filename);
int parser_advance(Parser *parser);
void parser_close(Parser *parser);

#endif
