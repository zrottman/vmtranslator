#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int writer_init(const char* vmfilename, FILE** fp);
int writer_close(FILE* fp);
int write_comment(const char* line, FILE* fp);