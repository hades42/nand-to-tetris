#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

#define MAX_LINE_LENGTH 256

typedef enum {
    A_INSTRUCTION, // 0
    C_INSTRUCTION, // 1
    L_INSTRUCTION, // 2
} instruction_type;

typedef struct {
    FILE *file;
    char current_instruction[MAX_LINE_LENGTH];
} Parser;

int parser_open(Parser *parser, const char *filename);
int parser_advance(Parser *parser);
void parser_close(Parser *parser);
instruction_type parser_instruction_type(Parser *parser);
char *parser_symbol(Parser *parser, char *symbol, size_t symbol_size);

// C-instruction: dest=comp;jump
char *parser_dest(Parser *parser, char *dest, size_t dest_size);
char *parser_comp(Parser *parser, char *comp, size_t comp_size);
char *parser_jump(Parser *parser, char *jump, size_t jump_size);

#endif
