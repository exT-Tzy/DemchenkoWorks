#include <iostream>
#include <stdexcept>

#include "matrix.h"

int main()
{
	matrix matr(2, 2);

	matr[0][0] = 1;
	matr[0][1] = 2;
	matr[1][0] = 3;
	matr[1][1] = 4;

	matrix i;

	i = 3 * matr;

	std::cout << i << std::endl;

	return 0;
}