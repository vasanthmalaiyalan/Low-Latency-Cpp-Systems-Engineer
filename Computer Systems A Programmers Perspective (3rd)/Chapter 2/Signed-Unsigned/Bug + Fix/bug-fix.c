#include<stdio.h>

// Buggy version
float sum_elements_bug(float a[], unsigned length) {
    int i;
    float result = 0;

    printf("\n [BUGGY LOOP START]\n");

    for (i = 0; i <= length -1; i++) {
        printf("i = %d\n", i); //debug
        result += a[i]; // invaild access when length=0
        if (i > 5) break;  // stop early to avoid crash
    }
    return result;
}
    // correct version
    float sum_elements_fixed(float a[], unsigned length) {
        int i;
        float result = 0;

        printf("\n[FIXED LOOP START]\n");

        for (i = 0; i < length; i++) {
            printf("i = %d\n", i);
            result += a[i];
        }
        return result;
    }
    int main() {

        float arr[] = {1.0, 2.0, 3.0};

        unsigned length = 0; // test case

        printf("Length = %u\n", length);

        // buggy
        float r1 = sum_elements_bug(arr, length);
        printf("Buggy result = %f\n", r1);

        // fixed
        float r2 = sum_elements_fixed(arr, length);
        printf("Fixed result = %f\n", r2);

        return 0;
}