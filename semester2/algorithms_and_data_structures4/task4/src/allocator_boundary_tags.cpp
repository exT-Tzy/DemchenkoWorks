#include "../include/allocator_boundary_tags.h"

#include <sstream>
#include <iomanip>

allocator_boundary_tags::~allocator_boundary_tags()
{
    if (get_logger() != nullptr)
    {
        get_logger()->trace("allocator_boundary_tags::~allocator_boundary_tags() started");
        get_logger()->debug("allocator_boundary_tags::~allocator_boundary_tags() called");
    }

    if (_trusted_memory != nullptr)
    {
        allocator::destruct(get_sync_object_ptr());
        deallocate_with_guard(_trusted_memory);
    }

    if (get_logger() != nullptr)
    {
        get_logger()->debug("allocator_boundary_tags::~allocator_boundary_tags() finished");
        get_logger()->trace("allocator_boundary_tags::~allocator_boundary_tags() finished");
    }
}

allocator_boundary_tags::allocator_boundary_tags(
    allocator_boundary_tags &&other) noexcept : _trusted_memory(other._trusted_memory)
{
    if (get_logger() != nullptr)
    {
        get_logger()->trace("allocator_boundary_tags::allocator_boundary_tags(move) started");
        get_logger()->debug("allocator_boundary_tags::allocator_boundary_tags(move) called");
    }

    other._trusted_memory = nullptr;

    if (get_logger() != nullptr)
    {
        get_logger()->debug("allocator_boundary_tags::allocator_boundary_tags(move) finished");
        get_logger()->trace("allocator_boundary_tags::allocator_boundary_tags(move) finished");
    }
}

allocator_boundary_tags &allocator_boundary_tags::operator=(
    allocator_boundary_tags &&other) noexcept
{
    trace_with_guard("allocator_boundary_tags::operator=(move) started");
    debug_with_guard("allocator_boundary_tags::operator=(move) called");

    if (this != &other)
    {
        if (_trusted_memory != nullptr)
        {
            allocator::destruct(get_sync_object_ptr());
            deallocate_with_guard(_trusted_memory);
        }

        _trusted_memory = other._trusted_memory;
        other._trusted_memory = nullptr;
    }

    debug_with_guard("allocator_boundary_tags::operator=(move) finished");
    trace_with_guard("allocator_boundary_tags::operator=(move) finished");

    return *this;
}

allocator_boundary_tags::allocator_boundary_tags(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if (logger != nullptr)
    {
        logger->trace("allocator_boundary_tags::allocator_boundary_tags(size_t, allocator*, logger*, fit_mode) started");
        logger->debug("allocator_boundary_tags::allocator_boundary_tags(size_t, allocator*, logger*, fit_mode) called");
    }

    auto const target_size = get_metadata_size() + space_size;

    try
    {
        _trusted_memory = parent_allocator == nullptr
                              ? ::operator new(target_size)
                              : parent_allocator->allocate(target_size, 1);
    }
    catch (std::bad_alloc const &ex)
    {
        if (logger != nullptr)
        {
            std::ostringstream oss;
            oss << "Failed to allocate trusted memory of size " << target_size << ": " << ex.what();
            logger->error(oss.str());
        }

        throw;
    }

    get_memory_size() = space_size;
    get_parent_allocator() = parent_allocator;
    get_logger_ptr() = logger;
    get_fit_mode() = allocate_fit_mode;

    allocator::construct(get_sync_object_ptr());

    void *first_block = reinterpret_cast<unsigned char *>(_trusted_memory) + get_metadata_size();
    get_first_block_address() = first_block;

    size_t first_block_size = space_size - get_block_metadata_size();

    get_block_size(first_block) = first_block_size;
    get_block_is_occupied(first_block) = false;
    get_trusted_memory_from_block(first_block) = _trusted_memory;
    get_next_block(first_block) = nullptr;
    get_prev_block(first_block) = nullptr;

    get_footer_size(first_block) = first_block_size;

    if (logger != nullptr)
    {
        logger->debug("allocator_boundary_tags::allocator_boundary_tags(size_t, allocator*, logger*, fit_mode) finished");
        logger->trace("allocator_boundary_tags::allocator_boundary_tags(size_t, allocator*, logger*, fit_mode) finished");
    }
}

