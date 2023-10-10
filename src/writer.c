#include "writer.h"

#define INC         "@SP\nM=M+1\n"
#define DEC         "@SP\nM=M-1\n"
#define PUSH        "@SP\nA=M\nM=D\n" INC
#define POP         "@R13\nM=D\n" DEC "A=M\nD=M\n@R13\nA=M\nM=D\n"
#define UNARY_LOAD  DEC "A=M\n"
#define BINARY_LOAD UNARY_LOAD "D=M\n" UNARY_LOAD

int writer_init(const char* asmfilename, int nobootstrap, FILE** fp) {
    size_t len;

    printf("Write file: %s\n", asmfilename);

    // open file
    if ((*fp = fopen(asmfilename, "w")) == NULL) {
        return 1;
    }

    // write bootstrap
    if (nobootstrap == 0 && write_bootstrap(*fp) != 0) {
        writer_close(*fp);
        return 2;
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

int write_bootstrap(FILE *fp) {
    if (write_comment("Start Bootstrap", fp) != 0) {
        return 1;
    }
    fputs("@256\n", fp);
    fputs("D=A\n", fp);
    fputs("@SP\n", fp);
    fputs("M=D\n", fp);
    if (write_call("Sys.init", "0", 0, "Bootstrap", fp) != 0) {
        return 2;
    }
    return 0;
}

int write_pushpop(enum Command command, enum Segment segment, char* idx, char* file_id, FILE* fp) {
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
                    fputs("D=M\n", fp);
                    fputs(PUSH, fp);
                    break;
                case C_POP:
                    fputs("D=D+M\n", fp);
                    fputs(POP, fp);
                    break;
                default:
                    return 1;
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
                    fputs("D=M\n", fp);
                    fputs(PUSH, fp);
                    break;
                case C_POP:
                    fputs("D=D+A\n", fp);
                    fputs(POP, fp);
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case S_STATIC:
            // TODO: need first line to be @<filename>.<idx>; no need for lookup func
            fprintf(fp, "@%s.%s\n", file_id, idx);
            //fprintf(fp, "%s\n", (lookup_seg_type(segment)));
            switch (command) {
                case C_PUSH:
                    fputs("D=M\n", fp);
                    fputs(PUSH, fp);
                    break;
                case C_POP:
                    fputs("D=A\n", fp);
                    fputs(POP, fp);
                    break;
                default:
                    return 3;
                    break;
            }
            break;
        case S_CONSTANT:
            if (command != C_PUSH) {
                // error
                return 4;
            }
            fprintf(fp, "@%s\n", idx);
            fputs("D=A\n", fp);
            fputs(PUSH, fp);
            break;
        case S_UNKNOWN:
        default:
            // error
            return 5;
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
                default:
                    return 1;
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
                    fputs("M=M-D\n", fp);
                    break;
                case C_AND:
                    fputs("M=D&M\n", fp);
                    break;
                case C_OR:
                    fputs("M=D|M\n", fp);
                    break;
                default:
                    return 2;
                    break;
            }
            fputs(INC, fp);
            break;
        case C_EQ:
        case C_GT:
        case C_LT:
            fputs(BINARY_LOAD, fp);
            fputs("D=D-M\n", fp);
            fprintf(fp, "@TRUE.%zu\n", uid);
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
                default:
                    return 3;
                    break;
            }
            fputs("D=0\n", fp);
            fprintf(fp, "@ENDIF.%zu\n", uid);
            fputs("0;JMP\n", fp);
            fprintf(fp, "(TRUE.%zu)\n", uid);
            fputs("D=-1\n", fp);
            fprintf(fp, "(ENDIF.%zu)\n", uid);
            fputs("@SP\n", fp);
            fputs("A=M\n", fp);
            fputs("M=D\n", fp);
            fputs(INC, fp);
            break;
        default:
            return 4;
            break;
    }
    return 0;
}

int write_label(char* label, FILE* fp) {
    fprintf(fp, "(%s)\n", label);
    return 0;
}

int write_if(char* label, FILE* fp) {
    fputs(DEC, fp);
    fputs("A=M\n", fp);
    fputs("D=M\n", fp);
    fprintf(fp, "@%s\n", label);
    fputs("D;JNE\n", fp);
    return 0;
}

int write_goto(char* label, FILE* fp) {
    fprintf(fp, "@%s\n", label);
    fputs("0;JMP\n", fp);
    return 0;
}

