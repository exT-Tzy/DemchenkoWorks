#include <iostream>
#include <stdexcept>
#include <bitset>

#include "bugint.h"

int main()
{
	int digits[3] = { INT_MIN , 1, 1 };
	bugint a(digits, 3);

	return 0;
}