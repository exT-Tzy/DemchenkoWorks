#include <stdio.h>

int analogStrLen(const char* str) {
	int i = 0;
	while (str[i] != 0) {
		i++;
	}
	return i;
}

int main() {
	char str[100] = "авпрд";

	printf("Original string: %s\n", str);
	printf("String length = %d\n", analogStrLen(str));

	return 0;
}