[[nodiscard]] void *allocator_boundary_tags::allocate(
    size_t value_size,
    size_t values_count)
{
    trace_with_guard("allocator_boundary_tags::allocate() started");
    debug_with_guard("allocator_boundary_tags::allocate() called");

    std::lock_guard<std::mutex> guard(get_sync_object());

    size_t requested_size = value_size * values_count;

    std::ostringstream req_oss;
    req_oss << "Requested allocation: " << requested_size << " bytes";
    trace_with_guard(req_oss.str());

    void *target_block = nullptr;
    void *current_block = get_first_block_address();
    auto fit_mode = get_fit_mode();

    while (current_block != nullptr)
    {
        if (!get_block_is_occupied(current_block) && get_block_size(current_block) >= requested_size)
        {
            if (fit_mode == allocator_with_fit_mode::fit_mode::first_fit)
            {
                target_block = current_block;
                break;
            }
            else if (fit_mode == allocator_with_fit_mode::fit_mode::the_best_fit)
            {
                if (target_block == nullptr || get_block_size(current_block) < get_block_size(target_block))
                {
                    target_block = current_block;
                }
            }
            else if (fit_mode == allocator_with_fit_mode::fit_mode::the_worst_fit)
            {
                if (target_block == nullptr || get_block_size(current_block) > get_block_size(target_block))
                {
                    target_block = current_block;
                }
            }
        }

        current_block = get_next_block(current_block);
    }

    if (target_block == nullptr)
    {
        error_with_guard("No suitable block found for allocation");
        debug_with_guard("allocator_boundary_tags::allocate() finished with exception");
        trace_with_guard("allocator_boundary_tags::allocate() finished with exception");

        throw std::bad_alloc();
    }

    size_t block_size = get_block_size(target_block);
    size_t remaining_size = block_size - requested_size;

    if (remaining_size >= get_block_metadata_size())
    {
        get_block_size(target_block) = requested_size;
        get_footer_size(target_block) = requested_size;

        void *new_block = reinterpret_cast<unsigned char *>(target_block) + get_block_metadata_size() + requested_size;

        size_t new_block_size = remaining_size - get_block_metadata_size();

        get_block_size(new_block) = new_block_size;
        get_block_is_occupied(new_block) = false;
        get_trusted_memory_from_block(new_block) = _trusted_memory;
        get_next_block(new_block) = get_next_block(target_block);
        get_prev_block(new_block) = target_block;
        get_footer_size(new_block) = new_block_size;

        if (get_next_block(target_block) != nullptr)
        {
            get_prev_block(get_next_block(target_block)) = new_block;
        }

        get_next_block(target_block) = new_block;

        std::ostringstream split_oss;
        split_oss << "Block split: allocated " << requested_size << " bytes, remaining " << new_block_size << " bytes";
        warning_with_guard(split_oss.str());
    }
    else
    {
        if (remaining_size > 0)
        {
            std::ostringstream adjust_oss;
            adjust_oss << "Request adjusted from " << requested_size << " to " << block_size << " bytes (internal fragmentation)";
            warning_with_guard(adjust_oss.str());
        }
    }

    get_block_is_occupied(target_block) = true;

    void *user_block = get_user_block_from_block(target_block);

    std::ostringstream alloc_oss;
    alloc_oss << "Allocated block at " << user_block << " (size: " << get_block_size(target_block) << " bytes)";
    trace_with_guard(alloc_oss.str());

    std::ostringstream avail_oss;
    avail_oss << "Available memory after allocation: " << get_available_memory() << " bytes";
    information_with_guard(avail_oss.str());

    std::string state = get_memory_state();
    debug_with_guard("Memory state after allocation: " + state);

    debug_with_guard("allocator_boundary_tags::allocate() finished");
    trace_with_guard("allocator_boundary_tags::allocate() finished");

    return user_block;
}

