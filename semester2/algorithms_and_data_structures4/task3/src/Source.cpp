#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cassert>

#include "allocator_sorted_list.h"
#include "logger.h"
#include "client_logger_builder.h"

int random_int(
    int min, 
    int max) 
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    std::uniform_int_distribution<> distrib(min, max);
    
    return distrib(gen);
}

int main() 
{
    logger* log = client_logger_builder()
        .add_file_stream("logs.log", logger::severity::debug)
        ->add_file_stream("logs.log", logger::severity::warning)
        ->build();

    allocator_sorted_list alloc(1024 * 1024, nullptr, log, allocator_with_fit_mode::fit_mode::first_fit);

    std::vector<std::pair<void*, size_t>> allocated_blocks;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 5000; ++i) 
    {
        bool should_allocate = (random_int(0, 1) == 0);

        if (should_allocate) 
        {
            size_t elements_count = random_int(1, 1024);
            
            try 
            {
                void* block = alloc.allocate(1, elements_count);
                allocated_blocks.emplace_back(block, elements_count);
                
                std::cout << "Allocated block of size " << elements_count << " at " << block << std::endl;
            }
            catch (const std::bad_alloc& e) 
            {
                std::cout << "Allocation failed: " << e.what() << std::endl;
            }
        }
        else if (!allocated_blocks.empty()) 
        {
            int index = random_int(0, allocated_blocks.size() - 1);
            void* block_to_free = allocated_blocks[index].first;
            
            alloc.deallocate(block_to_free);
            
            std::cout << "Deallocated block at " << block_to_free << std::endl;
            
            allocated_blocks.erase(allocated_blocks.begin() + index);
        }
    }

    for (auto& block : allocated_blocks) 
    {
        alloc.deallocate(block.first);
        
        std::cout << "Deallocated remaining block at " << block.first << std::endl;
    }
    
    allocated_blocks.clear();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Benchmark completed in " << duration << " ms" << std::endl;

    return 0;
}