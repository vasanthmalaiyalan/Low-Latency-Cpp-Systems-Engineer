#include<stdio.h>

int main() {
    int x = 42;
    int *ptr = &x;
    
    printf("Size of int:      %zu bytes\n", sizeof(int));
    printf("Size of pointer:  %zu bytes\n",sizeof(ptr));
    printf("Size of long:     %zu bytes\n",sizeof(long));
    printf("address of ptr:   %p\n",&ptr);
    printf("value of ptr:     %p\n",ptr);
    printf("address of x:      %p\n",&x);

    return 0;

}