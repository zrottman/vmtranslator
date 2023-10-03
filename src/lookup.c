#include "lookup.h"

struct command_map commands[] = {
    {"add", C_ARITHMETIC},
    {"sub", C_ARITHMETIC},
    {"neg", C_ARITHMETIC},
    {"eq", C_ARITHMETIC},
    {"gt", C_ARITHMETIC},
    {"lt", C_ARITHMETIC},
    {"and", C_ARITHMETIC},
    {"or", C_ARITHMETIC},
    {"not", C_ARITHMETIC},
    {"push", C_PUSH},
    {"pop", C_POP},
    {"label", C_LABEL},
    {"goto", C_GOTO},
    {"if-goto", C_IF},
    {"function", C_FUNCTION},
    {"return", C_RETURN},
    {"call", C_CALL},
    {NULL, C_UNKNOWN}
};

struct segment_map segments[] = {
    {"argument", S_ARGUMENT, "@ARG"},
    {"local", S_LOCAL, "@LCL"},
    {"static", S_STATIC, "@"},
    {"constant", S_CONSTANT, NULL},
    {"this", S_THIS, "@THIS"},
    {"that", S_THAT, "@THAT"},
    {"pointer", S_POINTER, "@3"},
    {"temp", S_TEMP, "@5"},
    {NULL, S_UNKNOWN, NULL}
};

enum Command get_command_type(char* token) {
    for (int i=0; commands[i].key != NULL; ++i) {
        if (strcmp(token, commands[i].key) == 0) {
            return commands[i].val;
        }
    }
    return C_UNKNOWN;
}

enum Segment get_segment_type(char* token) {
    for (int i=0; segments[i].key != NULL; ++i) {
        if (strcmp(token, segments[i].key) == 0) {
            return segments[i].val;
        }
    }
    return S_UNKNOWN;
}

char* asm_seg_from_type(enum Segment type) {
    for (int i=0; segments[i].val != S_UNKNOWN; ++i) {
        if (segments[i].val == type) {
            return segments[i].asm_seg;
        }
    }
    return NULL;
}
