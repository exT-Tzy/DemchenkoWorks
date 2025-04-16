#include <stdio.h>

#pragma warning(disable: 4996)

int main() {
	int a, j, i, mask_i, mask_j, bit_i, bit_j;
	const c = sizeof(int) * 8 - 1;
	printf("Enter integer value: ");
	scanf("%d", &a);
	printf("\nEnter first bit number (0 - %d): ", c);
	scanf("%d", &i);
	printf("\nEnter second bit number (0 - %d): ", c);
	scanf("%d", &j);

	if (i < 0 || i > c || j < 0 || j > c) {
		printf("\nMemory access error!");
		return 1;
	}

	mask_i = 1 << i;
	mask_j = 1 << j;

	bit_i = a & mask_i;
	bit_j = a & mask_j;

	if ((bit_i != bit_j) || (bit_i > 0 && bit_j > 0) || (bit_i != 0 && bit_j != 0)) {
		a = a ^ mask_i;
		a = a ^ mask_j;
	}

	printf("\nTransformed value = %d", a);

	return 0;
}