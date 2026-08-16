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

    if (type == C_PUSH) {
        
        // Get value out of segment and store in D
        if (strcmp(segment, "constant") == 0) {
            fprintf(output_file, "@%d\n", index);        
            fprintf(output_file, "D=A\n");
        } else if (base != NULL) {
            fprintf(output_file, "@%d\n", index);
            fprintf(output_file, "D=A\n");
            fprintf(output_file, "@%s\n", base);
            fprintf(output_file, "A=D+M\n");
            fprintf(output_file, "D=M\n");
        } else if (strcmp(segment, "temp") == 0) {
            if (index < 0 || index > 7) {
                fprintf(stderr, "Error: temp index can only go from range 0 to 7");
                return;
            } else {
                fprintf(output_file, "@%d\n", index + 5);
                fprintf(output_file, "D=M\n");
            }
        } else if (strcmp(segment, "pointer") == 0) {
            if (index == 0 || index == 1) {
                if (index == 0) {
                    fprintf(output_file, "@THIS\n");
                } else {
                    fprintf(output_file, "@THAT\n");
                }
                fprintf(output_file, "D=M\n");
            } else {
                fprintf(stderr, "Error: pointer index can only be 0 or 1");
                return;
            }
        } else {
            fprintf(stderr, "Error: Unknown segment in push");
            return;
        }

        // Push value in D to stack and increase stack pointer
        fprintf(output_file, "@SP\n");
        fprintf(output_file, "A=M\n");
        fprintf(output_file, "M=D\n");
        fprintf(output_file, "@SP\n");
        fprintf(output_file, "M=M+1\n");
    } else if (type == C_POP) {
        if (strcmp(segment, "constant") == 0) {
            fprintf(stderr, "Error: cannot pop to constant segment\n");
            return;
        }

        // Save the segment address into R13
        if (strcmp(segment, "temp") == 0) {
            if (index < 0 || index > 7) {
                fprintf(stderr, "Error: temp index can only go from range 0 to 7");
                return;
            } else {
                fprintf(output_file, "@%d\n", index + 5);
                fprintf(output_file, "D=A\n");
                fprintf(output_file, "@R13\n");
                fprintf(output_file, "M=D\n");
            }
        } else if (strcmp(segment, "pointer") == 0) {
            if (index == 0 || index == 1) {
                if (index == 0) {
                    fprintf(output_file, "@THIS\n");
                } else {
                    fprintf(output_file, "@THAT\n");
                }
                fprintf(output_file, "D=A\n");
                fprintf(output_file, "@R13\n");
                fprintf(output_file, "M=D\n");
            } else {
                fprintf(stderr, "Error: pointer index can only be 0 or 1");
                return;
            }
        } else if (base != NULL) {
            fprintf(output_file, "@%d\n", index);
            fprintf(output_file, "D=A\n");
            fprintf(output_file, "@%s\n", base);
            fprintf(output_file, "A=D+M\n");
            fprintf(output_file, "D=A\n");
            fprintf(output_file, "@R13\n");
            fprintf(output_file, "M=D\n");
        } else {
            fprintf(stderr, "Error: Unknown segment in pop");
            return;
        }
        

        // Assign value from stack to segment memory that save in R13
        fprintf(output_file, "@SP\n");
        fprintf(output_file, "M=M-1\n");
        fprintf(output_file, "A=M\n");
        fprintf(output_file, "D=M\n");
        fprintf(output_file, "@R13\n");
        fprintf(output_file, "A=M\n");
        fprintf(output_file, "M=D\n");
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
