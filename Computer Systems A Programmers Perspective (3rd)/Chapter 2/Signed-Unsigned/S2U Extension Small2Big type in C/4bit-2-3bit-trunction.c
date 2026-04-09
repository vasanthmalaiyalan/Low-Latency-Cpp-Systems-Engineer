#include<stdio.h>

// 4-bit binary print
void print4(int x) {
    for (int i =3; i >=0; i--) {
        printf("%d", (x >> i) & 1);
    }
}

// 3- bits binary print
void print3(int x) {
    for (int i = 2; i >= 0; i--) {
        printf("%d", (x >> i) & 1);
    }
}
// 4-bits signed value (two's complement)
int signed4(int x) {
    if (x & 0x8)  // MSB = 1
        return x - 16; // 2^4 = 16
      return x;  
}

// 3-bits signed value
int signed3(int x) {
    if (x & 0x4)  //MSB = 1
       return x - 8;  // 2^3 = 8
    return x;   
}

int main() {
    printf("Hex | 4-bit | 3-bit | Uns(orig) Uns(trunc) | Sig(orig) | Sig(trunc)\n");
    printf("-----------------------------------------------------------------\n");

    for (int x = 0; x < 16; x++) {

        int trunc = x & 0x7; // truncate to 3 bits (mod 8)

        printf("%3X | ", x);

        print4(x);
        printf(" | ");
        print3(trunc);
        printf(" | ");

        // unsigned
        printf("%10d | %11d | ", x, trunc);

        // signed
        printf("%9d | %10d\n", signed4(x), signed3(trunc));
    }
    return 0;
}