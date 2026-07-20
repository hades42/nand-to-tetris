#include <stdio.h>

#include "parser.h"

int main(int argc, char *argv[]) {
    Parser parser;

    parser_open(&parser, argv[1]);
    while(parser_advance(&parser)) {
        printf("%s\n", parser.current_instruction);
    }
    parser_close(&parser);
    return 0;
}