int write_function(char* label, char* n_locals, char* file_id, FILE* fp) {
    int n;
    if (write_label(label, fp) != 0) {
        return 1;
    }
    n = atoi(n_locals);
    for (int i=0; i<n; ++i) {
        if (write_pushpop(C_PUSH, S_CONSTANT, "0", file_id, fp) != 0) {
            return 2;
        }
    }
    return 0;
}

int write_call(char* f_name, char* n_args, size_t uid, char* file_id, FILE* fp) {
    char *ret_label;
    int i;
    
    // allocate arbitrary amount of space to store <f_name>.<uid>
    ret_label = (char *)malloc((strlen(f_name) + 10) * sizeof(char));

    // create return address label: <f_name>.<uid>
    strcpy(ret_label, f_name);
    i = strlen(f_name);
    ret_label[i++] = '.';
    sprintf(ret_label + i, "%zu", uid);

    // push return-address
    if (write_pushpop(C_PUSH, S_CONSTANT, ret_label, file_id, fp) != 0) {
        return 1;
    }

    // push LCL
    fputs("@LCL\n", fp);
    fputs("D=M\n", fp);
    fputs(PUSH, fp);

    // push ARG
    fputs("@ARG\n", fp);
    fputs("D=M\n", fp);
    fputs(PUSH, fp);

    // push THIS
    fputs("@THIS\n", fp);
    fputs("D=M\n", fp);
    fputs(PUSH, fp);

    // push THAT
    fputs("@THAT\n", fp);
    fputs("D=M\n", fp);
    fputs(PUSH, fp);

    // ARG = SP - n - 5
    fputs("@SP\n", fp);
    fputs("D=M\n", fp);
    fputs("@5\n", fp);
    fputs("D=D-A\n", fp);
    fprintf(fp, "@%s\n", n_args);
    fputs("D=D-A\n", fp);
    fputs("@ARG\n", fp);
    fputs("M=D\n", fp);

    // LCL = SP
    fputs("@SP\n", fp);
    fputs("D=M\n", fp);
    fputs("@LCL\n", fp);
    fputs("M=D\n", fp);

    // goto f
    if (write_goto(f_name, fp) != 0 ) {
        return 2;
    }

    // (return-address)
    if (write_label(ret_label, fp) != 0) {
        return 3;
    }

    free(ret_label);
    return 0;
}

int write_return(FILE *fp) {

    // FRAME = LCL
    fputs("@LCL\n", fp);
    fputs("D=M\n", fp);
    fputs("@FRAME\n", fp);
    fputs("M=D\n", fp);

    // RET = *(FRAME-5)
    fputs("@5\n", fp);
    fputs("A=D-A\n", fp); 
    fputs("D=M\n", fp);
    fputs("@RET\n", fp);
    fputs("M=D\n", fp);

    // *ARG = pop()
    fputs("@SP\n", fp);
    fputs("M=M-1\n", fp);
    fputs("A=M\n", fp);
    fputs("D=M\n", fp);
    fputs("@ARG\n", fp);
    fputs("A=M\n", fp);
    fputs("M=D\n", fp);
    fputs("@ARG\n", fp);
    fputs("D=M\n", fp);

    // SP = ARG+1
    fputs("@SP\n", fp);
    fputs("M=D+1\n", fp);

    // THAT = *(FRAME-1)
    fputs("@FRAME\n", fp);
    fputs("D=M\n", fp);
    fputs("@1\n", fp);
    fputs("A=D-A\n", fp);
    fputs("D=M\n", fp);
    fputs("@THAT\n", fp);
    fputs("M=D\n", fp);

    // THIS = *(FRAME-2)
    fputs("@FRAME\n", fp);
    fputs("D=M\n", fp);
    fputs("@2\n", fp);
    fputs("A=D-A\n", fp);
    fputs("D=M\n", fp);
    fputs("@THIS\n", fp);
    fputs("M=D\n", fp); 

    // ARG = *(FRAME-3)
    fputs("@FRAME\n", fp);
    fputs("D=M\n", fp);
    fputs("@3\n", fp);
    fputs("A=D-A\n", fp);
    fputs("D=M\n", fp);
    fputs("@ARG\n", fp);
    fputs("M=D\n", fp); 
    
    // LCL = *(FRAME-4)
    fputs("@FRAME\n", fp);
    fputs("D=M\n", fp);
    fputs("@4\n", fp);
    fputs("A=D-A\n", fp);
    fputs("D=M\n", fp);
    fputs("@LCL\n", fp);
    fputs("M=D\n", fp); 

    // goto RET
    fputs("@RET\n", fp);
    fputs("A=M\n", fp);
    fputs("0;JMP\n", fp);

    return 0;
}
