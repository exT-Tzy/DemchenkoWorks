#include <stdio.h>

#pragma warning(disable: 4996)

#define N 10 // array size

void arrPrint(int *array) {
	int n;
	for (n = 0; n < N; ++n) {
		printf("%d ", array[n]);
	}
	printf("\n");
}

int main() {
	int i, temp = 0, c = 0;
	int arr[N] = { 5, 2, 7, 12, 74, 23, 123, 32, 64, 321 };
	printf("Original array: ");
	arrPrint(arr);
	while (c != N - 1) {
		for (i = 0; i < N - 1; ++i) {
			if (arr[i] > arr[i + 1]) {
				temp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = temp;
			}
			else {
				c += 1;
			}
		}
		if (c == N - 1) {
			break;
		}
		c = 0;
	}
	printf("Sorted array: ");
	arrPrint(arr);
	return 0;
}