void allocator_boundary_tags::deallocate(
    void *at)
{
    trace_with_guard("allocator_boundary_tags::deallocate() started");
    debug_with_guard("allocator_boundary_tags::deallocate() called");

    std::lock_guard<std::mutex> guard(get_sync_object());

    if (at == nullptr)
    {
        error_with_guard("Attempted to deallocate nullptr");
        debug_with_guard("allocator_boundary_tags::deallocate() finished with exception");
        trace_with_guard("allocator_boundary_tags::deallocate() finished with exception");

        throw std::logic_error("Cannot deallocate nullptr");
    }

    void *block = get_block_from_user_block(at);

    void *left_bound = get_first_block_address();
    void *right_bound = reinterpret_cast<unsigned char *>(_trusted_memory) + get_metadata_size() + get_memory_size();

    if (block < left_bound || block >= right_bound)
    {
        error_with_guard("Block address out of allocator bounds");
        debug_with_guard("allocator_boundary_tags::deallocate() finished with exception");
        trace_with_guard("allocator_boundary_tags::deallocate() finished with exception");

        throw std::logic_error("Block address out of allocator bounds");
    }

    if (get_trusted_memory_from_block(block) != _trusted_memory)
    {
        error_with_guard("Block does not belong to this allocator instance");
        debug_with_guard("allocator_boundary_tags::deallocate() finished with exception");
        trace_with_guard("allocator_boundary_tags::deallocate() finished with exception");

        throw std::logic_error("Block does not belong to this allocator instance");
    }

    if (!get_block_is_occupied(block))
    {
        error_with_guard("Attempted to deallocate already free block");
        debug_with_guard("allocator_boundary_tags::deallocate() finished with exception");
        trace_with_guard("allocator_boundary_tags::deallocate() finished with exception");

        throw std::logic_error("Block is already free");
    }

    size_t block_size = get_block_size(block);

    if (block_size > 0 && block_size <= 4096)
    {
        std::string dump = get_block_dump(at, block_size);
        std::ostringstream dump_oss;
        dump_oss << "Block state before deallocation (size: " << block_size << " bytes): " << dump;
        debug_with_guard(dump_oss.str());
    }

    get_block_is_occupied(block) = false;

    std::ostringstream dealloc_oss;
    dealloc_oss << "Deallocating block at " << at << " (size: " << block_size << " bytes)";
    trace_with_guard(dealloc_oss.str());

    void *prev = get_prev_block(block);
    void *next = get_next_block(block);

    if (next != nullptr && !get_block_is_occupied(next))
    {
        size_t next_size = get_block_size(next);

        get_block_size(block) += get_block_metadata_size() + next_size;
        get_footer_size(block) = get_block_size(block);

        get_next_block(block) = get_next_block(next);

        if (get_next_block(next) != nullptr)
        {
            get_prev_block(get_next_block(next)) = block;
        }

        std::ostringstream merge_oss;
        merge_oss << "Merged with next block (gained " << (get_block_metadata_size() + next_size) << " bytes)";
        trace_with_guard(merge_oss.str());
    }

    if (prev != nullptr && !get_block_is_occupied(prev))
    {
        size_t current_size = get_block_size(block);

        get_block_size(prev) += get_block_metadata_size() + current_size;
        get_footer_size(prev) = get_block_size(prev);

        get_next_block(prev) = get_next_block(block);

        if (get_next_block(block) != nullptr)
        {
            get_prev_block(get_next_block(block)) = prev;
        }

        std::ostringstream merge_oss;
        merge_oss << "Merged with previous block (gained " << (get_block_metadata_size() + current_size) << " bytes)";
        trace_with_guard(merge_oss.str());
    }

    std::ostringstream avail_oss;
    avail_oss << "Available memory after deallocation: " << get_available_memory() << " bytes";
    information_with_guard(avail_oss.str());

    std::string state = get_memory_state();
    debug_with_guard("Memory state after deallocation: " + state);

    debug_with_guard("allocator_boundary_tags::deallocate() finished");
    trace_with_guard("allocator_boundary_tags::deallocate() finished");
}

inline void allocator_boundary_tags::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    trace_with_guard("allocator_boundary_tags::set_fit_mode() started");
    debug_with_guard("allocator_boundary_tags::set_fit_mode() called");

    get_fit_mode() = mode;

    debug_with_guard("allocator_boundary_tags::set_fit_mode() finished");
    trace_with_guard("allocator_boundary_tags::set_fit_mode() finished");
}

inline allocator *allocator_boundary_tags::get_allocator() const
{
    return get_parent_allocator();
}

inline logger *allocator_boundary_tags::get_logger() const
{
    return get_logger_ptr();
}

inline constexpr size_t allocator_boundary_tags::get_metadata_size() noexcept
{
    return sizeof(size_t) + sizeof(allocator *) + sizeof(logger *) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(std::mutex) + sizeof(void *);
}

inline size_t &allocator_boundary_tags::get_memory_size() const
{
    return *reinterpret_cast<size_t *>(_trusted_memory);
}

inline allocator *&allocator_boundary_tags::get_parent_allocator() const
{
    return *reinterpret_cast<allocator **>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(size_t));
}

