#include "utils.h"

// Function:    rfind
// Description: Home-rolled version of strrchr(). Searches for last
//              occurrence of `target` in string `s` and returns a
//              pointer to its location.
// Arguments:   s      : input string
//              target : target char
// Returns:     Pointer to last location in `s` of `target`
char* rfind(const char* s, char target) {
    if (s == NULL) {
        return NULL;
    }

    char *last_occurrence = NULL;

    while (*s != '\0') {
        if (*s == target) { last_occurrence = (char *)s; }
        s++;
    }

    return last_occurrence;
}

// Function:    mystrcmp
// Description: Home-rolled version of strcmp(). Compares two input strings,
//              returnin 0 if they are identical else 1.
// Arguments:   s1 : string one
//              s2 : string two
// Returns:     0 if they are identical, else 1.
int mystrcmp(const char* s1, const char* s2) {
    // handle NULL pointers
    if (s1 == 0 && s2 == 0) {
        return 0; 
    } else if (s1 == 0) {
        return -1;
    } else if (s2 == 0) {
        return 1;
    }

    while (*s1 != '\0' || *s2 != '\0') {
        if (*s1 != *s2) {
            return (*s1 > *s2) ? 1 : -1;
        }
        s1++;
        s2++;
    }

    return 0;
}

// Function:    get_fileid
// Description: Extracts filename (excluding extension) from path. Function
//              dynamically allocates memory for output string; caller responsible
//              for freeing returned char*.
// Arguments:   filepath : filename, e.g. path/to/myfile.ext
// Returns:     Pointer to dynamically-allocated fileid string 
char* get_fileid(const char* filepath) {
    if (filepath == NULL) {
        return NULL;
    }

    char* last_slash = rfind(filepath, '/');

    if (last_slash) {
        last_slash++;
    } else {
        last_slash = (char *)filepath;
    }

    char *last_dot = rfind(last_slash, '.');

    size_t len = last_dot ? (size_t)(last_dot - last_slash) : strlen(last_slash);

    char* result = (char *)malloc(len + 1);
    strncpy(result, last_slash, len);
    result[len] = '\0';

    return result;
}
