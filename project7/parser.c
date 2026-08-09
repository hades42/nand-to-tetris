#include "parser.h"
#include <stdio.h>
#include <ctype.h>

void remove_comment(char *line) {
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '/' && line[i+1] == '/') {
            line[i] = '\0';
            break;
        }
    }
};

// Remove trailing space
void remove_trailing_whitespace(char *line) {
    int read = 0;
    int write = 0;

    while(line[read] != '\0' && isspace(line[read])) {
        read++;
    }

    while(line[read] != '\0') {
        line[write] = line[read]; 
        read++;
        write++;
    }

    while(write > 0 && isspace(line[write])) {
        write--;
    }

    line[write] = '\0';
};

int parser_open(Parser *parser, const char *filename) {
    parser->file = fopen(filename, "r");
    if (parser->file == NULL) {
        perror(filename);
        return 0;
    }

    parser->current_instruction[0] = '\0';
    return 1;
};

int parser_advance(Parser *parser) {
    FILE *fp = parser->file;

    while(fgets(parser->current_instruction, sizeof(parser->current_instruction), fp) != NULL) {
        remove_comment(parser->current_instruction);
        remove_trailing_whitespace(parser->current_instruction);
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