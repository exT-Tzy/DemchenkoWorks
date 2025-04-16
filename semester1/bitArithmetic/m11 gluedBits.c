#include <stdio.h>

#pragma warning(disable: 4996)

int main() {
    unsigned int n, fb, lb, gb, mb;
    int len, i;

    printf("Enter integer value: ");
    scanf("%u", &n);
    printf("\nEnter length: ");
    scanf("%d", &len);
    printf("\nEnter amount of bits: ");
    scanf("%d", &i);

    fb = (n >> (len - i)) & ((1 << i) - 1); 
    lb = n & ((1 << i) - 1); 

    gb = (fb << i) | lb;

    mb = (n >> i) & ((1 << (len - 2 * i)) - 1);

    printf("\nGlued bits result = %u", gb);
    printf("\nBits between glued bits result = %u\n", mb);

    return 0;
}
