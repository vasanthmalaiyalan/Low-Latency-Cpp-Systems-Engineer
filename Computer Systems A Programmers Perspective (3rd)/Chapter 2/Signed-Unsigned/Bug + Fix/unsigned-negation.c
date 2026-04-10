#include<stdio.h>

//4-bit unsigned negation
unsigned u_neg_4bits(unsigned x) {
    return (x ==0) ? 0 : (16 - x); // 2^4 = 16
}

int main() {
    
    unsigned values[] = {1, 4, 7, 10, 14}; // 10=A, 14=E

    for (int i = 0; i < 5; i++) {
        unsigned x = values[i];
        unsigned nx = u_neg_4bits(x);

        printf("x = %X (%u), -u x = %X (%u), sum = %u\n",x, x, nx, nx, (x + nx) % 16);
    }

    return 0;
}