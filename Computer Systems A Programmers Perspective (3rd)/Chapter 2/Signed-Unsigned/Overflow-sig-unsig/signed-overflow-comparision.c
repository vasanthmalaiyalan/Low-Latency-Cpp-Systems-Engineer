#include<stdio.h>
#include<limits.h>

int main() {

    // Example 1: Positive + Positive -> Negative
    int a = INT_MAX; // max int (2147483647)
    int b = 1;

    int sum = a + b;

    printf("======== Example 1 =========\n");
    printf("a = %d\n", a);
    printf("b = %d\n", b);
    printf("a + b = %d\n", sum); // overflow

    // Example 2: x < y vs x - y < 0
    int x = 2000000000;
    int y = -2000000000;

    printf("\n===== Example 2 =========\n");
    printf("x = %d, y = %d\n", x, y);

    // Direct comparision (correct)
    printf("x < y = %d\n", x < y);

    // Subtraction comparison (dangerous)
    printf("(x - y) = %d\n", x - y);
    printf("(x - y) < 0 = %d\n", (x - y) < 0);

    return 0;
}

/*
🔴 Given
int x = 2000000000;
int y = -2000000000;

int z = x - y;

👉 mathematically:

x - y = 2000000000 - (-2000000000)
      = 4000000000
🔥 Problem

👉 int (32-bit) range:

-2147483648 to 2147483647

👉 ஆனால்:

4000000000 ❌ (out of range)
🔥 So என்ன ஆகும்?

👉 Computer:

mod 2^32 arithmetic

👉 அதாவது:

4000000000 mod 2^32
= 4000000000 (fits in 32-bit unsigned)

👉 binary representation:

4000000000 = 0xEE6B2800
🔥 Step 1: Binary view
0xEE6B2800 =

1110 1110 0110 1011 0010 1000 0000 0000
↑
MSB = 1 → negative (signed)
🔥 Step 2: Signed interpretation

👉 Two’s complement rule:

If MSB = 1 → negative number

👉 value கண்டுபிடிக்க:

value = unsigned_value - 2^32
🔥 Step 3: Apply formula
= 4000000000 - 4294967296
= -294967296 😳
🔥 Final result
(x - y) = -294967296
🔹 Simple shortcut 💡

👉 whenever overflow:

result = actual_value - 2^32
🔥 Visual intuition
Actual result:     4000000000
Max signed int:    2147483647

Overflow wrap:

4000000000
-4294967296
------------
-294967296
🔥 Why negative?

👉 MSB = 1 → sign bit set

1xxxxxxx → negative number
🔥 Key Insight

👉
same bits → different interpretation

Type	Value
unsigned	4000000000
signed	-294967296
🔥 One-line takeaway

👉
Overflow → bits same → signed interpretation → negative value 😳

🔥 Super simple analogy

👉 imagine:

number circle (mod 2^32)

👉 you cross max → wrap → negative side
*/