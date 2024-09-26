#include <stdio.h>

#pragma warning(disable: 4996)

int main() {
	int a, k, mask;
	const c = sizeof(int) * 8;
	printf("Enter integer value: ");
	scanf("%d", &a);
	printf("\nEnter amount of bits (0 - %d): ", c);
	scanf("%d", &k);

	if (k < 0 || k > c) {
		printf("\nMemory access error!");
		return 1;
	}

	mask = 0;
	mask = (~mask) << k;

	a = a & mask;

	printf("\nTransformed value = %d", a);

	return 0;
}