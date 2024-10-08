#include <stdio.h>

#define N 10

int evenValuesCpy(const int *src, int *dest) {
	int c = 0, i;

	for (i = 0; i < N; i++) {
		if (src[i] % 2 == 0) {
			*dest++ = src[i];
			c++;
		}
	}

	return c;
}

int main() {
	int arr[N] = { 42, 1, 65, 123, 63, 21, 52, 68, 24, 53 };
	int i, c, evenValuesArr[BUFSIZ] = { 0 };

	printf("Original array: ");

	for (i = 0; i < N; i++) {
		printf("%d ", arr[i]);
	}

	c = evenValuesCpy(arr, evenValuesArr);

	printf("\nEven values in array: ");

	for (i = 0; i < c; i++) {
		printf("%d ", evenValuesArr[i]);
	}

	return 0;
}