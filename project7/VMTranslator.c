#include "parser.h"
#include "code_writer.h"

char * output_file(const )

int main(int argc, char *argv[])  {
    Parser parser;
    CodeWriter code_writer;
    char arg_1[MAX_LINE_LENGTH];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <your assembly file path>\n", argv[0]);
        return 1;
    }

    if (!parser_open(&parser, argv[1])) {
        return 1;
    }


    while(parser_advance(&parser)) {
        char *arg1 = parser_arg1(&parser, arg_1, sizeof(arg_1));
        int arg2 = parser_arg2(&parser); 
    }

    parser_close(&parser);
}
