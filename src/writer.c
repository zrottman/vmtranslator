#include "writer.h"

#define INC         "@SP\nM=M+1\n"
#define DEC         "@SP\nM=M-1\n"
#define PUSH        "@SP\nA=M\nM=D\n" INC
#define POP         "@R13\nM=D\n" DEC "A=M\nD=M\n@R13\nA=M\nM=D\n"
#define UNARY_LOAD  DEC "A=M\n"
#define BINARY_LOAD UNARY_LOAD "D=M\n" UNARY_LOAD


int writer_init(const char* vmfilename, FILE** fp) {
    size_t len;
    char*  asmfilename;
    char*  p;
 
    len = strlen(vmfilename);
    asmfilename = (char*)malloc(len * sizeof(char) + 1);
    strcpy(asmfilename, vmfilename);

    p = NULL;
    
    p = rfind(asmfilename, '.');
    strcpy(p, ".asm\0");


    printf("Write file: %s\n", asmfilename);

    if ((*fp = fopen(asmfilename, "w")) == NULL) {
        return 1;
    }
    return 0;
}

int writer_close(FILE* fp) {
    fclose(fp);
    return 0;
}

int write_comment(const char* line, FILE* fp) {

    fputs("// ", fp);
    fputs(line, fp);
    fputs("\n", fp);

    return 0;
}


int write_pushpop(enum Command command, enum Segment segment, char* idx, FILE* fp) {
    switch (segment) {

        case S_LOCAL:
        case S_ARGUMENT:
        case S_THIS:
        case S_THAT:
            fprintf(fp, "@%s\n", idx);
            fputs("D=A\n", fp);
            fprintf(fp, "%s\n", (lookup_seg_type(segment)));
            switch (command) {
                case C_PUSH:
                    fputs("A=D+M\n", fp);
                    fputs("D+M\n", fp);
                    fputs(PUSH, fp);
                    break;
                case C_POP:
                    fputs("D=D+M\n", fp);
                    fputs(POP, fp);
                    break;
            }
            break;

        case S_POINTER:
        case S_TEMP:
            fprintf(fp, "@%s\n", idx);
            fputs("D=A\n", fp);
            fprintf(fp, "%s\n", (lookup_seg_type(segment)));
            switch (command) {
                case C_PUSH:
                    fputs("A=D+A\n", fp);
                    fputs("D+M\n", fp);
                    fputs(PUSH, fp);
                    break;
                case C_POP:
                    fputs("D=D+A\n", fp);
                    fputs(POP, fp);
                    break;
            }
            break;

        case S_STATIC:
            fprintf(fp, "%s\n", (lookup_seg_type(segment)));
            switch (command) {
                case C_PUSH:
                    fputs("D=M\n", fp);
                    fputs(PUSH, fp);
                    break;
                case C_POP:
                    fputs("D=A\n", fp);
                    fputs(POP, fp);
                    break;
            }
            break;

        case S_CONSTANT:
            if (command != C_PUSH) {
                // error
            }
            fprintf(fp, "@%s\n", idx);
            fputs("D=A\n", fp);
            fputs(PUSH, fp);
            break;

        case S_UNKNOWN:
            // error
            break;
    }
    return 0;
}

int write_arithmetic(enum Command command, FILE* fp) {
    switch (command) {

        case NEG:
        case NOT:
            fputs(UNARY_LOAD, fp);
            switch (command) {
                case NEG:
                    fputs("M=-M\n", fp);
                    break;
                case NOT:
                    fputs("M=!M\n", fp);
                    break;
            }
            fputs(INC, fp);
            break;

        case ADD;
        case SUB;
        case AND;
        case OR;
            fputs(BINARY_LOAD, fp);
            switch (command) {
                case ADD:
                    fputs("M=D+M\n");
                    break;
                case SUB:
                    fputs("M=D-M\n");
                    break;
                case AND:
                    fputs("M=D&M\n");
                    break;
                case OR:
                    fputs("M=D|M\n");
                    break;
            }
            fputs(INC, fp);
            break;
        case EQ;
        case GT;
        case LT;
            break;
            
    }
    return 0;
}
