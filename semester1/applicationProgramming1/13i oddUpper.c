#include <stdio.h>
#include <ctype.h>
#include <string.h>

void oddUpper(char* str) {
	int i;

	for (i = 0; i < strlen(str); i += 2) {
		str[i] = toupper(str[i]);
	}
}

int main() {
	char str[100] = "iudofsfdisncvbxbhrejnldfa";

	printf("Original str: %s\n", str);

	oddUpper(str);

	printf("Transformed str: %s\n", str);

	return 0;
}