#include <iostream>
#include <stdexcept>

#include "logical_values_array.h"

int main()
{
	logical_values_array arr1(0b1100);
	logical_values_array arr2(0b1010);

	logical_values_array arr3 = arr1 | arr2;

	char buf[sizeof(unsigned int) * 8 + 1];

	arr3.to_binary_string(buf);

	std::cout << buf << std::endl;

	return 0;
}