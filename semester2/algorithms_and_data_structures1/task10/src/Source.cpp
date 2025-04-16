#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdexcept>

#include "skew_priority_queue.h"

int main()
{
	skew_priority_queue spq;
	skew_priority_queue spq1;

	spq.push("1", 1);
	spq.push("2", 3);
	spq.push("3", 124);
	spq.push("4", 34);
	spq.push("5", 2);
	spq.push("6", 13);

	{
		spq1.push("a", 412);
		spq1.push("b", 1241241);
		spq1.push("c", 234234);
		spq1.push("d", 1221);
		spq1.push("e", 453);
		spq1.push("f", 2222222);
		spq1.push("g", 32);

		spq.merge(&spq1);
	}

	while (!spq.is_empty())
	{
		std::cout << spq.get_top_value() << std::endl;
		spq.pop();
	}

	return 0;
}