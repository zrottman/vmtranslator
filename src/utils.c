// Function:    rfind
// Description: Home-rolled version of strrchr(). Searches for last
//              occurrence of `target` in string `s` and returns a
//              pointer to its location.
// Arguments:   s      : input string
//              target : target char
// Returns:     Pointer to last location in `s` of `target`
char* rfind(char* s, char target) {
    char *p = 0;
    for (int i=0; s[i] != '\0'; ++i) {
        if (s[i] == target) { p = s+i; }
    }
    return p;
}

// Function:    mystrcmp
// Description: Home-rolled version of strcmp(). Compares two input strings,
//              returnin 0 if they are identical else 1.
// Arguments:   s1 : string one
//              s2 : string two
// Returns:     0 if they are identical, else 1.
int mystrcmp(const char* s1, const char* s2) {
    for (int i=0; s1[i] != '\0' || s2[i] != '\0'; ++i) {
        if (s1[i] != s2[i]) { return 1; }
    }
    return 0;
}
