#include <string.h>
#include <stdio.h>

#include "code.h"
#include "parser.h"
#include "symbol_table.h"

int main(int argc, char *argv[]) {
    Parser parser;
    FILE *output_file;
    // use to store the current symbol when reading the current instruction
    char symbol[MAX_LINE_LENGTH];
    char dest[MAX_LINE_LENGTH];
    char comp[MAX_LINE_LENGTH];
    char jump[MAX_LINE_LENGTH];

    symbol_table table;
    symbol_table_init(&table);
    // Pre-defined symbols
    symbol_table_add(&table, "SP", 0);
    symbol_table_add(&table, "LCL", 1);
    symbol_table_add(&table, "ARG", 2);
    symbol_table_add(&table, "THIS", 3);
    symbol_table_add(&table, "THAT", 4);
    symbol_table_add(&table, "R0", 0);
    symbol_table_add(&table, "R1", 1);
    symbol_table_add(&table, "R2", 2);
    symbol_table_add(&table, "R3", 3);
    symbol_table_add(&table, "R4", 4);
    symbol_table_add(&table, "R5", 5);
    symbol_table_add(&table, "R6", 6);
    symbol_table_add(&table, "R7", 7);
    symbol_table_add(&table, "R8", 8);
    symbol_table_add(&table, "R9", 9);
    symbol_table_add(&table, "R10", 10);
    symbol_table_add(&table, "R11", 11);
    symbol_table_add(&table, "R12", 12);
    symbol_table_add(&table, "R13", 13);
    symbol_table_add(&table, "R14", 14);
    symbol_table_add(&table, "R15", 15);
    symbol_table_add(&table, "SCREEN", 16384);
    symbol_table_add(&table, "KBD", 24576);

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <your assembly file path> <your output hack file path>\n", argv[0]);
        return 1;
    }

    if (!parser_open(&parser, argv[1])) {
        return 1;
    }

    // FIRST PASS
    int rom_address = 0;
    while(parser_advance(&parser)) {
        instruction_type type = parser_instruction_type(&parser);
        if (type == C_INSTRUCTION) {
            rom_address++;
        } else if (type == A_INSTRUCTION) {
            rom_address++;
        } else {
            char *parsed_symbol = parser_symbol(&parser, symbol, sizeof(symbol));
            symbol_table_add(&table, parsed_symbol, rom_address);
        }
    }
    parser_close(&parser);

    // SECOND PASS
    int next_available_variable = 16;

    parser_open(&parser, argv[1]);
    // Open output file to write into
    output_file = fopen(argv[2], "w");
    if (output_file == NULL) {
        perror(argv[2]);
        parser_close(&parser);
        return 1;
    }
    while(parser_advance(&parser)) {
        instruction_type type = parser_instruction_type(&parser);
        char code[MAX_LINE_LENGTH] = "";

        if (type == C_INSTRUCTION) {
            char *parse_dest = parser_dest(&parser, dest, sizeof(dest));
            char *parse_comp = parser_comp(&parser, comp, sizeof(comp));
            char *parse_jump = parser_jump(&parser, jump, sizeof(jump));

            const char *dest_code = code_dest(parse_dest);
            const char *comp_code = code_comp(parse_comp);
            const char *jump_code = code_jump(parse_jump);

            strcat(code, "111");
            strcat(code, comp_code);
            strcat(code, dest_code);
            strcat(code, jump_code);
            fprintf(output_file, "%s\n", code);
            // fprintf(output_file, "%s | %s\n", parser.current_instruction, code);
        } else if (type == A_INSTRUCTION) {
            char *symbol_a = parser_symbol(&parser, symbol, sizeof(symbol));
            if(!code_a_instruction(symbol_a, code, sizeof(code))) {
                if(symbol_table_contain(&table, symbol_a)) {
                    int symbol_address = symbol_table_get_address(&table, symbol_a);
                    code_a_value(symbol_address, code, sizeof(code));
                } else {
                    symbol_table_add(&table, symbol_a, next_available_variable);
                    code_a_value(next_available_variable, code, sizeof(code));
                    next_available_variable++;
                }
            }
            fprintf(output_file, "%s\n", code);
            // fprintf(output_file, "%s | %s\n",parser.current_instruction, code);
        }
    }
    fclose(output_file);
    parser_close(&parser);
    return 0;
}
