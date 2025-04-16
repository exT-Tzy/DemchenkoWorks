#include <stdio.h>

char* analogStrStr(const char* str, const char* sub_str) {
	char* str_ptr = str;
	char* sub_str_ptr = sub_str;
	char* str_temp;
	char* sub_str_temp;

	if (*sub_str == 0) {
		return (char*)str;
	}

	while (*str_ptr != 0) {

		if (*str_ptr == *sub_str_ptr) {
			str_temp = str_ptr;
			sub_str_temp = sub_str;

			while (*str_temp != 0 && *sub_str_temp != 0 && *str_temp == *sub_str_temp) {
				str_temp++;
				sub_str_temp++;
			}

			if (*sub_str_temp == 0) {
				return (char*)str_ptr;
			}
		}

		str_ptr++;
	}

	return NULL;
}

int main() {
	char str[100] = "1234567";
	char substr[100] = "345";

	char* result = analogStrStr(str, substr);

	if (result == NULL) {
		printf("Substring not found\n");
	}
	else {
		printf("Substring found\n");
	}

	return 0;
}