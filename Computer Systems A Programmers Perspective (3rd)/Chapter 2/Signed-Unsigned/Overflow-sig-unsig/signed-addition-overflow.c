#include<stdio.h>
#include<limits.h>

// Check signed addition overflow
int tadd_ok(int x, int y) {
    int sum = x + y;

    // positive overflow
    if (x > 0 && y > 0 && sum <= 0)
    return 0;

    // negative overflow
    if (x < 0 && y < 0 && sum >= 0)
    return 0;

    return 1; // safe
}

int main() {

    int test[][2] = {
        {10, 20},          //normal
        {INT_MAX, 1},      // positive overflow
        {INT_MIN, -1},     // negative overflow
        {1000, -500},      // normal
        {-2000, -3000},    // normal negative
    };

    int n = sizeof(test) / sizeof(test[0]);

    printf("========== signed Addition Overflow Check ==========\n");

    for (int i = 0; i < n; i++) {
        int x = test[i][0];
        int y = test[i][1];
        int sum = x + y;

        printf("x = %d, y = %d\n", x, y);
        printf("x + y = %d\n", sum);

        if (tadd_ok(x,y))
             printf("Result: Safe (No Overflow)\n");
        else 
           printf("Result: Overflow \n");
           
        printf("----------------------------------\n");
    }
    return 0;
}