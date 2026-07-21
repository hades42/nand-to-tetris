#include <stdio.h>

#include "parser.h"

int main(int argc, char *argv[]) {
    Parser parser;
    // use to store the current symbol when reading the current instruction
    char symbol[MAX_LINE_LENGTH];

    parser_open(&parser, argv[1]);
    while(parser_advance(&parser)) {
        printf("%s (%d) | symbol: %s\n",
            parser.current_instruction,
            parser_instruction_type(&parser),
            parser_symbol(&parser, symbol, sizeof(symbol))
        );
    }
    parser_close(&parser);
    return 0;
}