inline logger *&allocator_boundary_tags::get_logger_ptr() const
{
    return *reinterpret_cast<logger **>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(size_t) + sizeof(allocator *));
}

inline allocator_with_fit_mode::fit_mode &allocator_boundary_tags::get_fit_mode() const
{
    return *reinterpret_cast<allocator_with_fit_mode::fit_mode *>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(size_t) + sizeof(allocator *) + sizeof(logger *));
}

inline std::mutex *allocator_boundary_tags::get_sync_object_ptr() const
{
    return reinterpret_cast<std::mutex *>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(size_t) + sizeof(allocator *) + sizeof(logger *) + sizeof(allocator_with_fit_mode::fit_mode));
}

inline std::mutex &allocator_boundary_tags::get_sync_object() const
{
    return *get_sync_object_ptr();
}

inline void *&allocator_boundary_tags::get_first_block_address() const
{
    return *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(get_sync_object_ptr()) + sizeof(std::mutex));
}

inline constexpr size_t allocator_boundary_tags::get_block_metadata_size() noexcept
{
    return sizeof(size_t) + sizeof(bool) + sizeof(void *) + sizeof(void *) + sizeof(void *) + sizeof(size_t);
}

inline size_t &allocator_boundary_tags::get_block_size(
    void *block) const
{
    return *reinterpret_cast<size_t *>(block);
}

inline bool &allocator_boundary_tags::get_block_is_occupied(
    void *block) const
{
    return *reinterpret_cast<bool *>(reinterpret_cast<unsigned char *>(block) + sizeof(size_t));
}

inline void *&allocator_boundary_tags::get_trusted_memory_from_block(
    void *block) const
{
    return *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(block) + sizeof(size_t) + sizeof(bool));
}

inline void *&allocator_boundary_tags::get_next_block(
    void *block) const
{
    return *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(block) + sizeof(size_t) + sizeof(bool) + sizeof(void *));
}

inline void *&allocator_boundary_tags::get_prev_block(
    void *block) const
{
    return *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(block) + sizeof(size_t) + sizeof(bool) + sizeof(void *) + sizeof(void *));
}

inline size_t &allocator_boundary_tags::get_footer_size(
    void *block) const
{
    size_t block_size = get_block_size(block);
    return *reinterpret_cast<size_t *>(reinterpret_cast<unsigned char *>(block) + sizeof(size_t) + sizeof(bool) + sizeof(void *) + sizeof(void *) + sizeof(void *) + block_size);
}

std::string allocator_boundary_tags::get_block_dump(
    void *at,
    size_t size) const
{
    std::ostringstream oss;
    unsigned char *bytes = reinterpret_cast<unsigned char *>(at);

    size_t display_size = size > 256 ? 256 : size;

    oss << std::hex << std::setfill('0');

    for (size_t i = 0; i < display_size; ++i)
    {
        if (i > 0)
        {
            oss << " ";
        }
        oss << std::setw(2) << static_cast<int>(bytes[i]);
    }

    if (size > 256)
    {
        oss << " ... (" << (size - 256) << " more bytes)";
    }

    return oss.str();
}

std::string allocator_boundary_tags::get_memory_state() const
{
    std::ostringstream oss;

    void *current = get_first_block_address();
    bool first = true;

    while (current != nullptr)
    {
        if (!first)
        {
            oss << " | ";
        }
        first = false;

        if (get_block_is_occupied(current))
        {
            oss << "occup";
        }
        else
        {
            oss << "avail";
        }

        oss << " " << get_block_size(current);

        current = get_next_block(current);
    }

    return oss.str();
}

size_t allocator_boundary_tags::get_available_memory() const
{
    size_t available = 0;

    void *current = get_first_block_address();

    while (current != nullptr)
    {
        if (!get_block_is_occupied(current))
        {
            available += get_block_size(current);
        }

        current = get_next_block(current);
    }

    return available;
}

void *allocator_boundary_tags::get_user_block_from_block(
    void *block) const
{
    return reinterpret_cast<unsigned char *>(block) + sizeof(size_t) + sizeof(bool) + sizeof(void *) + sizeof(void *) + sizeof(void *);
}

void *allocator_boundary_tags::get_block_from_user_block(
    void *user_block) const
{
    return reinterpret_cast<unsigned char *>(user_block) - sizeof(size_t) - sizeof(bool) - sizeof(void *) - sizeof(void *) - sizeof(void *);
}