#include <stdio.h>

#pragma warning(disable: 4996)

int main() {
    int a, mask = 1, i = 0;
    char input[100];

    printf("Enter integer value: ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("\nInput error!");
        return 1;
    }

    while (input[i] != '\n') {
        if ((input[i] == '-' || input[i] == '+') && i == 0) {
            ++i;
        }
        else if (input[i] >= '0' && input[i] <= '9') {
            ++i;
        }
        else {
            printf("\nInput error!");
            return 2;
        }
    }

    if (sscanf(input, "%d", &a) != 1) {
        printf("\nTransformation fault!");
        return 3;
    }

    printf("\nLowest bit = %d", a & mask);

    return 0;
}
