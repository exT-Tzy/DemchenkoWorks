#include <stdio.h>
#include <math.h>

unsigned int recursive_array_comparison(
	double* arr1,
	double* arr2,
	size_t count_of_elements,
	double eps)
{
	if (!arr1 || !arr2) return 1;

	if (count_of_elements == 0) return 0;

	if (fabs(*arr1 - *arr2) < eps) printf("%f = %f\n", *arr1, *arr2);
	else if (*arr1 > *arr2) printf("%f > %f\n", *arr1, *arr2);
	else printf("%f < %f\n", *arr1, *arr2);

	return recursive_array_comparison(arr1 + 1, arr2 + 1, count_of_elements - 1, eps);
}

int main()
{
	double arr1[BUFSIZ] = { 1.123, 2.11, 3.12, 2.43, 5.221, 6.66, 4.23 };
	double arr2[BUFSIZ] = { 2.11, 3.12, 41.111, 123.2, 0.0, 6.66, 4.24 };

	recursive_array_comparison(arr1, arr2, 7, 0.0000000000001);

	return 0;
}