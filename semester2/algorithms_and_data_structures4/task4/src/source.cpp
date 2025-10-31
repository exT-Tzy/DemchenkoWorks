#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#include "../include/allocator_boundary_tags.h"
#include "../include/client_logger_builder.h"

struct test_struct
{
    int a;
    double b;
    char c;

    test_struct(int a_val, double b_val, char c_val) : a(a_val), b(b_val), c(c_val)
    {
        std::cout << "test_struct constructed: " << a << ", " << b << ", " << c << std::endl;
    }

    ~test_struct()
    {
        std::cout << "test_struct destructed: " << a << ", " << b << ", " << c << std::endl;
    }
};

void test_basic_allocation()
{
    std::cout << "\n    Basic Allocation" << std::endl;

    logger *log = client_logger_builder()
                      .add_console_stream(logger::severity::debug)
                      ->add_console_stream(logger::severity::information)
                      ->add_console_stream(logger::severity::warning)
                      ->add_console_stream(logger::severity::error)
                      ->add_file_stream("boundary_test.log", logger::severity::trace)
                      ->build();

    {
        allocator_boundary_tags alloc(1024, nullptr, log, allocator_with_fit_mode::fit_mode::first_fit);

        void *int_block = alloc.allocate(sizeof(int), 1);
        int *int_ptr = static_cast<int *>(int_block);
        allocator::construct(int_ptr, 42);

        std::cout << "Allocated int: " << *int_ptr << std::endl;

        allocator::destruct(int_ptr);
        alloc.deallocate(int_block);
    }

    delete log;
}

void test_multiple_allocations()
{
    std::cout << "\n    Multiple Allocations" << std::endl;

    logger *log = client_logger_builder()
                      .add_console_stream(logger::severity::debug)
                      ->add_console_stream(logger::severity::information)
                      ->build();

    {
        allocator_boundary_tags alloc(2048, nullptr, log, allocator_with_fit_mode::fit_mode::first_fit);

        std::vector<void *> blocks;

        for (int i = 0; i < 5; ++i)
        {
            void *block = alloc.allocate(sizeof(double), 1);
            double *dbl_ptr = static_cast<double *>(block);
            allocator::construct(dbl_ptr, 3.14 * i);

            std::cout << "Allocated double[" << i << "]: " << *dbl_ptr << std::endl;

            blocks.push_back(block);
        }

        for (size_t i = 0; i < blocks.size(); ++i)
        {
            double *dbl_ptr = static_cast<double *>(blocks[i]);
            allocator::destruct(dbl_ptr);
            alloc.deallocate(blocks[i]);
        }
    }

    delete log;
}

void test_struct_allocation()
{
    std::cout << "\n    Struct Allocation" << std::endl;

    logger *log = client_logger_builder()
                      .add_console_stream(logger::severity::debug)
                      ->build();

    {
        allocator_boundary_tags alloc(1024, nullptr, log);

        void *struct_block = alloc.allocate(sizeof(test_struct), 1);
        test_struct *struct_ptr = static_cast<test_struct *>(struct_block);
        allocator::construct(struct_ptr, 100, 99.99, 'A');

        std::cout << "Struct values: " << struct_ptr->a << ", " << struct_ptr->b << ", " << struct_ptr->c << std::endl;

        allocator::destruct(struct_ptr);
        alloc.deallocate(struct_block);
    }

    delete log;
}

void test_fit_modes()
{
    std::cout << "\n    Different Fit Modes" << std::endl;

    logger *log = client_logger_builder()
                      .add_console_stream(logger::severity::information)
                      ->add_console_stream(logger::severity::debug)
                      ->build();

    {
        std::cout << "\n--- First Fit" << std::endl;
        allocator_boundary_tags alloc_first(2048, nullptr, log, allocator_with_fit_mode::fit_mode::first_fit);

        void *b1 = alloc_first.allocate(sizeof(int), 10);
        void *b2 = alloc_first.allocate(sizeof(int), 20);
        void *b3 = alloc_first.allocate(sizeof(int), 15);

        alloc_first.deallocate(b2);

        void *b4 = alloc_first.allocate(sizeof(int), 10);

        alloc_first.deallocate(b1);
        alloc_first.deallocate(b3);
        alloc_first.deallocate(b4);
    }

    {
        std::cout << "\n--- Best Fit" << std::endl;
        allocator_boundary_tags alloc_best(2048, nullptr, log, allocator_with_fit_mode::fit_mode::the_best_fit);

        void *b1 = alloc_best.allocate(sizeof(int), 10);
        void *b2 = alloc_best.allocate(sizeof(int), 20);
        void *b3 = alloc_best.allocate(sizeof(int), 15);

        alloc_best.deallocate(b2);

        void *b4 = alloc_best.allocate(sizeof(int), 10);

        alloc_best.deallocate(b1);
        alloc_best.deallocate(b3);
        alloc_best.deallocate(b4);
    }

    {
        std::cout << "\n--- Worst Fit" << std::endl;
        allocator_boundary_tags alloc_worst(2048, nullptr, log, allocator_with_fit_mode::fit_mode::the_worst_fit);

        void *b1 = alloc_worst.allocate(sizeof(int), 10);
        void *b2 = alloc_worst.allocate(sizeof(int), 20);
        void *b3 = alloc_worst.allocate(sizeof(int), 15);

        alloc_worst.deallocate(b2);

        void *b4 = alloc_worst.allocate(sizeof(int), 10);

        alloc_worst.deallocate(b1);
        alloc_worst.deallocate(b3);
        alloc_worst.deallocate(b4);
    }

    delete log;
}

