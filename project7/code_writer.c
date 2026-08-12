#include "code_writer.h"
#include "string.h"

int code_writer_open(CodeWriter *code_writer, const char *filename) {
    // change file extension 
    code_writer->file = fopen(filename, "w");
    if (code_writer->file == NULL) {
        perror(filename);
        return 0;
    }
    return 1;
}

void code_writer_close(CodeWriter *code_writer) {
    if (code_writer->file != NULL) {
        fclose(code_writer->file);
        code_writer->file = NULL;
    }
}

void code_writer_push_pop(
    CodeWriter *code_writer, 
    command_type type,
    const char *segment,
    int index
) {
    FILE *output_file = code_writer->file; 
    if (type == C_PUSH) {
        if (strcmp(segment, "constant") == 0) {
            fprintf(output_file, "@%d\n", index);        
            fprintf(output_file, "D=A\n");
            fprintf(output_file, "@SP\n");
            fprintf(output_file, "A=M\n");
            fprintf(output_file, "M=D\n");
            fprintf(output_file, "@SP\n");
            fprintf(output_file, "M=M+1\n");
        }
    } else if (type == C_POP) {

    } else if (type == C_ARITHMETIC) {

    }
}
