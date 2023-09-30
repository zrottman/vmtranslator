#include <stdio.h>
#include <stdlib.h>

char* rfind(char* s, char target) {
    char c;
    char *p = NULL;
    for (int i=0; s[i] != '\0'; ++i) {
        if (s[i] == target) {
            p = s+i;
        }
    }
    return p;
}

int mystrcmp(const char* s1, const char* s2) {
    for (int i=0; s1[i] != '\0' || s2[i] != '\0'; ++i) {
        if (s1[i] != s2[i]) { return 1; }
    }
    return 0;
}

int main(int argc, char** argv) {

    char* ext;

    // validate argc
    if (argc < 2) {
        printf("Usage: ./vmtranslator <source vm files>\n");
        exit(1);
    }
    
    // validate file extensions
    for (int i=1; i<argc; ++i) {
        ext = rfind(argv[i], '.');
        if (mystrcmp(ext, ".vm") != 0) {
            printf("Files must be .vm files, you supplied: %s\n", argv[i]);
            exit(2);
        }
    }


    // loop through argv files and translate each one
    for (int i=1; i<argc; ++i) {
        // translate argv[i]
    }



    return 0;
}
