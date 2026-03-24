#include<stdio.h>
#include<limits.h>
#include<stdint.h>
#include<inttypes.h>

int main() {
    printf("=== Where to range come form? ====\n");

    //=============================
    // char (1 byte = 8 bits)
    //================================
    printf("---- char (1 byte = 8 bits) --\n");
    printf("unsigned char: 0 to %u\n", UCHAR_MAX);
    printf("signed char: %d to %d\n\n", SCHAR_MIN,SCHAR_MAX);

    //================================
    // Short ( 2 bytes = 16 bits)
    //==================================
    printf("------- short (2 byte = 16 bits) -----\n");
    printf("unsigned short: 0 to %u\n", USHRT_MAX);
    printf("signed short: %d to %d\n\n", SHRT_MIN,SHRT_MAX);

    //=======================================
    //int (4 byte = 32 bits typically)
    //==========================================
    printf("---int---");
    printf("unsigned int: 0 to %u\n", UINT_MAX);
    printf("signed int : %d to %d\n\n",INT_MIN, INT_MAX);

    //============================================
    // long (machine dependent)
    //============================================
    printf("---long (machine dependent)-----\n");
    printf("size of long: %zu bytes\n", sizeof(long));
    printf("signed long: %ld to %ld\n", LONG_MIN,LONG_MAX);
    printf("unsigned long: 0 TO %lu\n\n", ULONG_MAX);

    //=====================================
    // Fixed Size type (protable)
    //======================================
    printf("=== Fixed Size Types ===\n\n");

    printf("int32_t : %d to %d\n", INT32_MIN,INT32_MAX);
    printf("uint32_t : 0 to %u\n\n", UINT32_MAX);

    printf("int64_t : %" PRId64 " to %" PRId64 "\n", INT64_MIN,INT64_MAX);
    printf("uint64_t : 0 to %" PRIu64 "\n\n",UINT64_MAX);

    //======================================
    // ASmmetry (CSAPP key concept)
    //========================================
    printf("==== ASYMMETRY (IMPORTANT) ====\n\n");

    printf(" signed char:\n");
    printf(" Negative range: %d to -1\n",SCHAR_MIN);
    printf(" Positive range: 1 to %d\n", SCHAR_MAX);
    printf(" Zero: 0\n");
    printf(" -> Negative side has ONE extra value! \n\n");

    printf(" int:\n");
    printf(" Min: %d\n", INT_MIN);
    printf(" MAX: %d\n", INT_MAX);
    printf(" |Min| = %u\n", (unsigned)(-(INT_MIN + 1)) + 1);
    printf(" |Max| = %d\n", INT_MAX);
    printf(" -> Negative side has one extra value!\n\n");

    //================================================
    // Why asmmentry?
    //==================================================
    printf("== WHY ASYMMETRY? ==\n\n");

    printf(" 1 pattern -> zero\n");
    printf(" 127 pattern -> positive\n");
    printf(" 128 pattern -> negative\n");
    printf(" -> two's complement reason\n\n");

    //================================================
    //32-bit vs 64-bit
    //===============================================
    printf("===== 32-bit vs 64-bit =====\n\n");

    printf("Type       Size\n");
    printf("-----------------\n\n");
    printf("char   %zu bytes\n", sizeof(char));
    printf("short  %zu bytes\n", sizeof(short));
    printf("int    %zu bytes\n", sizeof(int));
    printf("long   %zu bytes\n", sizeof(long));
    printf("pointer %zu bytes\n", sizeof(void*));

    return 0;
}
