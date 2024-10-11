#include <stdio.h>

#define N 3

void printMatrix(int const matrix[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}

int detMatrix(int const matrix[N][N]) {
    int result = 1, i;

    for (i = 0; i < N; i++) {
        result *= matrix[i][i];
    }

    return result;
}

int main() {
    int matrix[N][N] = { { -4, 1, -1 },
                         { 5, 6, -3 },
                         { 2, 4, 5 } };

    printf("Matrix = \n\n");
    printMatrix(matrix);
    printf("Determinant = %d\n", detMatrix(matrix));

	return 0;
}