#include <stdio.h>

unsigned int recursive_strlen(char* ptr)
{
	if (*ptr == 0 || !ptr) return 0;

	return 1 + recursive_strlen(ptr + 1);
}

int main()
{
	char str[BUFSIZ] = "fjdsjgodsfjg";

	printf("%d\n", recursive_strlen(str));

	return 0;
}