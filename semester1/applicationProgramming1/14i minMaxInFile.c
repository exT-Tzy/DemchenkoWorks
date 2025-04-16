#include <stdio.h>
#include <stdlib.h>

#define N 10 // the number of values in the file

#pragma warning(disable: 4996)

void arrFile(const char* file_name, int* dest) {
	int i;
	FILE* fi = fopen(file_name, "r");

	if (fi == NULL) {
		printf("File open fault!");
		exit(1);
	}

	for (i = 0; i < N; i++) {
		fscanf(fi, "%d", &dest[i]);
	}

	fclose(fi);
}

int maxValue(int* arr) {
	int i, temp = arr[0];

	for (i = 1; i < N; i++) {
		if (arr[i] > temp) {
			temp = arr[i];
		}
	}

	return temp;
}

int minValue(int* arr) {
	int i, temp = arr[0];

	for (i = 1; i < N; i++) {
		if (arr[i] < temp) {
			temp = arr[i];
		}
	}

	return temp;
}

void arrPrint(int* arr) {
	int i;

	for (i = 0; i < N; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int main() {
	int i;
	int arr[N];
	char file_name[BUFSIZ];

	printf("Enter file name: ");
	scanf("%s", file_name);
	printf("\n");

	arrFile(file_name, arr);

	printf("Array = ");
	arrPrint(arr);

	printf("Min value = %d, max value = %d\n", minValue(arr), maxValue(arr));

	return 0;
}