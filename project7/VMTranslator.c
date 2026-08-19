#include "parser.h"
#include "code_writer.h"

// TODO (vannguyen): Need to put some gaurd against file extension and filename
void output_file(const char *file_name, char *output_file, size_t output_file_size) {
    int read = 0;
    size_t write = 0;

    while (file_name[read] != '\0' && file_name[read] != '.' && write + 1 < output_file_size) {
        output_file[write++] = file_name[read++];
    }

    char extension[4] = ".asm";
    for (int i = 0; i < 4; i++) {
        output_file[write++] = extension[i];
    }

    output_file[write] = '\0';
}

int main(int argc, char *argv[])  {
    Parser parser;
    CodeWriter code_writer;
    char arg_1[MAX_LINE_LENGTH];
    char output_file_name[MAX_FILE_LENGTH];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <your assembly file path>\n", argv[0]);
        return 1;
    }

    if (!parser_open(&parser, argv[1])) {
        return 1;
    }

    output_file(argv[1], output_file_name, sizeof(output_file_name));
    code_writer_file_name(&code_writer, argv[1]);
    if (!code_writer_open(&code_writer, output_file_name)) {
        return 1;
    }

    while(parser_advance(&parser)) {
        char *arg1 = parser_arg1(&parser, arg_1, sizeof(arg_1));
        command_type type = parser_command_type(parser.current_instruction);
        if (type == C_PUSH || type == C_POP) {
            int arg2 = parser_arg2(&parser); 
            code_writer_push_pop(&code_writer, type, arg1, arg2);
        } else if (type == C_ARITHMETIC) {
            code_writer_arithmetic(&code_writer, arg1);
        }
    }

    parser_close(&parser);
    code_writer_close(&code_writer);
}
