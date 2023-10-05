#include "utils.h"
#include "lookup.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int writer_init(const char* vmfilename, FILE** fp);
int writer_close(FILE* fp);
int write_comment(const char* line, FILE* fp);
int write_pushpop(enum Command command, enum Segment segment, char* idx, FILE* fp);
int write_arithmetic(enum Command command, size_t uid, FILE* fp);
