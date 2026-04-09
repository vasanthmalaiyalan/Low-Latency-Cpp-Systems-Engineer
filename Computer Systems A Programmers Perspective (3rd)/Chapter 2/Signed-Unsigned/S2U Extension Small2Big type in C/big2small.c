#include<stdio.h>

typedef unsigned char *byte_pointer;

// Hex view
void show_bytes(byte_pointer start, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02x ", start[i]);
    }
    printf("\n");
}

// Binary view 
void show_bits(byte_pointer start, size_t len) {
    for (size_t i = 0; i < len; i++) {
        unsigned char byte = start[i];
        for (int j = 7; j >= 0; j--) {
            printf("%d", (byte >> j) & 1);
        }
        printf(" ");
    }
    printf("\n");
}

int main() {

    // Example 1 (book example)
    int x = 53191;

    short sx = (short)x; // truncation (32 -> 16)
    int y = sx;  // signed extenstion back

    printf("========= Example 1 ===========\n");
    printf("x = %d\n", x);
    printf("sx = %d\n", sx);
    printf("y  = %d\n", y);

    printf("Memory:\n");
    printf("x (int)   : "); show_bytes((byte_pointer)&x, sizeof(int));
    printf("sx        : "); show_bytes((byte_pointer)&x, sizeof(short));
    printf("y         : "); show_bytes((byte_pointer)&y, sizeof(int));

    printf("\nMemory (BITS):\n");
    printf("x         : "); show_bits((byte_pointer)&x, sizeof(int));
    printf("sx        : "); show_bits((byte_pointer)&sx, sizeof(short));
    printf("y         : "); show_bits((byte_pointer)&y,  sizeof(int));

    // Example 2
    unsigned int a = 70000;
    unsigned short sa = (unsigned short)a;

    printf("\n======== Example 2 ===========\n");
    printf("a  = %u\n", a);
    printf("sa = %u\n\n", sa);

    printf("Memory (HEX):\n");
    printf("a   : "); show_bytes((byte_pointer)&a, sizeof(unsigned int));
    printf("sa  : "); show_bytes((byte_pointer)&sa, sizeof(unsigned short));

    printf("\nMemory (BITS):\n");
    printf("a   : "); show_bits((byte_pointer)&a, sizeof(unsigned int));
    printf("sa  : "); show_bits((byte_pointer)&sa, sizeof(unsigned short));

    return 0;
   
}