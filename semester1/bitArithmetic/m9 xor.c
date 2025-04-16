#include <stdio.h>

#pragma warning(disable: 4996)

int main() {
    unsigned int a;
    int p, bitAmount, i, temp;
    bitAmount = sizeof(a) * 8;

    printf("Enter 2^p bit integer value: ");
    scanf("%u", &a);

    printf("\nEnter p value (1 - %d): ", bitAmount);
    scanf("%d", &p);

    if (p <= 0 || p > bitAmount) {
        printf("\nInput error!");
        return 1;
    }

    for (i = 0; i < p; ++i) {
        if (i == 0) {
            temp = a % 2;
            a >>= 1;
        }
        else {
            temp = (a % 2) ^ temp;
            a >>= 1;
        }
    }

    printf("\nResult = %d\n", temp);

    return 0;
}
