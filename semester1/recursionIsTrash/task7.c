#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

uint64_t recursive_strict_disjunction_of_bits(uint64_t x) 
{
    if (x == 0 || x == 1) return x;

    uint64_t left_part = x >> 1;
    uint64_t right_part = x & 1;

    return recursive_strict_disjunction_of_bits(left_part) ^ right_part;
}

int main()
{
	uint64_t num = 0b1011;

    printf("%I64u", recursive_strict_disjunction_of_bits(num));

	return 0;
}