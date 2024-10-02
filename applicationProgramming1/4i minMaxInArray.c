#include <stdio.h>

#define N 10

int maxValue(int *arr) {
	int i, temp = arr[0];

	for (i = 1; i < N; i++) {
		if (arr[i] > temp) {
			temp = arr[i];
		}
	}

	return temp;
}

int minValue(int *arr) {
	int i, temp = arr[0];

	for (i = 1; i < N; i++) {
		if (arr[i] < temp) {
			temp = arr[i];
		}
	}

	return temp;
}

void arrPrint(int *arr) {
	int i;

	for (i = 0; i < N; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int main() {
	int arr[N] = { 1, 123, -12, 54, 23, -68, 143, 437, 88, 52 };

	printf("Array = ");
	arrPrint(arr);

	printf("Min value = %d, max value = %d\n", minValue(arr), maxValue(arr));

	return 0;
}