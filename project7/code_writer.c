#include "code_writer.h"
#include "string.h"

int code_writer_open(CodeWriter *code_writer, const char *filename) {
    // change file extension 
    code_writer->file = fopen(filename, "w");
    code_writer->comparison_id = 0;
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
        char *base = NULL;
        if (strcmp(segment, "local") == 0) {
            base = "LCL";
        } else if (strcmp(segment, "argument") == 0) {
            base = "ARG";
        } else if (strcmp(segment, "this") == 0) {
            base = "THIS";
        } else if (strcmp(segment, "that") == 0) {
            base = "THAT";
        }

        if (base != NULL) {
            fprintf(output_file, "@%d\n", index);
            fprintf(output_file, "D=A\n");
            fprintf(output_file, "@%s\n", base);
            fprintf(output_file, "A=D+M\n");
            fprintf(output_file, "D=M\n");
        }

        if (strcmp(segment, "constant") == 0) {
            fprintf(output_file, "@%d\n", index);        
            fprintf(output_file, "D=A\n");
            fprintf(output_file, "@SP\n");
            fprintf(output_file, "A=M\n");
            fprintf(output_file, "M=D\n");
            fprintf(output_file, "@SP\n");
            fprintf(output_file, "M=M+1\n");
        } else if (
            strcmp(segment, "local") == 0 ||
            strcmp(segment, "argument") == 0 ||
            strcmp(segment, "this") == 0 ||
            strcmp(segment, "that") == 0
        ) {
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

void code_writer_arithmetic (
    CodeWriter *code_writer,
    const char *command
) {
    FILE *output_file = code_writer->file;
    char *jump = NULL;
    if (strcmp(command, "eq") == 0) {
        jump = "JEQ";
    } else if (strcmp(command, "gt") == 0) {
        jump = "JGT";
    } else if (strcmp(command, "lt") == 0) {
        jump = "JLT";
    }
    
    if (strcmp(command, "add") == 0) {
        fprintf(output_file, "@SP\n");
        fprintf(output_file, "M=M-1\n");
        fprintf(output_file, "A=M\n");
        fprintf(output_file, "D=M\n");
        fprintf(output_file, "@SP\n");
        fprintf(output_file, "A=M-1\n");
        fprintf(output_file, "M=D+M\n");
    } else if (strcmp(command, "sub") == 0) {
        fprintf(output_file, "@SP\n");
        fprintf(output_file, "M=M-1\n");
        fprintf(output_file, "A=M\n");
        fprintf(output_file, "D=M\n");
        fprintf(output_file, "@SP\n");
        fprintf(output_file, "A=M-1\n");
        fprintf(output_file, "M=M-D\n");
    } else if (strcmp(command, "neg") == 0) {
        fprintf(output_file, "@SP\n");
        fprintf(output_file, "A=M-1\n");
        fprintf(output_file, "M=-M\n");
    } else if (strcmp(command, "and") == 0) {
        fprintf(output_file, "@SP\n");
        fprintf(output_file, "M=M-1\n");
        fprintf(output_file, "A=M\n");
        fprintf(output_file, "D=M\n");
        fprintf(output_file, "@SP\n");
        fprintf(output_file, "A=M-1\n");
        fprintf(output_file, "M=D&M\n");
    } else if (strcmp(command, "or") == 0) {
        fprintf(output_file, "@SP\n");
        fprintf(output_file, "M=M-1\n");
        fprintf(output_file, "A=M\n");
        fprintf(output_file, "D=M\n");
        fprintf(output_file, "@SP\n");
        fprintf(output_file, "A=M-1\n");
        fprintf(output_file, "M=D|M\n");
    } else if (strcmp(command, "not") == 0) {
        fprintf(output_file, "@SP\n");
        fprintf(output_file, "A=M-1\n");
        fprintf(output_file, "M=!M\n");
    } else if (
        strcmp(command, "eq") == 0 || 
        strcmp(command, "lt") == 0 || 
        strcmp(command, "gt") == 0
    ) {
        fprintf(output_file, "@SP\n");
        fprintf(output_file, "M=M-1\n");
        fprintf(output_file, "A=M\n");
        fprintf(output_file, "D=M\n");
        fprintf(output_file, "@SP\n");
        fprintf(output_file, "A=M-1\n");
        fprintf(output_file, "D=M-D\n");
        fprintf(output_file, "@COMPARE_TRUE_%d\n", code_writer->comparison_id);
        fprintf(output_file, "D;%s\n", jump);
        fprintf(output_file, "@SP\n");
        fprintf(output_file, "A=M-1\n");
        fprintf(output_file, "M=0\n");
        fprintf(output_file, "@COMPARE_END_%d\n", code_writer->comparison_id);
        fprintf(output_file, "0;JMP\n");
        fprintf(output_file, "(COMPARE_TRUE_%d)\n", code_writer->comparison_id);
        fprintf(output_file, "@SP\n");
        fprintf(output_file, "A=M-1\n");
        fprintf(output_file, "M=-1\n");
        fprintf(output_file, "(COMPARE_END_%d)\n", code_writer->comparison_id);

        code_writer->comparison_id++;
    }
}
