#include<stdio.h>  //printf
#include<stdlib.h> // malloc, calloc, free
#include<stdint.h> // uint64_t
#include<limits.h> // SIZE_MAX
#include<stddef.h> // size_t

/////////////////////////////////////////////////
// Vulnerable version (original bug)
void *bad_malloc(size_t ele_cnt, size_t ele_size) {
    size_t size = ele_cnt * ele_size;
//overflow
    return malloc(size);
}

////////////////////////////////////////////////////
// wrong fix (still vulnerable)
void *wrong_fix_malloc(size_t ele_cnt, size_t ele_size) {
    uint64_t asize = ele_cnt * (uint64_t)ele_size;
    return malloc(asize); // truncated to size
}

////////////////////////////////////////////////////
// correct fix (safe)
void *safe_malloc(size_t ele_cnt, size_t ele_size) {

    // prevent overflow before multiplication
    if (ele_size != 0 && ele_cnt > SIZE_MAX / ele_size) {
        printf("Overflow detected ! Allocation blocked.\n");
        return NULL;
    }

    size_t size = ele_cnt * ele_size;

    if (size == 0) { //optional extre safety
        printf("Zero allocation avoided.\n");
        return NULL;
    }
    return malloc(size);
}

/////////////////////////////////////////////////////
int main() {

    printf("======= Integer Overflow Demo ========\n\n");
    // Dangerous values
    size_t ele_cnt = 0xFFFFFFFF; //very large
    size_t ele_size = 4;

    printf("ele_cnt = %zu\n", ele_cnt);
    printf("ele_size = %zu\n\n", ele_size);

    ////////////////////////////////////////////////
    printf("Vulnerable version:\n");
    void *p1 = bad_malloc(ele_cnt, ele_size);
    printf("Allocated pointer: %p\n\n", p1);

    ////////////////////////////////////////////////
    printf("Wrong 64-bit fix:\n");
    void *p2 = wrong_fix_malloc(ele_cnt, ele_size);
    printf("Allocated pointer: %p\n\n", p2);

    /////////////////////////////////////////////////
    printf("Safe version:\n");
    void *p3 = safe_malloc(ele_cnt, ele_size);
    printf("Allocated pointer: %p\n\n", p3);

    ////////////////////////////////////////////////
    printf("Best (calloc):\n");
    void *p4 = calloc(ele_cnt, ele_size); //safe internally
    printf("Allocated pointer: %p\n\n", p4);

    /////////////////////////////////////////////

    free(p1);
    free(p2);
    free(p3);
    free(p4);

    return 0;
}