#include "lookup.h"

struct ArgMap commands[] = {
    {"add",      COMMAND, {.c_type = C_ARITHMETIC}, NULL},
    {"sub",      COMMAND, {.c_type = C_ARITHMETIC}, NULL},
    {"neg",      COMMAND, {.c_type = C_ARITHMETIC}, NULL},
    {"eq",       COMMAND, {.c_type = C_ARITHMETIC}, NULL},
    {"gt",       COMMAND, {.c_type = C_ARITHMETIC}, NULL},
    {"lt",       COMMAND, {.c_type = C_ARITHMETIC}, NULL},
    {"and",      COMMAND, {.c_type = C_ARITHMETIC}, NULL},
    {"or",       COMMAND, {.c_type = C_ARITHMETIC}, NULL},
    {"not",      COMMAND, {.c_type = C_ARITHMETIC}, NULL},
    {"push",     COMMAND, {.c_type = C_PUSH},       NULL},
    {"pop",      COMMAND, {.c_type = C_POP},        NULL},
    {"label",    COMMAND, {.c_type = C_LABEL},      NULL},
    {"goto",     COMMAND, {.c_type = C_GOTO},       NULL},
    {"if-goto",  COMMAND, {.c_type = C_IF},         NULL},
    {"function", COMMAND, {.c_type = C_FUNCTION},   NULL},
    {"return",   COMMAND, {.c_type = C_RETURN},     NULL},
    {"call",     COMMAND, {.c_type = C_CALL},       NULL},
    {NULL,       COMMAND, {.c_type = C_UNKNOWN},    NULL}
};

struct ArgMap segments[] = {
    {"argument", SEGMENT, {.s_type = S_ARGUMENT},   "@ARG"},
    {"local",    SEGMENT, {.s_type = S_LOCAL},      "@LCL"},
    {"static",   SEGMENT, {.s_type = S_STATIC},     "@"},
    {"constant", SEGMENT, {.s_type = S_CONSTANT},   NULL},
    {"this",     SEGMENT, {.s_type = S_THIS},       "@THIS"},
    {"that",     SEGMENT, {.s_type = S_THAT},       "@THAT"},
    {"pointer",  SEGMENT, {.s_type = S_POINTER},    "@3"},
    {"temp",     SEGMENT, {.s_type = S_TEMP},       "@5"},
    {NULL,       SEGMENT, {.s_type = S_UNKNOWN},    NULL}
};

/*
struct ArgMap lookup_vm_token(char* token, struct ArgMap *map) {
    size_t i;

    for (i=0; map[i].vm_token != NULL; ++i) {
        if (strcmp(token, map[i].vm_token) == 0) {
            return map[i];
        }
    }

    // if not found, return "uknown" mapping
    return map[i];
}
*/

enum Command lookup_vm_command(char* token) {
    struct ArgMap target = {.vm_token = token};
    struct ArgMap res = lookup(target, commands, VM_TOK);
    return res.com_or_seg.c_type;
}

enum Segment lookup_vm_segment(char* token) {
    struct ArgMap target = {.vm_token = token};
    struct ArgMap res = lookup(target, segments, VM_TOK);
    return res.com_or_seg.s_type;
}

struct ArgMap lookup(struct ArgMap target, struct ArgMap *source, enum SearchOn search_on) {
    size_t i;

    for (i=0; source[i].vm_token != NULL; ++i) {
        switch (search_on) {
            case VM_TOK:
                if (strcmp(target.vm_token, source[i].vm_token) == 0) {
                    return source[i];
                }
                break;
            case ASM_TOK:
                if (strcmp(target.asm_token, source[i].asm_token) == 0) {
                    return source[i];
                }
                break;
            case COM_OR_SEG:
                if (target.arg_type == COMMAND && target.com_or_seg.c_type == source[i].com_or_seg.c_type) {
                    return source[i];
                } else if (target.arg_type == SEGMENT && target.com_or_seg.s_type == source[i].com_or_seg.s_type) {
                    return source[i];
                }
                break;
        }
    }

    // if not found, return "uknown" mapping
    return source[i];
}

const char* lookup_seg_type(enum Segment seg_type) {
    struct ArgMap target = {.arg_type= SEGMENT, .com_or_seg.s_type = seg_type};
    struct ArgMap res = lookup(target, segments, COM_OR_SEG);
    return res.asm_token;

}
/*
struct ArgMap lookup_arg(struct ArgMap arg) {
    struct ArgMap *map = arg.arg_type == COMMAND ? commands : segments;
    size_t i;

    for (i=0; map[i].vm_token != NULL; ++i) {
        switch (arg.arg_type) {
            case COMMAND:
                if (arg.com_or_seg.c_type == map.com_or_seg.c_type) {
                    return map[i].asm_token;
                }
                break;
            case SEGMENT:
                if (arg.com_or_seg.c_type == map.com_or_seg.c_type) {
                    return map[i].asm_token;
                }
                break
        }
        if (strcmp(token, map[i].vm_token) == 0) {
            return map[i];
        }
    }

    // if not found, return "uknown" mapping
    return map[i];

}

char* asm_seg_from_type(enum Segment type) {
    for (int i=0; segments[i].s_type != S_UNKNOWN; ++i) {
        if (segments[i].s_type == type) {
            return segments[i].seg_asm;
        }
    }
    return NULL;
}
*/
