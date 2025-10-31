#ifndef PROGRAMMINGSEMINARS_ALLOCATOR_BOUNDARY_TAGS_H
#define PROGRAMMINGSEMINARS_ALLOCATOR_BOUNDARY_TAGS_H

#include <mutex>
#include "allocator.h"
#include "allocator_guardant.h"
#include "allocator_with_fit_mode.h"
#include "logger.h"
#include "logger_guardant.h"

class allocator_boundary_tags final : private allocator_guardant,
                                      private logger_guardant,
                                      public allocator_with_fit_mode
{

private:
    void *_trusted_memory;

public:
    ~allocator_boundary_tags() override;

    allocator_boundary_tags(
        allocator_boundary_tags const &other) = delete;

    allocator_boundary_tags &operator=(
        allocator_boundary_tags const &other) = delete;

    allocator_boundary_tags(
        allocator_boundary_tags &&other) noexcept;

    allocator_boundary_tags &operator=(
        allocator_boundary_tags &&other) noexcept;

public:
    explicit allocator_boundary_tags(
        size_t space_size,
        allocator *parent_allocator = nullptr,
        logger *logger = nullptr,
        allocator_with_fit_mode::fit_mode allocate_fit_mode = allocator_with_fit_mode::fit_mode::first_fit);

public:
    [[nodiscard]] void *allocate(
        size_t value_size,
        size_t values_count) override;

    void deallocate(
        void *at) override;

public:
    inline void set_fit_mode(
        allocator_with_fit_mode::fit_mode mode) override;

private:
    inline allocator *get_allocator() const override;

    inline logger *get_logger() const override;

private:
    inline static constexpr size_t get_metadata_size() noexcept;

    inline size_t &get_memory_size() const;

    inline allocator *&get_parent_allocator() const;

    inline logger *&get_logger_ptr() const;

    inline allocator_with_fit_mode::fit_mode &get_fit_mode() const;

    inline std::mutex *get_sync_object_ptr() const;

    inline std::mutex &get_sync_object() const;

    inline void *&get_first_block_address() const;

private:
    inline static constexpr size_t get_block_metadata_size() noexcept;

    inline size_t &get_block_size(
        void *block) const;

    inline bool &get_block_is_occupied(
        void *block) const;

    inline void *&get_trusted_memory_from_block(
        void *block) const;

    inline void *&get_next_block(
        void *block) const;

    inline void *&get_prev_block(
        void *block) const;

    inline size_t &get_footer_size(
        void *block) const;

    std::string get_block_dump(
        void *at,
        size_t size) const;

    std::string get_memory_state() const;

    size_t get_available_memory() const;

    void *get_user_block_from_block(
        void *block) const;

    void *get_block_from_user_block(
        void *user_block) const;
};

#endif // PROGRAMMINGSEMINARS_ALLOCATOR_BOUNDARY_TAGS_H