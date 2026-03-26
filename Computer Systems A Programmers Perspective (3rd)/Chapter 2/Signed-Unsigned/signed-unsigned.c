#include<stdio.h>

// B2U function (Eq 2.1)
unsigned int B2U(unsigned char x, int w) {
    unsigned int result = 0;

    for (int i = 0; i < w; i++) {
        int xi = (x >> i) & 1;
        result += xi * (1 << i);
    }
    return result;
}
// B2T (Eq 2,3)
int B2T(unsigned char x, int w) {
    int result = 0;
    // MSB (negative weight) 
    for (int i = 0; i < w; i++) {
        int xi = (x >> i) & 1;

        if (i == w-1)
        result -= xi * (1 << i); // MSB negative
        else
        result += xi * (1 << i); //rest positive
    }
    return result;
}

    //print binary
    void print_binary(unsigned char x, int w) {
        printf("[");
        for (int i = w-1; i >= 0; i--) {
            printf("%d",(x >> i) & 1);
        }
        printf("]");
    }
    // Show table row
    void show(unsigned char x, int w) {
        printf("0x%X  ", x);
        print_binary(x, w);
        printf(" B2U = %-2u B2T = %d\n",B2U(x, w), B2T(x, w));
    }
int main() {
    int w = 4;

    printf("==== CSAPP Practice 2.17 ====\n\n");
    printf("Hex Binary Unsigned  Signed\n");
    printf("------------------------------\n");

    show(0xA, 4); // 1010
    show(0x1, 4); // 0001
    show(0xB, 4); // 1011
    show(0x2, 4); //0010
    show(0x7, 4); //0111
    show(0xc, 4); //1100

    return 0;
}