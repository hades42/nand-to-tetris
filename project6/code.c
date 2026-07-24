#include "code.h"
#include <stdlib.h>
#include <string.h>

const char *code_dest(const char *mnemonic) {
    if (strcmp(mnemonic, "D") == 0) {
        return "010";
    } else if (strcmp(mnemonic, "M") == 0) {
        return "001";
    } else if ((strcmp(mnemonic, "MD") == 0) || (strcmp(mnemonic, "DM") == 0)) {
        return "011";
    } else if (strcmp(mnemonic, "A") == 0) {
        return "100";
    } else if ((strcasecmp(mnemonic, "AM") == 0) || (strcasecmp(mnemonic, "MA") == 0) ) {
        return "101";
    } else if ((strcmp(mnemonic, "AD") == 0) || (strcmp(mnemonic, "DA ") == 0)) {
        return "110";
    } else if ((strcmp(mnemonic, "ADM") == 0) || (strcmp(mnemonic, "AMD") == 0)) {
        return "111";
    } else {
        return "000";
    }
}

const char *code_jump(const char *mnemonic) {
   if (strcmp(mnemonic, "JGT") == 0) {
       return "001";
   } else if (strcmp(mnemonic, "JEQ") == 0) {
       return "010";
   } else if (strcmp(mnemonic, "JGE") == 0) {
       return "011";
   } else if (strcmp(mnemonic, "JLT") == 0) {
       return "100";
   } else if (strcmp(mnemonic, "JNE") == 0) {
       return "101";
   } else if (strcmp(mnemonic, "JLE") == 0) {
       return "110";
   } else if (strcmp(mnemonic, "JMP") == 0) {
       return "111";
   } else {
       return "000";
   }
}

const char *code_comp(const char *mnemonic) {
    if (strcmp(mnemonic, "0") == 0) return "0101010";
    else if (strcmp(mnemonic, "1") == 0) return "0111111";
    else if (strcmp(mnemonic, "-1") == 0) return "0111010";
    else if (strcmp(mnemonic, "D") == 0) return "0001100";
    else if (strcmp(mnemonic, "A") == 0) return "0110000";
    else if (strcmp(mnemonic, "M") == 0) return "1110000";
    else if (strcmp(mnemonic, "!D") == 0) return "0001101";
    else if (strcmp(mnemonic, "!A") == 0) return "0110001";
    else if (strcmp(mnemonic, "!M") == 0) return "1110001";
    else if (strcmp(mnemonic, "-D") == 0) return "0001111";
    else if (strcmp(mnemonic, "-A") == 0) return "0110011";
    else if (strcmp(mnemonic, "-M") == 0) return "1110011";
    else if (strcmp(mnemonic, "D+1") == 0) return "0011111";
    else if (strcmp(mnemonic, "A+1") == 0) return "0110111";
    else if (strcmp(mnemonic, "M+1") == 0) return "1110111";
    else if (strcmp(mnemonic, "D-1") == 0) return "0001110";
    else if (strcmp(mnemonic, "A-1") == 0) return "0110010";
    else if (strcmp(mnemonic, "M-1") == 0) return "1110010";
    else if (strcmp(mnemonic, "D+A") == 0) return "0000010";
    else if (strcmp(mnemonic, "D+M") == 0) return "1000010";
    else if (strcmp(mnemonic, "D-A") == 0) return "0010011";
    else if (strcmp(mnemonic, "D-M") == 0) return "1010011";
    else if (strcmp(mnemonic, "A-D") == 0) return "0000111";
    else if (strcmp(mnemonic, "M-D") == 0) return "1000111";
    else if (strcmp(mnemonic, "D&A") == 0) return "0000000";
    else if (strcmp(mnemonic, "D&M") == 0) return "1000000";
    else if (strcmp(mnemonic, "D|A") == 0) return "0010101";
    else if (strcmp(mnemonic, "D|M") == 0) return "1010101";
    else return "";
}

// For A Instruction only
int code_a_instruction (char *symbol, char* code, size_t code_size) {
    char *endptr;
    long value = strtol(symbol, &endptr, 10);

    if (code_size < 17) {
        return 0;
    }

    if (*endptr != '\0' || value > 32767 || value < 0 || endptr == symbol) {
        return 0;
    } else {
        for (int bit = 15; bit >= 0; bit--) {
            code[15 - bit] = ((value >> bit) & 1) ? '1' : '0';
        }
        code[16] = '\0';
        return 1;
    }
}
