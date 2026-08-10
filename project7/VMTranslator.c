#include "parser.h"

int main(int argc, char *argv[])  {
    Parser parser;
    char* arg_1, arg_2;

    if (!parser_open(&parser, argv[1])) {
        return 1;
    }

    while(parser_advance(&parser)) {
        command_type command_type = parser_command_type(&parser);
        arg_1 = parser_arg1(parser.current_instruction);
        if (command_type == C_PUSH || command_type == C_POP || command_type == C_FUNCTION || command_type == C_CALL) {
            arg_2 = parser_arg2(parser.current_instruction);
        }
    }

    parser_close(&parser);
}
