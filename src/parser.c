#include "parser.h"

int parser_translate(const char* vmfile) {

    FILE* fp;
    char  line[128];

    // open file
    fp = fopen(vmfile, "r");
    if (fp == NULL) {
        printf("File `%s` could not be opened or could not be found.\n", vmfile);
        return 1; 
    }
    
    // writer_init(vmfile)
    
    int i=1; // for line number printing -- to delete
    while (fgets(line, sizeof(line), fp) != NULL) {


        // strip comments and trim leading/trailing space
        cleanline(line);

        // skip blank lines
        if (*line == '\0') { continue; }

        // print vm line
        // write_comment(line);
        printf("%2d: %s\n", i++, line);

        // parse line
        parse_line(line);
    }
     
    // close code writer
    // writer_close()
    
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
    // trim leading space: copy `i` to `j` until the end of the string
    while (line[i] != '\0') {
        line[j++] = line[i++];
    }

    // trim trailing space: backtrack past trailing whitespace and remove
    j--;
    while (line[j] == '\n' || line[j] == '\t' || line[j] == ' ' || line[j] == '\r') {
        j--;
    }

    line[++j] = '\0';
}

void parse_line(char *line) {

    static size_t line_num = 1;
    size_t        token_count;
    char*         token;
    char*         tokens[3];

    // parse three tokens
    token_count = 0;
    token = strtok(line, " ");
    while (token != NULL) {
        tokens[token_count++] = token;
        token = strtok(NULL, " ");
    }

    switch (get_command_type(tokens[0])) {
        case C_PUSH:
            printf("%2zu: push\n", line_num);
            // write_pushpop(C_PUSH, tokens[1], tokens[2]);
            break;
        case C_POP:
            printf("%2zu: pop\n", line_num);
            // write_pushpop(C_POP, tokens[1], tokens[2]);
            break;
        case C_ARITHMETIC:
            printf("%2zu: arithmetic\n", line_num);
            // write_arithmetic(tokens[0]);
            break;
        case C_LABEL:
            printf("%2zu: label\n", line_num);
            // write_label(tokens[1]);
            break;
        case C_IF:
            printf("%2zu: if\n", line_num);
            // write_if(tokens[1]);
            break;
        case C_GOTO:
            printf("%2zu: goto\n", line_num);
            // write_goto(tokens[1]);
            break;
        case C_FUNCTION:
            printf("%2zu: function\n", line_num);
            // write_function(tokens[1], tokens[2]);
            break;
        case C_CALL:
            printf("%2zu: call\n", line_num);
            // write_call(tokens[1], tokens[2]);
            break;
        case C_RETURN:
            printf("%2zu: return\n", line_num);
            // write_return();
            break;
        case C_UNKNOWN:
            printf("%2zu: unknown\n", line_num);
            break;
    }

    line_num++;
}
