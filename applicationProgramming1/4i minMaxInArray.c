#include <stdio.h>

#define N 10

void minMax(int const *arr, int *min_placement, int *max_placement) {
	int *arr_ptr, i;
	
	*min_placement = arr[0];
	*max_placement = arr[0];
	arr_ptr = arr + 1;

	for (i = 1; i < N; i++) {
		if (*arr_ptr > *max_placement) {
			*max_placement = *arr_ptr;
		}
		if (*arr_ptr < *min_placement) {
			*min_placement = *arr_ptr;
		}

		arr_ptr++;
	}
}

void arrPrint(int *arr) {
	int i;

	for (i = 0; i < N; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int main() {
	int minValue, maxValue;
	int arr[N] = { 1, 123, -12, 54, 23, -68, 143, 437, 88, 52 };

	printf("Array = ");
	arrPrint(arr);

	minMax(arr, &minValue, &maxValue);

	printf("Min value = %d, max value = %d\n", minValue, maxValue);

	return 0;
}