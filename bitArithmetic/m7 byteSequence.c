#include <stdio.h>

#pragma warning(disable: 4996)

int main() {
    int a, byte0, byte1, byte2, byte3, mask = 255, i, s[4], gluedBytes = 0;
    printf("Enter integer value: ");
    scanf("%d", &a);
    printf("\nEnter byte sequence (example: 1 2 3 4): ");
    
    for (i = 0; i < 4; ++i) {
        scanf("%d", &s[i]);
    }

    byte3 = a & mask;
    byte2 = (a >> 8) & mask;
    byte1 = (a >> 16) & mask;
    byte0 = (a >> 24) & mask;

    for (i = 0; i < 4; ++i) {
        if (s[i] == 1) {
            gluedBytes = gluedBytes | byte0;
        }
        if (s[i] == 2) {
            gluedBytes = gluedBytes | byte1;
        }
        if (s[i] == 3) {
            gluedBytes = gluedBytes | byte2;
        }
        if (s[i] == 4) {
            gluedBytes = gluedBytes | byte3;
        }
        if (i != 3) {
            gluedBytes = gluedBytes << 8;
        }
    }

    printf("\nTransformed value = %d\n", gluedBytes);

    return 0;
}
