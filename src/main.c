#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "parser.h"

int main(int argc, char** argv) {

    char* ext;

    // validate number args
    if (argc < 2) {
        printf("Usage: ./vmtranslator <source vm files>\n");
        exit(1);
    }
    
    // validate arg file extensions
    for (int i=1; i<argc; ++i) {
        ext = rfind(argv[i], '.');
        if (ext == NULL || mystrcmp(ext, ".vm") != 0) {
            printf("Files must be .vm files, you supplied: %s\n", argv[i]);
            exit(2);
        }
    }

    // loop through argv files and translate each
    for (int i=1; i<argc; ++i) {
        if (parser_translate(argv[i]) != 0) {
            break;
        }
    }

    return 0;
}
