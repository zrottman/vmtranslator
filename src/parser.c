#include "parser.h"

int parser_translate(const char* vmfile) {

    // open file
    FILE *fp;
    char line[128];

    fp = fopen(vmfile, "r");
    if (fp == NULL) {
        printf("File `%s` could not be opened or could not be found.\n", vmfile);
        return 1; 
    }
    
    int i=1;
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%2d: %s", i++, line);
    }
     
    // create code writer
    
    // read each line

    // get command type
    
    // close code writer
    // close file
    fclose(fp);

    return 0;
}
