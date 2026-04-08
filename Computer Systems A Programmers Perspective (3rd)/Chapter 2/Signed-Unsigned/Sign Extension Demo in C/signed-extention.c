#include<stdio.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len) {
    for (size_t i = 0; i < len; i++)
         printf("%02x ", start[i]);
    printf("\n");     
}

int main() {

    short sx = -12345; // 16-bit signed
    unsigned uy = sx; // tricky conversation

    printf("sx (short) = %d\n", sx);
    printf("uy (unsigned) = %u\n\n", uy); // small type → first promoted to int , then → converted to target type

    printf("Memory view:\n");

    printf("sx: ");
    show_bytes((byte_pointer)&sx, sizeof(short));

    printf("uy:  ");
    show_bytes((byte_pointer)&uy, sizeof(unsigned));

    return 0;
}

/*
🔥 Step-by-step Explanation
✅ Step 1: Initial value
short sx = -12345;

👉 16-bit representation:

CFC7
(binary: 1100 1111 1100 0111)
✅ Step 2: Assignment
unsigned uy = sx;

👉 இது simple cast இல்ல ❗
👉 internally இது இப்படிதான் நடக்கும்:

(unsigned) sx
= (unsigned) (int) sx
🔥 Step 3: First → size increase (sign extension)
int temp = sx;

👉 16-bit → 32-bit

CFC7  →  FFFFCFC7

👉 why FFFF?

➡️ MSB = 1 → negative
➡️ so fill with 1 (sign extension)

🔥 Step 4: Then → type change
unsigned uy = temp;

👉 bits same:

FFFFCFC7

👉 but now interpreted as unsigned:

= 4294954951 😳
🔹 Visual Flow
sx (short, 16-bit)
CFC7  → -12345

↓ sign extension

int (32-bit)
FFFFCFC7 → -12345

↓ reinterpret as unsigned

unsigned (32-bit)
FFFFCFC7 → 4294954951
🔹 Compare with expected (but wrong assumption)

நீ நினைப்பது:

(unsigned short)sx → 53191

👉 yes correct:

CFC7 → 53191

👉 ஆனால் actual code இது இல்ல ❗

🔥 Key Insight

👉 C language rule:

small type → first promoted to int
then → converted to target type
🔹 Bonus: Verify manually
printf("%u\n", (unsigned)(int)-12345);

👉 same result:

4294954951
🔥 One-line takeaway

👉
short → int (sign extension) → unsigned (reinterpret bits)

➡️ அதனால் பெரிய positive number கிடைக்கும்
*/