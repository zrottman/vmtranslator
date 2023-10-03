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
    {"argument", S_ARGUMENT},
    {"local", S_LOCAL},
    {"static", S_STATIC},
    {"constant", S_CONSTANT},
    {"this", S_THIS},
    {"that", S_THAT},
    {"pointer", S_POINTER},
    {"temp", S_TEMP},
    {NULL, S_UNKNOWN}
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
