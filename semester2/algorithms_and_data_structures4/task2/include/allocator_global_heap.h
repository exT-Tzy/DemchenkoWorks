#ifndef PROGRAMMINGSEMINARS_ALLOCATOR_GLOBAL_HEAP_H
#define PROGRAMMINGSEMINARS_ALLOCATOR_GLOBAL_HEAP_H

#include "allocator.h"
#include "allocator_guardant.h"
#include "logger.h"
#include "logger_guardant.h"

class allocator_global_heap final : private allocator_guardant,
                                    private logger_guardant,
                                    public allocator
{

private:
    logger *_logger;

public:
    ~allocator_global_heap() override;

    allocator_global_heap(
        allocator_global_heap const &other) = delete;

    allocator_global_heap &operator=(
        allocator_global_heap const &other) = delete;

    allocator_global_heap(
        allocator_global_heap &&other) noexcept;

    allocator_global_heap &operator=(
        allocator_global_heap &&other) noexcept;

public:
    explicit allocator_global_heap(
        logger *logger = nullptr);

public:
    [[nodiscard]] void *allocate(
        size_t value_size,
        size_t values_count) override;

    void deallocate(
        void *at) override;

private:
    inline allocator *get_allocator() const override;

    inline logger *get_logger() const override;

private:
    inline static constexpr size_t get_block_metadata_size() noexcept;

    inline allocator *&get_allocator_from_block(
        void *block) const;

    inline size_t &get_size_from_block(
        void *block) const;

    std::string get_block_dump(
        void *at,
        size_t size) const;
};

#endif // PROGRAMMINGSEMINARS_ALLOCATOR_GLOBAL_HEAP_H