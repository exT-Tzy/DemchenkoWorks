#include <stdio.h>

double recursive_float_pow(
	double num,
	unsigned int pow)
{
	if (pow == 0) return 1;
	
	return num * recursive_float_pow(num, pow - 1);
}

int main()
{
	printf("%f\n", recursive_float_pow(12.3, 3));

	return 0;
}