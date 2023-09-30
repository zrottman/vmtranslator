#include "parser.h"

int parser_translate(const char* vmfile) {

    // open file
    FILE *fp;
    char line[128];
    char* token;
    //enum CommandType command_type;

    fp = fopen(vmfile, "r");
    if (fp == NULL) {
        printf("File `%s` could not be opened or could not be found.\n", vmfile);
        return 1; 
    }
    
    int i=1;
    while (fgets(line, sizeof(line), fp) != NULL) {

        // strip comments and trim leading/trailing space
        cleanline(line);

        // skip blank lines
        if (*line == '\0') { continue; }

        // print vm line
        //printf("%2d: %s\n", i++, line);

        printf("%2d: ", i++);
        token = strtok(line, " ");
        //command_type = get_command_type(token);
        while (token != NULL) {
            printf("%s/", token);
            token = strtok(NULL, " ");
        }
        printf("\n");
        
    }
     
    // create code writer
    

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

