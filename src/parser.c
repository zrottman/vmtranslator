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
        cleanline(line);
        if (*line != '\0')
            printf("%2d: %s\n", i++, line);
    }
     
    // create code writer
    
    // read each line

    // get command type
    
    // close code writer
    // close file
    fclose(fp);

    return 0;
}

void cleanline(char* line) {

    // strip comments
    char *p = strstr(line, "//");
    if (p != NULL) {
        *p = '\0';
    }

    // trim
    trim(line);
}

void trim(char* line) {
    int i, j;
    i = j = 0;

    // move `i` past leading whitespace
    while (line[i] == ' ' || line[i] == '\t' || line[i] == '\n' || line[i] == '\r') {
        i++;
    }
    // copy `i` to `j` until the end of the string
    while (line[i] != '\0') {
        line[j++] = line[i++];
    }

    // backtrack past trailing whitespace and remove
    j--;
    while (line[j] == '\n' || line[j] == '\t' || line[j] == ' ' || line[j] == '\r') {
        j--;
    }

    line[++j] = '\0';
}


