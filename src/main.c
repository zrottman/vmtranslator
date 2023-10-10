#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"
#include "parser.h"
#include "writer.h"

int main(int argc, char** argv) {

    char* ext;
    char* expected_ext;
    FILE* fp_out;

    int   nobootstrap = 0;
    int   option;


    // get options
    while ((option = getopt(argc, argv, "b")) != -1) {
        switch (option) {
            case 'b':
                // if -b flag given, do not write default bootstrap assembly
                nobootstrap = 1;
                break;
            default:
                printf("Usage: ./vmtranslator [-b] <destination asm file> <source vm files>\n");
                exit(1);
        }
    }

    // validate number of args
    if (argc - optind < 2) {
        printf("xUsage: ./vmtranslator [-b] <destination asm file> <source vm files>\n");
        exit(2);
    }

    // validate arg file extensions
    for (int i=optind; i<argc; ++i) {
        expected_ext = (i == optind) ? ".asm" : ".vm";
        ext = rfind(argv[i], '.');
        if (ext == NULL || mystrcmp(ext, expected_ext) != 0) {
            printf("Expected %s file, got: %s\n", expected_ext, argv[i]);
            exit(3);
        }
    }

    // open write file
    if (writer_init(argv[optind++], nobootstrap, &fp_out) != 0) {
        printf("Output file could not be created or opened for `%s`\n", argv[1]);
        exit(4);
    }

    // loop through argv files and translate each
    for (int i=optind; i<argc; ++i) {
        if (parser_translate(argv[i], fp_out) != 0) {
            // error
            break;
        }
    }

    // close code writer
    writer_close(fp_out);

    return 0;
}
