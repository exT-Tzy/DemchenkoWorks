#include <stdio.h>

#define ROWS1 2		//the number of rows in the first matrix
#define COLS1 2		//the number of columns in the first matrix
#define ROWS2 2		//the number of rows in the second matrix
#define COLS2 3		//the number of columns in the second matrix

int matrix_multiplication(int const matrix1[ROWS1][COLS1], int const matrix2[ROWS2][COLS2], int result_matrix[ROWS1][COLS2]) {
	int i, j, k, temp;
	

	if (COLS1 != ROWS2) {
		return 1;
	}

	for (i = 0; i < ROWS1; i++) {
		for (j = 0; j < COLS2; j++) {
			temp = 0;

			for (k = 0; k < COLS1; k++) {
				temp += matrix1[i][k] * matrix2[k][j];
			}

			result_matrix[i][j] = temp;
		}
	}

	return 0;
}

void print_result_matrix(int matrix[ROWS1][COLS2]) {
	int i, j;

	for (i = 0; i < ROWS1; i++) {
		for (j = 0; j < COLS2; j++) {
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}

int main() {
	int matrix1[ROWS1][COLS1] = {
		{ 1, 2 },
		{ 3, 4 }
	};
	int matrix2[ROWS2][COLS2] = {
		{ 1, 2, 3 },
		{ 4, 5, 6 }
	};
	int result_matrix[ROWS1][COLS2], i;

	switch (matrix_multiplication(matrix1, matrix2, result_matrix)) 
	{
	case 0:
		print_result_matrix(result_matrix);
		break;
	case 1:
		printf("Matrix multiplication is not possible!\n");
		break;
	default:
		printf("Undefined behavior o__O\n");
		break;
	}

	return 0;
}