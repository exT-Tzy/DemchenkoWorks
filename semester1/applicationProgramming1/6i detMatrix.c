#include <stdio.h>

#define N 2

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

    if (N == 1) {
        result = matrix[0][0];
    }
    else if (N == 2) {
        result = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    else {
        for (i = 0; i < N; i++) {
            result *= matrix[i][i];
        }
    }

    return result;
}

void canonicalize(int matrix[N][N]) {
    // TODO
}

int main() {
    int matrix[N][N] = { { -4, 1 },
                         { 5, 6 } };

    printf("Matrix = \n\n");
    printMatrix(matrix);
    printf("Determinant = %d\n", detMatrix(matrix));

	return 0;
}