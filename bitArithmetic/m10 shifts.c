#include <stdio.h>

#pragma warning(disable: 4996)

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

#define ROTATE_RIGHT(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

int main() {
    unsigned int v, leftShifted, rightShifted;
    int p, n;

    printf("Enter 2^p bit integer value: ");
    scanf("%u", &v);

    printf("\nEnter p value: ");
    scanf("%d", &p);

    printf("\nEnter amount of bits for shift: ");
    scanf("%d", &n);

    leftShifted = ROTATE_LEFT(v, n);
    printf("\nTransformed value after left shift = %u", leftShifted);

    rightShifted = ROTATE_RIGHT(v, n);
    printf("\nTransformed value after right shift = %u", rightShifted);

    return 0;
}
