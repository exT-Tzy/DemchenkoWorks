#include <stdio.h>

#pragma warning(disable: 4996)

void reverseStr(char *str) {
	int len = 0, i, j;
	char temp;

	while (str[len] != '\0') {
		len++;
	}

	for (i = 0, j = len - 1; i < j; i++, j--) {
		temp = str[j];
		str[j] = str[i];
		str[i] = temp;
	}
}

int main() {
	char str[100];

	printf("Enter string: ");
	scanf("%s", str);

	reverseStr(str);

	printf("\nReversed string: %s\n", str);

	return 0;
}