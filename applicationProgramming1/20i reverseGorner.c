#include <stdio.h>
//#include <ctype.h>
#include <string.h>

char* inverse_gorner(int num, int base) {
    if (base < 2 || base > 36) {
        return NULL;
    }

    char buf[33];
    int i = 0;
    int is_negative = 0;

    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    do {
        int digit = num % base;
        buf[i++] = (digit < 10) ? digit + '0' : digit - 10 + 'A';
        num /= base;
    } while (num > 0);

    if (is_negative) {
        buf[i++] = '-';
    }

    buf[i] = '\0';
    
    _strrev(buf); 
    
    return buf;
}

int main() {
    int number = -92347;
    int base = 25; 

    char* result = inverse_gorner(number, base);

    if (result) {
        printf("%d(10) == %s(%d)\n", number, result, base);
    }
    else {
        printf("Invalid base value!\n");
    }

    return 0;
}

