#include "parser.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void remove_comment(char *line) {
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '/' && line[i + 1] == '/') {
            line[i] = '\0';
            break;
        }
    }
}

void remove_whitespace(char *line) {
    int read = 0;
    int write = 0;

    while (line[read] != '\0') {
        if (!isspace(line[read])) {
            line[write] = line[read];
            write++;
        }
        read++;
    }
    line[write] = '\0';
}

// Open the file
int parser_open(Parser *parser, const char *filename) {
    parser->file = fopen(filename, "r");
    if (parser->file == NULL) {
        perror(filename);
        return 0;
    }

    // Represent the current_instruction is empty rather than some random data
    parser->current_instruction[0] = '\0';
    return 1;
}

// Read the next instruction
int parser_advance(Parser *parser) {
    FILE *fp = parser->file;

    while (fgets(parser->current_instruction, sizeof(parser->current_instruction), fp) != NULL) {
        remove_comment(parser->current_instruction);
        remove_whitespace(parser->current_instruction);
        if (parser->current_instruction[0] == '\0') {
           continue;
        }
        return 1;
    };
    return 0;
}

void parser_close(Parser *parser) {
    if (parser->file != NULL) {
        fclose(parser->file);
        parser->file = NULL;
    }
}
