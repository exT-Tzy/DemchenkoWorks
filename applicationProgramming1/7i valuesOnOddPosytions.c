#include <stdio.h>

#define N 11

void oddPosytionsCopy(int const* src, int* dest) {
	int i;

	for (i = 0; i < N; i += 2) {
		*dest++ = src[i];
	}
}

int main() {
	int arr[N] = { 45, 12, 64, 1, 23, 6, 932, 54, 52, 25, 431 };
	int oddPosArr[BUFSIZ] = { 0 }, c, i;
	
	c = N / 2;

	if (N % 2) {
		c += 1;
	}

	printf("Original array: ");

	for (i = 0; i < N; i++) {
		printf("%d ", arr[i]);
	}

	oddPosytionsCopy(arr, oddPosArr);

	printf("\nValues on odd positions: ");

	for (i = 0; i < c; i++) {
		printf("%d ", oddPosArr[i]);
	}

	return 0;
}