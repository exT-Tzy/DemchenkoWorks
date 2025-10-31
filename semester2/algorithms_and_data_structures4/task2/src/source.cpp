#include <iostream>
#include <string>
#include <vector>

#include "../include/allocator_global_heap.h"
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
                      .add_console_stream(logger::severity::trace)
                      ->add_console_stream(logger::severity::debug)
                      ->add_console_stream(logger::severity::warning)
                      ->add_console_stream(logger::severity::error)
                      ->add_file_stream("allocator_test.log", logger::severity::trace)
                      ->build();

    {
        allocator_global_heap alloc(log);

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
                      ->add_console_stream(logger::severity::error)
                      ->build();

    {
        allocator_global_heap alloc(log);

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
        allocator_global_heap alloc(log);

        void *struct_block = alloc.allocate(sizeof(test_struct), 1);
        test_struct *struct_ptr = static_cast<test_struct *>(struct_block);
        allocator::construct(struct_ptr, 100, 99.99, 'A');

        std::cout << "Struct values: " << struct_ptr->a << ", " << struct_ptr->b << ", " << struct_ptr->c << std::endl;

        allocator::destruct(struct_ptr);
        alloc.deallocate(struct_block);
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
        allocator_global_heap alloc(log);

        void *array_block = alloc.allocate(sizeof(int), 10);
        int *array_ptr = static_cast<int *>(array_block);

        for (int i = 0; i < 10; ++i)
        {
            allocator::construct(array_ptr + i, i * 10);
            std::cout << "Array[" << i << "] = " << array_ptr[i] << std::endl;
        }

        for (int i = 0; i < 10; ++i)
        {
            allocator::destruct(array_ptr + i);
        }

        alloc.deallocate(array_block);
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
        allocator_global_heap alloc1(log);
        allocator_global_heap alloc2(log);

        void *block = alloc1.allocate(sizeof(int), 1);
        int *int_ptr = static_cast<int *>(block);
        allocator::construct(int_ptr, 777);

        std::cout << "Allocated int in alloc1: " << *int_ptr << std::endl;

        try
        {
            std::cout << "Attempting to deallocate with wrong allocator..." << std::endl;
            alloc2.deallocate(block);
        }
        catch (std::logic_error const &ex)
        {
            std::cout << "Caught expected exception: " << ex.what() << std::endl;
        }

        try
        {
            std::cout << "Attempting to deallocate nullptr..." << std::endl;
            alloc1.deallocate(nullptr);
        }
        catch (std::logic_error const &ex)
        {
            std::cout << "Caught expected exception: " << ex.what() << std::endl;
        }

        allocator::destruct(int_ptr);
        alloc1.deallocate(block);
    }

    delete log;
}

void test_large_allocation()
{
    std::cout << "\n    Large Allocation" << std::endl;

    logger *log = client_logger_builder()
                      .add_console_stream(logger::severity::debug)
                      ->build();

    {
        allocator_global_heap alloc(log);

        void *large_block = alloc.allocate(1, 1024 * 1024);
        unsigned char *bytes = static_cast<unsigned char *>(large_block);

        for (int i = 0; i < 100; ++i)
        {
            bytes[i] = static_cast<unsigned char>(i % 256);
        }

        std::cout << "Large block allocated and initialized" << std::endl;

        alloc.deallocate(large_block);
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
        allocator_global_heap alloc(log);

        const char *text = "Hello, World!";
        size_t text_len = std::strlen(text) + 1;

        void *string_block = alloc.allocate(sizeof(char), text_len);
        char *string_ptr = static_cast<char *>(string_block);

        std::strcpy(string_ptr, text);

        std::cout << "Allocated string: " << string_ptr << std::endl;

        alloc.deallocate(string_block);
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
        test_array_allocation();
        test_exception_handling();
        test_large_allocation();
        test_string_allocation();

        std::cout << "\nAll tests completed" << std::endl;
    }
    catch (std::exception const &ex)
    {
        std::cerr << "Unexpected exception: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}