void test_coalescing()
{
    std::cout << "\n    Memory Coalescing" << std::endl;

    logger *log = client_logger_builder()
                      .add_console_stream(logger::severity::debug)
                      ->add_console_stream(logger::severity::information)
                      ->build();

    {
        allocator_boundary_tags alloc(2048, nullptr, log);

        void *b1 = alloc.allocate(sizeof(int), 10);
        void *b2 = alloc.allocate(sizeof(int), 10);
        void *b3 = alloc.allocate(sizeof(int), 10);

        std::cout << "\nDeallocating middle block..." << std::endl;
        alloc.deallocate(b2);

        std::cout << "\nDeallocating first block..." << std::endl;
        alloc.deallocate(b1);

        std::cout << "\nDeallocating last block..." << std::endl;
        alloc.deallocate(b3);
    }

    delete log;
}

void test_exception_handling()
{
    std::cout << "\n    Exception Handling" << std::endl;

    logger *log = client_logger_builder()
                      .add_console_stream(logger::severity::error)
                      ->add_console_stream(logger::severity::debug)
                      ->build();

    {
        allocator_boundary_tags alloc1(1024, nullptr, log);
        allocator_boundary_tags alloc2(1024, nullptr, log);

        void *block = alloc1.allocate(sizeof(int), 1);
        int *int_ptr = static_cast<int *>(block);
        allocator::construct(int_ptr, 777);

        std::cout << "Allocated int in alloc1: " << *int_ptr << std::endl;

        try
        {
            std::cout << "\nAttempting to deallocate with wrong allocator..." << std::endl;
            alloc2.deallocate(block);
        }
        catch (std::logic_error const &ex)
        {
            std::cout << "Caught expected exception: " << ex.what() << std::endl;
        }

        try
        {
            std::cout << "\nAttempting to deallocate nullptr..." << std::endl;
            alloc1.deallocate(nullptr);
        }
        catch (std::logic_error const &ex)
        {
            std::cout << "Caught expected exception: " << ex.what() << std::endl;
        }

        allocator::destruct(int_ptr);
        alloc1.deallocate(block);

        try
        {
            std::cout << "\nAttempting to double-free..." << std::endl;
            alloc1.deallocate(block);
        }
        catch (std::logic_error const &ex)
        {
            std::cout << "Caught expected exception: " << ex.what() << std::endl;
        }
    }

    delete log;
}

void test_string_allocation()
{
    std::cout << "\n    String Allocation" << std::endl;

    logger *log = client_logger_builder()
                      .add_console_stream(logger::severity::debug)
                      ->build();

    {
        allocator_boundary_tags alloc(2048, nullptr, log);

        const char *text = "Abbooooooooobaaaa BEEEEEEEEEE";
        size_t text_len = std::strlen(text) + 1;

        void *string_block = alloc.allocate(sizeof(char), text_len);
        char *string_ptr = static_cast<char *>(string_block);

        std::strcpy(string_ptr, text);

        std::cout << "Allocated string: " << string_ptr << std::endl;

        alloc.deallocate(string_block);
    }

    delete log;
}

void test_array_allocation()
{
    std::cout << "\n    Array Allocation" << std::endl;

    logger *log = client_logger_builder()
                      .add_console_stream(logger::severity::debug)
                      ->build();

    {
        allocator_boundary_tags alloc(2048, nullptr, log);

        void *array_block = alloc.allocate(sizeof(int), 20);
        int *array_ptr = static_cast<int *>(array_block);

        for (int i = 0; i < 20; ++i)
        {
            allocator::construct(array_ptr + i, i * 5);
        }

        std::cout << "Array elements: ";
        for (int i = 0; i < 20; ++i)
        {
            std::cout << array_ptr[i] << " ";
        }
        std::cout << std::endl;

        for (int i = 0; i < 20; ++i)
        {
            allocator::destruct(array_ptr + i);
        }

        alloc.deallocate(array_block);
    }

    delete log;
}

void test_set_fit_mode()
{
    std::cout << "\n    Dynamic Fit Mode Change" << std::endl;

    logger *log = client_logger_builder()
                      .add_console_stream(logger::severity::debug)
                      ->build();

    {
        allocator_boundary_tags alloc(2048, nullptr, log, allocator_with_fit_mode::fit_mode::first_fit);

        void *b1 = alloc.allocate(sizeof(int), 10);

        std::cout << "\nChanging to best fit..." << std::endl;
        alloc.set_fit_mode(allocator_with_fit_mode::fit_mode::the_best_fit);

        void *b2 = alloc.allocate(sizeof(int), 10);

        std::cout << "\nChanging to worst fit..." << std::endl;
        alloc.set_fit_mode(allocator_with_fit_mode::fit_mode::the_worst_fit);

        void *b3 = alloc.allocate(sizeof(int), 10);

        alloc.deallocate(b1);
        alloc.deallocate(b2);
        alloc.deallocate(b3);
    }

    delete log;
}

int main()
{
    try
    {
        test_basic_allocation();
        test_multiple_allocations();
        test_struct_allocation();
        test_fit_modes();
        test_coalescing();
        test_exception_handling();
        test_string_allocation();
        test_array_allocation();
        test_set_fit_mode();

        std::cout << "\nAll tests completed" << std::endl;
    }
    catch (std::exception const &ex)
    {
        std::cerr << "Unexpected exception: " << ex.what() << std::endl;

        return 1;
    }

    return 0;
}