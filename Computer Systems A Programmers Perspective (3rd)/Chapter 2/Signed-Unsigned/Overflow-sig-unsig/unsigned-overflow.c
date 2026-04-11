#include<stdio.h>

int main() {
    unsigned int x = 9;
    unsigned int y = 12;

    unsigned int s = x + y;

    printf("x = %u, y = %u\n", x, y);
    printf("x + y = %u\n", s);

    if (s < x) {
        printf("Overflow occurred\n");
     else
        printf("No overflow");   
    }
    return 0;
}

/*
🔹 1. Basic Idea

👉 நாம் இரண்டு unsigned numbers add பண்ணுறோம்:

0 ≤ x, y < 2^w

👉 result:

0 ≤ x + y < 2^(w+1)

👉 😳 அதாவது:
➡️ result represent பண்ண w+1 bits தேவைப்படலாம்

🔥 Problem

👉 ஆனால் computer:

fixed size (w bits மட்டும்)

👉 so:

extra bit → discard (cut)
🔹 2. Core Concept (Unsigned Addition)

👉 define பண்ணுறோம்:

x +u y = (x + y) mod 2^w

👉 simple meaning:

overflow வந்தா → wrap-around
🔥 3. இரண்டு case
✅ Case 1: No overflow
x + y < 2^w

👉 result:

x +u y = x + y

✔ normal addition

❌ Case 2: Overflow
x + y ≥ 2^w

👉 result:

x +u y = x + y - 2^w

👉 😳 wrap-around

🔥 4. Example (4-bit system)

👉 w = 4 → max = 15
👉 2^w = 16

🔹 Example:
x = 9  → 1001
y = 12 → 1100

👉 normal sum:

9 + 12 = 21

👉 binary:

1001
+1100
------
10101   (5 bits 😳)
🔥 truncate (cut MSB)
0101 → 5

👉 so:

9 +u 12 = 5
🔥 Check with formula
21 mod 16 = 5 ✅
🔹 5. Why overflow happens?

👉 because:

result > max representable value

👉 so:

extra bit lost
🔥 6. Visual intuition

👉 imagine circular number line:

0 → 1 → 2 → ... → 15 → 0 → 1 → ...

👉 so:

15 + 1 = 0

👉 wrap-around 🔄

🔹 7. Overflow detection 🔥

👉 rule:

if (s < x) → overflow

👉 where:

s = x +u y
🔥 Example:
x = 9
y = 12
s = 5

👉 check:

5 < 9 → TRUE → overflow ✅
🔥 Why this works?

👉 normally:

x + y ≥ x

👉 but overflow:

x + y - 2^w < x

👉 so result smaller ஆகிடும்

🔹 8. C Code Example
#include <stdio.h>

int main() {
    unsigned int x = 9;
    unsigned int y = 12;

    unsigned int s = x + y;

    printf("x = %u, y = %u\n", x, y);
    printf("x + y = %u\n", s);

    if (s < x)
        printf("Overflow occurred!\n");
    else
        printf("No overflow\n");

    return 0;
}
🔥 Output
x = 9, y = 12
x + y = 21   (if 32-bit, no overflow)

But in 4-bit logic:
x + y = 5
Overflow occurred!
🔹 9. Important Notes

👉 Cல:

unsigned overflow → defined behavior ✅

👉 signed overflow:

undefined behavior ❌
🔥 10. Key Takeaways

👉
✔ unsigned addition = mod arithmetic
✔ overflow = wrap-around
✔ result smaller ஆகும்
✔ s < x → overflow detect

🔥 One-line summary

👉
“Unsigned addition = (x + y) mod 2^w → overflowனா wrap-around”
*/