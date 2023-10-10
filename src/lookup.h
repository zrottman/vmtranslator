#ifndef LOOKUP_H
#define LOOKUP_H

#include <stdio.h>
#include <string.h>

enum ArgType { COMMAND, SEGMENT};

enum Segment {
    S_ARGUMENT, S_LOCAL, S_STATIC, S_CONSTANT, 
    S_THIS, S_THAT, S_POINTER, S_TEMP, S_UNKNOWN
};

enum Command { 
    C_ADD, C_SUB, C_NEG, C_EQ, C_GT, C_LT, C_AND, C_OR, C_NOT, 
    C_PUSH, C_POP, C_LABEL, C_GOTO, C_IF,
    C_FUNCTION, C_RETURN, C_CALL, C_UNKNOWN
};

enum SearchOn { VM_TOK, ASM_TOK, COM_OR_SEG };

union ComOrSeg {
    enum Segment s_type;
    enum Command c_type;
};

struct ArgMap {
    const char*    vm_token;
    enum ArgType   arg_type;
    union ComOrSeg com_or_seg;
    const char*    asm_token;
};


struct ArgMap lookup(struct ArgMap target, struct ArgMap *source, enum SearchOn search_on);

enum Command  lookup_vm_command(char* token);
enum Segment  lookup_vm_segment(char* token);
const char*   lookup_seg_type(enum Segment seg_type);

#endif // LOOKUP_H
