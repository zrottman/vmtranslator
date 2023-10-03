#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include <string.h>

enum Segment {
    S_ARGUMENT, S_LOCAL, S_STATIC, S_CONSTANT, 
    S_THIS, S_THAT, S_POINTER, S_TEMP, S_UNKNOWN
};

enum Command { 
    C_ARITHMETIC, C_PUSH, C_POP, C_LABEL, C_GOTO, C_IF,
    C_FUNCTION, C_RETURN, C_CALL, C_UNKNOWN
};

struct command_map {
    const char*  key;
    enum Command val;
} command_map;

struct segment_map {
    const char*  key;
    enum Segment val;
    const char*  asm_seg;
} segment_map;

/*
 * commands:
 *  arithmetic: add sub neg eq gt lt and or not
 *  push
 *  pop
 *  label: 
 *  goto: goto
 *  if: if-goto
 *  function
 *  return
 *
 */
enum Command get_command_type(char* token);
enum Segment get_segment_type(char* token);
char* asm_seg_from_type(enum Segment type);




#endif // LINKEDLIST_H
