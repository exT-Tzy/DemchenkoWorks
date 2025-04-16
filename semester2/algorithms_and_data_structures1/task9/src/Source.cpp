#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdexcept>

#include "leftist_priority_queue.h"

int main()
{
	leftist_priority_queue lpq;
	leftist_priority_queue lpq1;

	lpq.push("1", 1);
	lpq.push("2", 3);
	lpq.push("3", 124);
	lpq.push("4", 34);
	lpq.push("5", 2);
	lpq.push("6", 13);
	
	{
		lpq1.push("a", 412);
		lpq1.push("b", 1241241);
		lpq1.push("c", 234234);
		lpq1.push("d", 1221);
		lpq1.push("e", 453);
		lpq1.push("f", 2222222);
		lpq1.push("g", 32);

		lpq.merge(&lpq1);
	}

	while (!lpq.is_empty())
	{
		std::cout << lpq.get_top_value() << std::endl;
		lpq.pop();
	}

	return 0;
}