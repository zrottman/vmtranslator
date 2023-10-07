#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <string.h>
#include "lookup.h"
#include "writer.h"

int  parser_translate(const char* vmfile, FILE* fp_out);
void cleanline(char* line);
void trim(char* line);
int parse_line(char* line, FILE* fp_out);

#endif // PARSER_H
