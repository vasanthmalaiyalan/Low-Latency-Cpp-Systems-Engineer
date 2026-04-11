 #include<stdio.h>
 #include<limits.h>

 int main() {

    printf("======= Example 1: Positive Overflow =========\n");

    int x = INT_MAX; //2147483647
    int y = 1;

    int sum = x + y;

    printf("x = %d\n", x);
    printf("y = %d\n", y);
    printf("x + y = %d\n", sum);

    if (x > 0 && y > 0 && sum <= 0)
       printf("Positive Overflow detected!\n");


    printf("\n ======== Example 2: Negative Overflow -2147483649 mod 2^32 ==========\n");
    
    int a = INT_MIN; // -2147483648
    int b = -1;

    int sum2 = a + b;

    printf("a = %d\n", a);
    printf("b = %d\n", b);
    printf("a + b = %d\n", sum2);

    if (a < 0 && b < 0 && sum2 >= 0)
        printf("Negative Overflow detected!\n");

    printf("\n ========= Ecxample 3: Normal ase =========\n");
    
    int p = 10, q = 20;
    int sum3 = p + q;

    printf("p = %d, q = %d\n", p, q);
    printf("p + q = %d (corrected)\n", sum3);

    return 0;
 }