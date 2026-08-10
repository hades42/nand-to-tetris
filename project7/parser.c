#include "parser.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void remove_comment(char *line) {
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '/' && line[i+1] == '/') {
            line[i] = '\0';
            break;
        }
    }
}

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

    while(write > 0 && isspace(line[write-1])) {
        write--;
    }

    line[write] = '\0';
}

int parser_open(Parser *parser, const char *filename) {
    parser->file = fopen(filename, "r");
    if (parser->file == NULL) {
        perror(filename);
        return 0;
    }

    parser->current_instruction[0] = '\0';
    return 1;
}

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

command_type parser_command_type(char *line) {
    char command[32];
    int i;
    for(i = 0; line[i] != ' ' && line[i] != '\0' && i < sizeof(command) - 1; i++) {
        command[i] = line[i];
    }

    command[i] = '\0';
    if(strcmp(command, "push") == 0) {
        return C_PUSH;
    } else if (strcmp(command, "pop") == 0) {
        return C_POP;
    } else if (
        strcmp(command, "add") == 0 || 
        strcmp(command, "sub") == 0 ||
        strcmp(command, "neg") == 0 ||
        strcmp(command, "eq") == 0 ||
        strcmp(command, "gt") == 0 ||
        strcmp(command, "lt") == 0 ||
        strcmp(command, "and") == 0 ||
        strcmp(command, "or") == 0 ||
        strcmp(command, "not") == 0
        )
    {
        return C_ARITHMETIC;
    } else {
        return C_LABEL;
    }
}

char *parser_arg1(Parser *parser, char *arg, size_t arg_size) {
    command_type command_type = parser_command_type(parser->current_instruction);
    char *line = parser->current_instruction;

    int read = 0;
    size_t write = 0;

    if (command_type == C_ARITHMETIC) {
        while (line[read] != '\0' && write + 1 < arg_size) {
            arg[write] = line[read];
            read++;
            write++;
        }
    } else if (command_type == C_PUSH || command_type == C_POP) {
        while (!isspace(line[read])) {
            read++;
        }

        while (!isspace(line[read]) && write + 1 < arg_size) {
            arg[write] = line[read];
            read++;
            write++;
        }
    } else {
        arg[0] = '\0';
        return arg;
    }

    arg[write] = '\0';
    return arg;
}

int *parser_arg2(Parser *parser) {

}