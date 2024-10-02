#include <stdio.h>

#pragma warning(disable: 4996)

int maxPow(int a) {
    if (a == 0) {
        return -1;
    }
    else {
        return 1 + maxPow(a >> 1); 
    }
}

int main() {
    int n;

    printf("Enter integer value: ");
    scanf("%d", &n);

    int pow = maxPow(n);

    if (pow == -1) {
        printf("Max powew = unlimited quantity\n");
    }
    else {
        printf("Max power = %d\n", pow);
    }

    return 0;
}
