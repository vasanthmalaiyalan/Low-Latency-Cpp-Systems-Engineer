#include<stdio.h>
#include<limits.h>

// Buggy version
int tadd_ok_bug(int x, int y) {
    int sum = x + y;
    return (sum - x == y) && (sum - y == x);
}

// correct version
int tadd_ok_correct(int x, int y) {
    int sum = x + y;

    if (x > 0 && y > 0 && sum <= 0)
        return 0;

    if (x < 0 && y < 0 && sum >= 0)
        return 0;
        
    return 1;    
}

int main() {

    int test[][2] = {
        {10, 20},         // normal
        {INT_MAX, 1},     // Positive overflow
        {INT_MIN, -1},    // Negative overflow
        {100, -50},       // normal
    };

    int n = sizeof(test) / sizeof(test[0]);

    printf("======= Compare Buggy vs Correct ======\n\n");

    for (int i = 0; i < n; i++) {

        int x = test[i][0];
        int y = test[i][1];
        int sum = x + y;

        printf("x = %d, y = %d\n", x, y);
        printf("x + y = %d\n", sum);

        int bug = tadd_ok_bug(x, y);
        int correct = tadd_ok_correct(x, y);

        printf("Buggy result = %d\n", bug);
        printf("Correct result = %d\n", correct);

        printf("-----------------------------------\n");
    }

    return 0;
}

/*
“2³¹ = 2147483648 தானே? அப்போ ஏன் INT_MAX = 2147483647?”

🔹 1. முதலில் 32-bit number

👉 32 bits → total values:

2^32 = 4294967296 values
🔹 2. Unsigned range

👉 unsignedல:

0 → 4294967295 (2^32 - 1)

👉 இங்க:

2147483648 (2^31) → valid ✅
🔥 3. Signed (Two’s Complement) range

👉 signedல:

-2^31  to  2^31 - 1

👉 so:

-2147483648 to 2147483647
🔥 முக்கிய point

👉 total values same:

4294967296

👉 but split:

Type	Range
negative	-2147483648 → -1
positive	0 → 2147483647
🔴 ஏன் +2147483648 இல்லை?

👉 because:

one bit = sign bit ❗

👉 MSB (most significant bit):

0 → positive
1 → negative
🔹 4. Binary explanation
👉 INT_MAX
01111111 11111111 11111111 11111111
= 2147483647
👉 INT_MIN
10000000 00000000 00000000 00000000
= -2147483648
🔥 Important insight

👉

+2147483648 represent செய்ய முடியாது ❌

👉 அந்த bit pattern already use பண்ணுது:

-2147483648 க்கு
🔹 5. So your example
{INT_MAX, 1}

👉 means:

2147483647 + 1

👉 result:

2147483648 ❌ (cannot represent)

👉 so:

wrap → -2147483648 😳
🔥 6. Why 2147483647?

👉 because:

max positive = 2^31 - 1

👉 not:

2^31 ❌
🔥 7. Simple analogy

👉 imagine 32 seats:

1 seat → negative sign
remaining → values

👉 so positive side:

one less number 😄
🔥 8. One-line takeaway

👉
“signed int → range = -2^31 to 2^31 - 1 (not symmetric)”

🔥 Final clarity

👉

2³¹ = 2147483648 ✔
but signedல represent முடியாது ❌
அதனால் max = 2147483647

💥 நீ இந்த point புரிஞ்சா:
👉 signed vs unsigned முழுக்க clear
*/