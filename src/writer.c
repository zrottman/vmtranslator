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

int write_arithmetic(enum Command command, size_t uid, FILE* fp) {
    switch (command) {

        case C_NEG:
        case C_NOT:
            fputs(UNARY_LOAD, fp);
            switch (command) {
                case C_NEG:
                    fputs("M=-M\n", fp);
                    break;
                case C_NOT:
                    fputs("M=!M\n", fp);
                    break;
            }
            fputs(INC, fp);
            break;

        case C_ADD:
        case C_SUB:
        case C_AND:
        case C_OR:
            fputs(BINARY_LOAD, fp);
            switch (command) {
                case C_ADD:
                    fputs("M=D+M\n", fp);
                    break;
                case C_SUB:
                    fputs("M=D-M\n", fp);
                    break;
                case C_AND:
                    fputs("M=D&M\n", fp);
                    break;
                case C_OR:
                    fputs("M=D|M\n", fp);
                    break;
            }
            fputs(INC, fp);
            break;
        case C_EQ:
        case C_GT:
        case C_LT:
            fputs(BINARY_LOAD, fp);
            fputs("D=D-M\n", fp);
            //fprintf(fp, "@TRUE.%s$%s\n", ); // file id and counter
            fprintf(fp, "@TRUE.%zu\n", uid); // file id also required if UID is reset to 0 each time parser_translate is called

            switch (command) {
                case C_EQ:
                    fputs("D;JEQ\n", fp);
                    break;
                case C_GT:
                    fputs("D;JLT\n", fp);
                    break;
                case C_LT:
                    fputs("D;JGT\n", fp);
                    break;
            }
            fputs("D=0\n", fp);
            //fprintf(fp, "@ENDIF.%s$%s\n", ); // file id and counter
            fprintf(fp, "@ENDIF.%zu\n", uid); // file id also required if UID is reset to 0 each time parser_translate is called

            fputs("0;JMP\n", fp);
            //fprintf(fp, "(TRUE.%s$%s)\n", ); // file id and counter
            fprintf(fp, "(TRUE.%zu)\n", uid); // file id also required if UID is reset to 0 each time parser_translate is called
            fputs("D=-1\n", fp);
            //fprintf(fp, "(ENDIF.%s$%s)\n", ); // file id and counter
            fprintf(fp, "(ENDIF.%zu)\n", uid); // file id also required if UID is reset to 0 each time parser_translate is called
            fputs("@SP\n", fp);
            fputs("A=M\n", fp);
            fputs("M=D\n", fp);
            fputs(INC, fp);
            break;
            
    }
    return 0;
}
