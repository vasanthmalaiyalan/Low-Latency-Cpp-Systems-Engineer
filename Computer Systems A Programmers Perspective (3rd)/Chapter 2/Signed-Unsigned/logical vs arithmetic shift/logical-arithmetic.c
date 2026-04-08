#include<stdio.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02x ", start[i]);
    }
    printf("\n");
}

// fun1: unsigned extraction
int fun1(unsigned word) {
    return (int)((word << 24) >> 24);
}

//fun2: signed extraction
int fun2(unsigned word) {
    return ((int)word << 24) >> 24;// this si signed you know
}

int main() {

    unsigned test_values[] = {
        0x00000076,
        0x87654321,
        0x000000c9,
        0xEDCBA987
    };

    int n = sizeof(test_values) / sizeof(test_values[0]);

    for (int i = 0; i < n; i++) {

        unsigned w = test_values[i];
        printf("==============================\n");
        printf("Input w = 0x%08X\n", w);

        int r1 = fun1(w);
        int r2 = fun2(w);

        printf("fun1(w) = 0x%08X (%d)\n", r1, r1);
        printf("fun2(w) = 0x%08X (%d)\n", r2, r2);

        printf("Memory of w:   ");
        show_bytes((byte_pointer)&w, sizeof(unsigned));

        printf("Memory fun1   ");
        show_bytes((byte_pointer)&r1, sizeof(int));

        printf("Memory fun2:  ");
        show_bytes((byte_pointer)&r2, sizeof(int));
    }

    return 0;
}

/*
Visual Memory Representation

For 0x000000C9:
fun1 (Zero Extension)
text

Original:     [00][00][00][C9]
              ↓ << 24
              [C9][00][00][00]
              ↓ >> 24 (logical)
              [00][00][00][C9]
              ↓ as int
              0x000000C9 = 201

fun2 (Sign Extension)
text

Original:     [00][00][00][C9]
              ↓ (int) cast
              [00][00][00][C9] (still positive)
              ↓ << 24
              [C9][00][00][00] (now MSB=1 → negative)
              ↓ >> 24 (arithmetic)
              [FF][FF][FF][C9]
              ↓ as int
              0xFFFFFFC9 = -55
*/

