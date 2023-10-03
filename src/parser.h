#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <string.h>
#include "lookup.h"
#include "writer.h"

int  parser_translate(const char* vmfile);
void cleanline(char* line);
void trim(char* line);
void parse_line(char* line);

#endif // PARSER_H
