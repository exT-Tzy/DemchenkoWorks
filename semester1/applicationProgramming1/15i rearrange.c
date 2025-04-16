#include <stdio.h>
#include <string.h>
#include <ctype.h>

#pragma warning(disable: 4996)

void rearrange(char* str) {
	char digits[BUFSIZ] = { 0 };
	char letters[BUFSIZ] = { 0 };
	char others[BUFSIZ] = { 0 };
	char* digits_ptr = digits, * letters_ptr = letters, * others_ptr = others, *str_ptr = str;

	while (*str_ptr++) {
		if (isdigit(*str_ptr)) {
			*digits_ptr++ = *str_ptr;
		}
		else if (isalpha(*str_ptr)) {
			*letters_ptr++ = *str_ptr;
		}
		else {
			*others_ptr++ = *str_ptr;
		}
	}

	*digits_ptr = 0;
	*letters_ptr = 0;
	*others_ptr = 0;

	strcpy(str, digits);
	strcat(str, letters);
	strcat(str, others);
}

int main() {
	char str[BUFSIZ] = "34yhdf&^*DSgfusdu90324236";

	printf("Original str: %s\n", str);

	rearrange(str);

	printf("Transformed str: %s\n", str);

	return 0;
}