#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "parser.h"
#include "writer.h"

int main(int argc, char** argv) {

    char* ext;
    char* expected_ext;
    FILE *fp_out;

    // validate number of args
    if (argc < 3) {
        printf("Usage: ./vmtranslator <destination asm file> <source vm files>\n");
        exit(1);
    }
    
    // validate arg file extensions
    for (int i=1; i<argc; ++i) {
        expected_ext = (i == 1) ? ".asm" : ".vm";
        ext = rfind(argv[i], '.');
        if (ext == NULL || mystrcmp(ext, expected_ext) != 0) {
            printf("Expected %s file, got: %s\n", expected_ext, argv[i]);
            exit(2);
        }
    }

    // open write file
    if (writer_init(argv[1], &fp_out) != 0) {
        printf("Output file could not be created or opened for `%s`\n", argv[1]);
        exit(3);
    }

    // loop through argv files and translate each
    for (int i=2; i<argc; ++i) {
        if (parser_translate(argv[i], fp_out) != 0) {
            // error
            break;
        }
    }

    // close code writer
    writer_close(fp_out);

    return 0;
}
