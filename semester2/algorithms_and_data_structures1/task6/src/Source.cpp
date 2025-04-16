#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdexcept>

#include "binary_priority_queue.h"

int main()
{
	binary_priority_queue bpq;
	binary_priority_queue bpq1;

	bpq.push("1", 1);
	bpq.push("2", 3);
	bpq.push("3", 124);
	bpq.push("4", 34);
	bpq.push("5", 2);
	bpq.push("6", 13);

	bpq1.push("a", 412);
	bpq1.push("b", 1241241);
	bpq1.push("c", 234234);
	bpq1.push("d", 1221);
	bpq1.push("e", 453);
	bpq1.push("f", 2222222);
	bpq1.push("g", 32);

	bpq.merge(&bpq1);


	while (!bpq.is_empty())
	{
		std::cout << bpq.get_top_value() << std::endl;
		bpq.pop();
	}

	return 0;
}