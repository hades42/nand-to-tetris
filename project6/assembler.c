#include <string.h>
#include <stdio.h>

#include "code.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    Parser parser;
    FILE *output_file;
    // use to store the current symbol when reading the current instruction
    char symbol[MAX_LINE_LENGTH];
    char dest[MAX_LINE_LENGTH];
    char comp[MAX_LINE_LENGTH];
    char jump[MAX_LINE_LENGTH];

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <your assembly file path> <your output hack file path>\n", argv[0]);
        return 1;
    }

    if (!parser_open(&parser, argv[1])) {
        return 1;
    }
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
        } else if (type == A_INSTRUCTION) {
            char *symbol_a = parser_symbol(&parser, symbol, sizeof(symbol));
            code_a_instruction(symbol_a, code, sizeof(code));
            fprintf(output_file, "%s\n", code);
        }
    }
    fclose(output_file);
    parser_close(&parser);
    return 0;
}
