#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include "fibonacci_priority_queue.h"

int main()
{
    fibonacci_priority_queue pq;

    // Testing element insertion

    std::cout << "Adding elements to the queue..." << std::endl;
    pq.push("low priority task", 1);
    pq.push("high priority task", 10);
    pq.push("medium priority task", 5);
    pq.push("critical task", 15);
    pq.push("normal task", 3);

    // Testing element extraction by priority

    std::cout << "\nExtracting elements in descending priority order:" << std::endl;
    while (!pq.empty())
    {
        const char* top_value = pq.get_top_value();
        if (top_value)
        {
            std::cout << "Element: " << top_value << std::endl;
            pq.pop();
        }
        else
        {
            break;
        }
    }

    // Testing queue merging

    std::cout << "\nTesting queue merging..." << std::endl;
    fibonacci_priority_queue pq1;
    fibonacci_priority_queue pq2;

    pq1.push("task A", 8);
    pq1.push("task B", 3);

    pq2.push("task C", 12);
    pq2.push("task D", 6);

    std::cout << "Queue 1 before merge: " << pq1.get_top_value() << std::endl;
    std::cout << "Queue 2 before merge: " << pq2.get_top_value() << std::endl;

    pq1.merge(&pq2);

    std::cout << "After merge, extracting all elements:" << std::endl;
    while (!pq1.empty())
    {
        const char* top_value = pq1.get_top_value();
        if (top_value)
        {
            std::cout << "Element: " << top_value << std::endl;
            pq1.pop();
        }
        else
        {
            break;
        }
    }

    // Testing copying

    std::cout << "\nTesting copying..." << std::endl;

    fibonacci_priority_queue original;

    original.push("original 1", 7);
    original.push("original 2", 14);

    fibonacci_priority_queue copy = original;

    std::cout << "Original queue: " << original.get_top_value() << std::endl;
    std::cout << "Copied queue: " << copy.get_top_value() << std::endl;

    original.pop();

    std::cout << "After pop from original:" << std::endl;
    std::cout << "Original queue: " << (original.get_top_value() ? original.get_top_value() : "empty") << std::endl;
    std::cout << "Copied queue: " << copy.get_top_value() << std::endl;

    return 0;
}