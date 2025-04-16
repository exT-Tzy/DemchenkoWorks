#include <stdio.h>

#pragma warning(disable: 4996)

int main() {
	int a, k, mask;
	const c = sizeof(int) * 8 - 1;
	printf("Enter integer value: ");
	scanf("%d", &a);
	printf("\nEnter bit number (0 - %d): ", c);
	scanf("%d", &k);

	if (k < 0 || k > c) {
		printf("\nMemory access error!");
		return 1;
	}

	mask = 1 << k;

	if (a & mask != 0) {
		printf("\nBit value = 1\n");
	}
	else {
		printf("\nBit value = 0\n");
	}

	return 0;
}