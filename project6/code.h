#ifndef CODE_H
#define CODE_H

#include <stdio.h>

const char *code_dest(const char *mnemonic);
const char *code_comp(const char *mnemonic);
const char *code_jump(const char *mnemonic);
int code_a_instruction (char *symbol, char* code, size_t code_size);
int code_a_value (int value, char *code, size_t code_size);

#endif
