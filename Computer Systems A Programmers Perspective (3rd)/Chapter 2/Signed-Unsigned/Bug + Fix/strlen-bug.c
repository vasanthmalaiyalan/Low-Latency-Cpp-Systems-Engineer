#include<stdio.h>
#include<string.h>

// Buggy version
int strlonger_bug(const char *s, const char *t) {
    // subtraction on unsigned (size_t) -> dangerous
    return strlen(s) - strlen(t) > 0;// because strlen return only unsigned value right
}
// corrected version
int strlonger_fix(const char *s, const char *t) {
    //direct comparison -> safe
    return strlen(s) > strlen(t); // c -> comparision result always 1 or 0
}

// Debug version (to show what's happening)
int strlonger_debug(const char *s, const char *t) {
    size_t ls = strlen(s);
    size_t lt = strlen(t);

    size_t diff = ls - lt; // unsigned subtraction
    
    printf("ls - lt (unsigned) = %zu\n", diff);// that z means size modeifier

    return diff > 0;
}

int main() {
    // test cases
    const char *s1 = "Hi";
    const char *t1 = "Hello";

    const char *s2 = "Welcome";
    const char *t2 = "Hi";

    printf("=======Case 1: s short than t ========\n");
    printf("Buggy = %d\n", strlonger_bug(s1, t1));
    printf("Fixed = %d\n",  strlonger_fix(s1, t1));

    printf("\n---Debug ----\n");
    strlonger_debug(s1, t1);

    printf("\n=========== Case 2: s longer than t ===========\n");
    printf("Buggy = %d\n", strlonger_bug(s2, t2));
    printf("Fixed = %d\n", strlonger_fix(s2, t2));

    return 0;
}

