#include "parser.h"

int parser_translate(const char* vmfile, FILE* fp_out) {

    FILE* fp_in;
    char  line[128];

    // open file
    fp_in = fopen(vmfile, "r");
    if (fp_in == NULL) {
        printf("Input file `%s` could not be opened or could not be found.\n", vmfile);
        return 1; 
    }
    
    int i=1; // for line number printing -- to delete
    while (fgets(line, sizeof(line), fp_in) != NULL) {


        // strip comments and trim leading/trailing space
        cleanline(line);

        // skip blank lines
        if (*line == '\0') { continue; }

        // write vm line as comment
        write_comment(line, fp_out);
        //printf("%2d: %s\n", i++, line);

        // parse line
        if (parse_line(line, fp_out) != 0) {
            printf("Error parsing line `%s` from file `%s`\n", line, vmfile);
            fclose(fp_in);
            return 1;
        }
    }
     
    // close file
    fclose(fp_in);

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

int parse_line(char *line, FILE* fp_out) {

    static size_t line_num = 1;
    size_t        token_count;
    char*         token;
    char*         tokens[3];
    static size_t uid = 0; // unique ID for labels/jumps, ideally would reset to 0 for each .vm file

    // parse three tokens
    token_count = 0;
    token = strtok(line, " ");
    while (token != NULL) {
        tokens[token_count++] = token;
        token = strtok(NULL, " ");
    }

    switch (lookup_vm_command(tokens[0])) {
        case C_PUSH:
            if (write_pushpop(C_PUSH, lookup_vm_segment(tokens[1]), tokens[2], fp_out) != 0) {
                // error;
                return 1;
            }
            break;
        case C_POP:
            if (write_pushpop(C_POP, lookup_vm_segment(tokens[1]), tokens[2], fp_out) != 0) {
                // error;
                return 2;
            }
            break;
        case C_ADD:
        case C_SUB:
        case C_NEG:
        case C_EQ:
        case C_GT:
        case C_LT:
        case C_AND:
        case C_OR:
        case C_NOT:
            if (write_arithmetic(lookup_vm_command(tokens[0]), uid++, fp_out) != 0) {
                // error;
                return 3;
            }
            break;
        case C_LABEL:
            if (write_label(tokens[1], fp_out) != 0) {
                //error;
                return 4;
            }
            break;
        case C_IF:
            if (write_if(tokens[1], fp_out) != 0) {
                //error
                return 5;
            }
            break;
        case C_GOTO:
            if (write_goto(tokens[1], fp_out) != 0) {
                // error
                return 6;
            }
            break;
        case C_FUNCTION:
            if (write_function(tokens[1], tokens[2], fp_out) != 0) {
                // error
                return 7;
            }
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

    return 0;
}
