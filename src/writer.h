#include "utils.h"
#include "lookup.h"
#include <stdlib.h>
#include <stdio.h>

int writer_init(const char* asmfilename, FILE** fp);
int writer_close(FILE* fp);

int write_comment(const char* line, FILE* fp);
int write_bootstrap(FILE* fp);
int write_pushpop(enum Command command, enum Segment segment, char* idx, FILE* fp);
int write_arithmetic(enum Command command, size_t uid, FILE* fp);
int write_label(char* label, FILE* fp);
int write_if(char* label, FILE* fp);
int write_goto(char* label, FILE* fp);
int write_function(char* label, char* n_locals, FILE* fp);
int write_call(char* f_name, char* n_args, size_t uid, FILE* fp);
int write_return(FILE *fp);
