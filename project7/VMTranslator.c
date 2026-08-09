#include "parser.h"

int main(int argc, char *argv[])  {
    Parser parser;

    if (!parser_open(&parser, argv[1])) {
        return 1;
    }

    while(parser_advance(&parser)) {
        command_type command_type = parser_command_type(&parser);
    }

    parser_close(&parser);
}
