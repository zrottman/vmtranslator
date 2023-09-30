#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

enum CommandType { 
    C_ARITHMETIC, C_PUSH, C_POP, C_LABEL, C_GOTO, C_IF,
    C_FUNCTION, C_RETURN, C_CALL
};

int parser_translate(const char* vmfile);

#endif // PARSER_H
