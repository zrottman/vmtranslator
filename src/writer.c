#include "writer.h"

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
            if (command == C_PUSH) {
                fprintf(fp, "@%s\n", idx);
                fputs("D=A\n", fp);
                fprintf(fp, "%s\n", (asm_seg_from_type(segment)));
                fputs("A=D+M\n", fp);
                fputs("D+M\n", fp);

                // push
                fputs("@SP\n", fp);
                fputs("A=M\n", fp);
                fputs("M=D\n", fp);
                fputs("@SP\n", fp);
                fputs("M=M+1\n", fp);
            } else if (command == C_POP) {

            }
            break;
        case S_POINTER:
        case S_TEMP:
            break;
        case S_STATIC:
            break;
        case S_CONSTANT:
            break;
        case S_UNKNOWN:
            break;
    }
    return 0;
}
