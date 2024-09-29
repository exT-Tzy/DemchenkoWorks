#include <stdio.h>

#pragma warning (disable: 4996)

void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

int main() {
	int num1, num2;

	printf("Enter first value: ");
	scanf("%d", &num1);
	printf("\nEnter second value: ");
	scanf("%d", &num2);

	swap(&num1, &num2);

	printf("\nSwapped values: % d % d\n", num1, num2);

	return 0;
}