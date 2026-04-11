#include<stdio.h>
#include<limits.h>

// check unsigned addition overflow
int uadd_ok(unsigned x, unsigned y) {
    unsigned s = x + y;
    return s >= x; // if false -> overflow
}

// unsigned negation (-u x)
unsigned u_neg(unsigned x) {
    return (x == 0) ? 0 : (0 -x); //same as 2^w - x
}

int main() {

    // Test cases for overflow
    unsigned a = 10, b = 20;
    unsigned c = UINT_MAX, d = 1;

    printf("====== Unsigned Addition Check =======\n");
     printf("%u + %u -> %u | overflow? %s\n", a, b, a + b, uadd_ok(a, b) ? "NO" : "YES");

     printf("%u + %u -> %u | overflow? %s\n", c, d, c + d, uadd_ok(c, d) ? "NO" : "YES");

     // Test unsigned negation
     printf("\n =============== Unsigned Negation ===============\n");

     unsigned x = 5;
     unsigned nx = u_neg(x);

     printf("x = %u\n", x);
     printf("'-u x = %u\n", nx);
     printf("x + (-u x) = %u\n", x + nx);

     // More example
     printf("\n More Negation Example:\n");

     for (unsigned i = 0; i <= 5; i++) {
        unsigned neg = u_neg(i);
        printf("x = %u, -u x = %u, sum = %u\n", i, neg, i + neg);
     }

     return 0;
}