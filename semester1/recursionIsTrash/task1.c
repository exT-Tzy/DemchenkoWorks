#include <stdio.h>

unsigned int recursive_factorial(unsigned int num)
{
	if (num == 0 || num == 1) return 1;

	return num * recursive_factorial(num - 1);
}

int main()
{
	printf("%u\n", recursive_factorial(5));

	return 0;
}