#include <stdio.h>

#define N 10 // the number of values in array

int main() {
	int unique_count = 1, is_unique, i, j;
	double arr[N] = { 1.2, 1.5, 54.23, 12, 1.5, 1.2, 25.421, 537.12342, 423.656, 543 };
	double newArr[BUFSIZ] = { 0 };
	double* newArr_ptr = newArr;

	*newArr_ptr++ = arr[0];

	printf("Original array: ");

	for (i = 0; i < N; i++) {
		printf("%f ", arr[i]);
	}

	for (i = 1; i < N; i++) {
		is_unique = 1;

		for (j = 0; j < unique_count; j++) {
			if (arr[i] == newArr[j]) {
				is_unique = 0;
				break;
			}
		}

		if (is_unique == 1) {
			*newArr_ptr++ = arr[i];
			unique_count++;
		}
	}

	printf("\nNew array: ");

	for (i = 0; i < unique_count; i++) {
		printf("%f ", newArr[i]);
	}

	printf("\n");

	return 0;
}