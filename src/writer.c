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
