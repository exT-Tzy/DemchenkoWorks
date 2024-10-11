#include <stdio.h>

void my_strcat(char* dest, const char* src) {
    while (*dest) {
        dest++;
    }
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}

int main() {
    char str1[BUFSIZ] = "uheiwrtf3478576934d98DFSU()()_(()HJIDHSIds000_ooo";
    char str2[BUFSIZ] = "77YY7YDHFSHDFSHHFHncdxnvnn<<>A<SD<899";

    my_strcat(str1, str2);

    printf("%s\n", str1);

	return 0;
}