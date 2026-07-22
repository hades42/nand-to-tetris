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

instruction_type parser_instruction_type(Parser *parser) {
    char *current_instr = parser -> current_instruction;
    switch (current_instr[0]) {
        case '@':
            return A_INSTRUCTION;
            break;
        case '(':
            return L_INSTRUCTION;
            break;
        default:
            return C_INSTRUCTION;
            break;
    }
}

char *parser_symbol(Parser *parser, char *symbol, size_t symbol_size) {
    char *current_instr = parser->current_instruction;
    int read = 1;
    size_t write = 0;
    instruction_type type = parser_instruction_type(parser);

    if (symbol_size == 0) {
        return NULL;
    }
    // write + 1 < symbol_size is require to have space for '\0'
    if (type == A_INSTRUCTION) {
        while(current_instr[read] != '\0' && write + 1 < symbol_size) {
            symbol[write] = current_instr[read];
            read++;
            write++;
        }
    } else if (type == L_INSTRUCTION) {
        while (current_instr[read] != ')' && current_instr[read] != '\0' && write + 1 < symbol_size) {
            symbol[write] = current_instr[read];
            read++;
            write++;
        }
    } else {
        symbol[0] = '\0';
        return symbol;
    }

    symbol[write] = '\0';
    return symbol;
}

char *parser_dest(Parser *parser, char *dest, size_t dest_size) {
    char *current_instr = parser->current_instruction;
    instruction_type type = parser_instruction_type(parser);
    int read = 0;
    size_t write = 0;

    if (dest_size == 0) {
        return NULL;
    }

    if (type == C_INSTRUCTION) {
        const char *find_equal = strchr(current_instr, '=');
        if (find_equal != NULL) {
            while (current_instr[read] != '=' && current_instr[read] != '\0' && write + 1 < dest_size) {
                dest[write] = current_instr[read];
                read++;
                write++;
            }
        } else {
            dest[0] = '\0';
            return dest;
        }
    } else {
        dest[0] = '\0';
        return dest;
    }

    dest[write] = '\0';
    return dest;
}

char *parser_comp(Parser *parser, char *comp, size_t comp_size) {
    char *current_instr = parser->current_instruction;
    instruction_type type = parser_instruction_type(parser);
    int read = 0;
    size_t write = 0;

    if (comp_size == 0) {
        return NULL;
    }

    if (type == C_INSTRUCTION) {
        char *find_equal = strchr(current_instr, '=');
        // If there is an equal sign, the starting point should be after the "=", otherwise
        // otherwise, it would be at the beginning
        char *start = find_equal != NULL ? find_equal + 1 : current_instr;
        while (start[read] != ';' && start[read] != '\0' && write + 1 < comp_size) {
            comp[write] = start[read];
            write++;
            read++;
        }
    } else {
        comp[0] = '\0';
        return comp;
    }

    comp[write] = '\0';
    return comp;
}

char *parser_jump(Parser *parser, char *jump, size_t jump_size) {
    char *current_instr = parser->current_instruction;
    instruction_type type = parser_instruction_type(parser);
    int read = 0;
    size_t write = 0;

    if (jump_size == 0) {
        return NULL;
    }

    if (type == C_INSTRUCTION) {
        char *find_semicolon = strchr(current_instr, ';');
        if (find_semicolon == NULL) {
            jump[0] = '\0';
            return jump;
        }

        char *start = find_semicolon + 1;
        while (start[read] != '\0' && write + 1 < jump_size) {
            jump[write] = start[read];
            write++;
            read++;
        }
    } else {
        jump[0] = '\0';
        return jump;
    }

    jump[write] = '\0';
    return jump;
}
