#include "../include/allocator_buddies_system.h"

#include <sstream>
#include <iomanip>
#include <cmath>
#include <vector>
// #include <algorithm>

allocator_buddies_system::~allocator_buddies_system()
{
    if (get_logger() != nullptr)
    {
        get_logger()->trace("allocator_buddies_system::~allocator_buddies_system() started");
        get_logger()->debug("allocator_buddies_system::~allocator_buddies_system() called");
    }

    if (_trusted_memory != nullptr)
    {
        allocator::destruct(get_sync_object_ptr());
        deallocate_with_guard(_trusted_memory);
    }

    if (get_logger() != nullptr)
    {
        get_logger()->debug("allocator_buddies_system::~allocator_buddies_system() finished");
        get_logger()->trace("allocator_buddies_system::~allocator_buddies_system() finished");
    }
}

allocator_buddies_system::allocator_buddies_system(
    allocator_buddies_system &&other) noexcept : _trusted_memory(other._trusted_memory)
{
    if (get_logger() != nullptr)
    {
        get_logger()->trace("allocator_buddies_system::allocator_buddies_system(move) started");
        get_logger()->debug("allocator_buddies_system::allocator_buddies_system(move) called");
    }

    other._trusted_memory = nullptr;

    if (get_logger() != nullptr)
    {
        get_logger()->debug("allocator_buddies_system::allocator_buddies_system(move) finished");
        get_logger()->trace("allocator_buddies_system::allocator_buddies_system(move) finished");
    }
}

allocator_buddies_system &allocator_buddies_system::operator=(
    allocator_buddies_system &&other) noexcept
{
    trace_with_guard("allocator_buddies_system::operator=(move) started");
    debug_with_guard("allocator_buddies_system::operator=(move) called");

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

    debug_with_guard("allocator_buddies_system::operator=(move) finished");
    trace_with_guard("allocator_buddies_system::operator=(move) finished");

    return *this;
}

allocator_buddies_system::allocator_buddies_system(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if (logger != nullptr)
    {
        logger->trace("allocator_buddies_system::allocator_buddies_system(size_t, allocator*, logger*, fit_mode) started");
        logger->debug("allocator_buddies_system::allocator_buddies_system(size_t, allocator*, logger*, fit_mode) called");
    }

    size_t min_power = static_cast<size_t>(std::ceil(std::log2(get_block_metadata_size() + 1)));
    size_t max_power = static_cast<size_t>(std::floor(std::log2(space_size)));

    if (power_of_two(min_power) > space_size)
    {
        if (logger != nullptr)
        {
            logger->error("Space size is too small for buddy system allocator");
        }
        throw std::bad_alloc();
    }

    size_t free_lists_count = max_power - min_power + 1;
    size_t metadata_total_size = get_metadata_size() + free_lists_count * sizeof(void *);

    auto const target_size = metadata_total_size + space_size;

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
    get_min_block_power() = min_power;
    get_max_block_power() = max_power;

    allocator::construct(get_sync_object_ptr());

    void **free_lists = get_free_lists_start();
    for (size_t i = 0; i < free_lists_count; ++i)
    {
        free_lists[i] = nullptr;
    }

    void *first_block = reinterpret_cast<unsigned char *>(_trusted_memory) + metadata_total_size;

    get_block_power(first_block) = max_power;
    get_block_is_occupied(first_block) = false;
    get_trusted_memory_from_block(first_block) = _trusted_memory;
    get_next_block(first_block) = nullptr;

    free_lists[max_power - min_power] = first_block;

    if (logger != nullptr)
    {
        std::ostringstream oss;
        oss << "Initialized buddy system: min_power=" << min_power << ", max_power=" << max_power << ", space=" << space_size;
        logger->trace(oss.str());
        logger->debug("allocator_buddies_system::allocator_buddies_system(size_t, allocator*, logger*, fit_mode) finished");
        logger->trace("allocator_buddies_system::allocator_buddies_system(size_t, allocator*, logger*, fit_mode) finished");
    }
}

[[nodiscard]] void *allocator_buddies_system::allocate(
    size_t value_size,
    size_t values_count)
{
    trace_with_guard("allocator_buddies_system::allocate() started");
    debug_with_guard("allocator_buddies_system::allocate() called");

    std::lock_guard<std::mutex> guard(get_sync_object());

    size_t requested_size = value_size * values_count;

    std::ostringstream req_oss;
    req_oss << "Requested allocation: " << requested_size << " bytes";
    trace_with_guard(req_oss.str());

    size_t required_power = get_required_power(requested_size);

    if (required_power > get_max_block_power())
    {
        error_with_guard("Requested size exceeds maximum block size");
        debug_with_guard("allocator_buddies_system::allocate() finished with exception");
        trace_with_guard("allocator_buddies_system::allocate() finished with exception");

        throw std::bad_alloc();
    }

    if (power_of_two(required_power) > requested_size)
    {
        std::ostringstream adjust_oss;
        adjust_oss << "Request adjusted from " << requested_size << " to " << power_of_two(required_power) << " bytes (power of 2 alignment)";
        warning_with_guard(adjust_oss.str());
    }

    void *block = find_free_block(required_power);

    if (block == nullptr)
    {
        error_with_guard("No suitable block found for allocation");
        debug_with_guard("allocator_buddies_system::allocate() finished with exception");
        trace_with_guard("allocator_buddies_system::allocate() finished with exception");

        throw std::bad_alloc();
    }

    get_block_is_occupied(block) = true;

    void *user_block = get_user_block_from_block(block);

    std::ostringstream alloc_oss;
    alloc_oss << "Allocated block at " << user_block << " (power: " << get_block_power(block) << ", size: " << power_of_two(get_block_power(block)) << " bytes)";
    trace_with_guard(alloc_oss.str());

    std::ostringstream avail_oss;
    avail_oss << "Available memory after allocation: " << get_available_memory() << " bytes";
    information_with_guard(avail_oss.str());

    std::string state = get_memory_state();
    debug_with_guard("Memory state after allocation: " + state);

    debug_with_guard("allocator_buddies_system::allocate() finished");
    trace_with_guard("allocator_buddies_system::allocate() finished");

    return user_block;
}

void allocator_buddies_system::deallocate(
    void *at)
{
    trace_with_guard("allocator_buddies_system::deallocate() started");
    debug_with_guard("allocator_buddies_system::deallocate() called");

    std::lock_guard<std::mutex> guard(get_sync_object());

    if (at == nullptr)
    {
        error_with_guard("Attempted to deallocate nullptr");
        debug_with_guard("allocator_buddies_system::deallocate() finished with exception");
        trace_with_guard("allocator_buddies_system::deallocate() finished with exception");

        throw std::logic_error("Cannot deallocate nullptr");
    }

    void *block = get_block_from_user_block(at);

    size_t metadata_total_size = get_metadata_size() + (get_max_block_power() - get_min_block_power() + 1) * sizeof(void *);
    void *left_bound = reinterpret_cast<unsigned char *>(_trusted_memory) + metadata_total_size;
    void *right_bound = reinterpret_cast<unsigned char *>(_trusted_memory) + metadata_total_size + get_memory_size();

    if (block < left_bound || block >= right_bound)
    {
        error_with_guard("Block address out of allocator bounds");
        debug_with_guard("allocator_buddies_system::deallocate() finished with exception");
        trace_with_guard("allocator_buddies_system::deallocate() finished with exception");

        throw std::logic_error("Block address out of allocator bounds");
    }

    if (get_trusted_memory_from_block(block) != _trusted_memory)
    {
        error_with_guard("Block does not belong to this allocator instance");
        debug_with_guard("allocator_buddies_system::deallocate() finished with exception");
        trace_with_guard("allocator_buddies_system::deallocate() finished with exception");

        throw std::logic_error("Block does not belong to this allocator instance");
    }

    if (!get_block_is_occupied(block))
    {
        error_with_guard("Attempted to deallocate already free block");
        debug_with_guard("allocator_buddies_system::deallocate() finished with exception");
        trace_with_guard("allocator_buddies_system::deallocate() finished with exception");

        throw std::logic_error("Block is already free");
    }

    size_t block_power = get_block_power(block);
    size_t block_size = power_of_two(block_power);

    if (block_size > 0 && block_size <= 4096)
    {
        std::string dump = get_block_dump(at, block_size);
        std::ostringstream dump_oss;
        dump_oss << "Block state before deallocation (power: " << block_power << ", size: " << block_size << " bytes): " << dump;
        debug_with_guard(dump_oss.str());
    }

    get_block_is_occupied(block) = false;

    std::ostringstream dealloc_oss;
    dealloc_oss << "Deallocating block at " << at << " (power: " << block_power << ", size: " << block_size << " bytes)";
    trace_with_guard(dealloc_oss.str());

    while (block_power < get_max_block_power())
    {
        void *buddy = get_buddy_address(block, block_power);

        if (buddy < left_bound || buddy >= right_bound)
        {
            break;
        }

        if (get_trusted_memory_from_block(buddy) != _trusted_memory ||
            get_block_is_occupied(buddy) ||
            get_block_power(buddy) != block_power)
        {
            break;
        }

        remove_block_from_free_list(buddy, block_power);

        if (buddy < block)
        {
            block = buddy;
        }

        ++block_power;
        get_block_power(block) = block_power;

        std::ostringstream merge_oss;
        merge_oss << "Merged with buddy (new power: " << block_power << ", size: " << power_of_two(block_power) << " bytes)";
        trace_with_guard(merge_oss.str());
    }

    add_block_to_free_list(block, block_power);

    std::ostringstream avail_oss;
    avail_oss << "Available memory after deallocation: " << get_available_memory() << " bytes";
    information_with_guard(avail_oss.str());

    std::string state = get_memory_state();
    debug_with_guard("Memory state after deallocation: " + state);

    debug_with_guard("allocator_buddies_system::deallocate() finished");
    trace_with_guard("allocator_buddies_system::deallocate() finished");
}

inline void allocator_buddies_system::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    trace_with_guard("allocator_buddies_system::set_fit_mode() started");
    debug_with_guard("allocator_buddies_system::set_fit_mode() called");

    get_fit_mode() = mode;

    debug_with_guard("allocator_buddies_system::set_fit_mode() finished");
    trace_with_guard("allocator_buddies_system::set_fit_mode() finished");
}

inline allocator *allocator_buddies_system::get_allocator() const
{
    return get_parent_allocator();
}

inline logger *allocator_buddies_system::get_logger() const
{
    return get_logger_ptr();
}

inline constexpr size_t allocator_buddies_system::get_metadata_size() noexcept
{
    return sizeof(size_t) + sizeof(allocator *) + sizeof(logger *) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(std::mutex) + sizeof(size_t) + sizeof(size_t);
}

inline size_t &allocator_buddies_system::get_memory_size() const
{
    return *reinterpret_cast<size_t *>(_trusted_memory);
}

inline allocator *&allocator_buddies_system::get_parent_allocator() const
{
    return *reinterpret_cast<allocator **>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(size_t));
}

inline logger *&allocator_buddies_system::get_logger_ptr() const
{
    return *reinterpret_cast<logger **>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(size_t) + sizeof(allocator *));
}

inline allocator_with_fit_mode::fit_mode &allocator_buddies_system::get_fit_mode() const
{
    return *reinterpret_cast<allocator_with_fit_mode::fit_mode *>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(size_t) + sizeof(allocator *) + sizeof(logger *));
}

inline std::mutex *allocator_buddies_system::get_sync_object_ptr() const
{
    return reinterpret_cast<std::mutex *>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(size_t) + sizeof(allocator *) + sizeof(logger *) + sizeof(allocator_with_fit_mode::fit_mode));
}

inline std::mutex &allocator_buddies_system::get_sync_object() const
{
    return *get_sync_object_ptr();
}

inline size_t &allocator_buddies_system::get_min_block_power() const
{
    return *reinterpret_cast<size_t *>(reinterpret_cast<unsigned char *>(get_sync_object_ptr()) + sizeof(std::mutex));
}

inline size_t &allocator_buddies_system::get_max_block_power() const
{
    return *reinterpret_cast<size_t *>(reinterpret_cast<unsigned char *>(get_sync_object_ptr()) + sizeof(std::mutex) + sizeof(size_t));
}

inline void **allocator_buddies_system::get_free_lists_start() const
{
    return reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(get_sync_object_ptr()) + sizeof(std::mutex) + sizeof(size_t) + sizeof(size_t));
}

inline constexpr size_t allocator_buddies_system::get_block_metadata_size() noexcept
{
    return sizeof(size_t) + sizeof(bool) + sizeof(void *) + sizeof(void *);
}

inline size_t &allocator_buddies_system::get_block_power(
    void *block) const
{
    return *reinterpret_cast<size_t *>(block);
}

inline bool &allocator_buddies_system::get_block_is_occupied(
    void *block) const
{
    return *reinterpret_cast<bool *>(reinterpret_cast<unsigned char *>(block) + sizeof(size_t));
}

inline void *&allocator_buddies_system::get_trusted_memory_from_block(
    void *block) const
{
    return *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(block) + sizeof(size_t) + sizeof(bool));
}

inline void *&allocator_buddies_system::get_next_block(
    void *block) const
{
    return *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(block) + sizeof(size_t) + sizeof(bool) + sizeof(void *));
}

void *allocator_buddies_system::get_buddy_address(
    void *block,
    size_t power) const
{
    size_t metadata_total_size = get_metadata_size() + (get_max_block_power() - get_min_block_power() + 1) * sizeof(void *);
    void *memory_start = reinterpret_cast<unsigned char *>(_trusted_memory) + metadata_total_size;

    size_t offset = reinterpret_cast<unsigned char *>(block) - reinterpret_cast<unsigned char *>(memory_start);
    size_t block_size = power_of_two(power);

    size_t buddy_offset = offset ^ block_size;

    return reinterpret_cast<unsigned char *>(memory_start) + buddy_offset;
}

size_t allocator_buddies_system::get_required_power(
    size_t size) const
{
    size_t total_size = size + get_block_metadata_size();
    size_t power = static_cast<size_t>(std::ceil(std::log2(total_size)));

    if (power < get_min_block_power())
    {
        power = get_min_block_power();
    }

    return power;
}

void *allocator_buddies_system::split_block(
    void *block,
    size_t current_power,
    size_t target_power)
{
    trace_with_guard("allocator_buddies_system::split_block() started");

    while (current_power > target_power)
    {
        --current_power;

        size_t block_size = power_of_two(current_power);

        void *buddy = reinterpret_cast<unsigned char *>(block) + block_size;

        get_block_power(buddy) = current_power;
        get_block_is_occupied(buddy) = false;
        get_trusted_memory_from_block(buddy) = _trusted_memory;

        add_block_to_free_list(buddy, current_power);

        get_block_power(block) = current_power;

        std::ostringstream split_oss;
        split_oss << "Split block to power " << current_power << " (size: " << block_size << " bytes)";
        trace_with_guard(split_oss.str());
    }

    trace_with_guard("allocator_buddies_system::split_block() finished");

    return block;
}

void *allocator_buddies_system::find_free_block(
    size_t power)
{
    trace_with_guard("allocator_buddies_system::find_free_block() started");

    auto fit_mode = get_fit_mode();
    void **free_lists = get_free_lists_start();
    size_t min_power = get_min_block_power();
    size_t max_power = get_max_block_power();

    void *selected_block = nullptr;
    size_t selected_power = 0;

    for (size_t p = power; p <= max_power; ++p)
    {
        void *current = free_lists[p - min_power];

        if (current == nullptr)
        {
            continue;
        }

        if (fit_mode == allocator_with_fit_mode::fit_mode::first_fit)
        {
            selected_block = current;
            selected_power = p;
            break;
        }
        else if (fit_mode == allocator_with_fit_mode::fit_mode::the_best_fit)
        {
            if (selected_block == nullptr)
            {
                selected_block = current;
                selected_power = p;
            }

            break;
        }
        else if (fit_mode == allocator_with_fit_mode::fit_mode::the_worst_fit)
        {
            selected_block = current;
            selected_power = p;
        }
    }

    if (selected_block == nullptr)
    {
        trace_with_guard("allocator_buddies_system::find_free_block() finished (not found)");
        return nullptr;
    }

    remove_block_from_free_list(selected_block, selected_power);

    if (selected_power > power)
    {
        selected_block = split_block(selected_block, selected_power, power);
    }

    trace_with_guard("allocator_buddies_system::find_free_block() finished");

    return selected_block;
}

void allocator_buddies_system::add_block_to_free_list(
    void *block,
    size_t power)
{
    trace_with_guard("allocator_buddies_system::add_block_to_free_list() started");

    void **free_lists = get_free_lists_start();
    size_t min_power = get_min_block_power();

    get_next_block(block) = free_lists[power - min_power];
    free_lists[power - min_power] = block;

    trace_with_guard("allocator_buddies_system::add_block_to_free_list() finished");
}

void allocator_buddies_system::remove_block_from_free_list(
    void *block,
    size_t power)
{
    trace_with_guard("allocator_buddies_system::remove_block_from_free_list() started");

    void **free_lists = get_free_lists_start();
    size_t min_power = get_min_block_power();

    void **list_head = &free_lists[power - min_power];

    if (*list_head == block)
    {
        *list_head = get_next_block(block);
    }
    else
    {
        void *current = *list_head;
        while (current != nullptr)
        {
            if (get_next_block(current) == block)
            {
                get_next_block(current) = get_next_block(block);
                break;
            }
            current = get_next_block(current);
        }
    }

    trace_with_guard("allocator_buddies_system::remove_block_from_free_list() finished");
}

std::string allocator_buddies_system::get_block_dump(
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

std::string allocator_buddies_system::get_memory_state() const
{
    std::ostringstream oss;

    size_t metadata_total_size = get_metadata_size() + (get_max_block_power() - get_min_block_power() + 1) * sizeof(void *);
    void *memory_start = reinterpret_cast<unsigned char *>(_trusted_memory) + metadata_total_size;
    void *memory_end = reinterpret_cast<unsigned char *>(_trusted_memory) + metadata_total_size + get_memory_size();

    std::vector<std::pair<void *, std::pair<bool, size_t>>> blocks;

    void **free_lists = get_free_lists_start();
    size_t min_power = get_min_block_power();
    size_t max_power = get_max_block_power();

    for (size_t p = min_power; p <= max_power; ++p)
    {
        void *current = free_lists[p - min_power];
        while (current != nullptr)
        {
            blocks.push_back({current, {false, power_of_two(p)}});
            current = get_next_block(current);
        }
    }

    for (unsigned char *ptr = reinterpret_cast<unsigned char *>(memory_start);
         ptr < reinterpret_cast<unsigned char *>(memory_end);)
    {
        void *block = ptr;
        bool found = false;

        for (auto const &b : blocks)
        {
            if (b.first == block)
            {
                found = true;
                break;
            }
        }

        if (!found && get_trusted_memory_from_block(block) == _trusted_memory && get_block_is_occupied(block))
        {
            size_t block_power = get_block_power(block);
            blocks.push_back({block, {true, power_of_two(block_power)}});
            ptr += power_of_two(block_power);
        }
        else if (found)
        {
            for (auto const &b : blocks)
            {
                if (b.first == block)
                {
                    ptr += b.second.second;
                    break;
                }
            }
        }
        else
        {
            ptr += get_block_metadata_size();
        }
    }

    std::sort(blocks.begin(), blocks.end(),
              [](auto const &a, auto const &b)
              { return a.first < b.first; });

    bool first = true;
    for (auto const &block : blocks)
    {
        if (!first)
        {
            oss << " | ";
        }
        first = false;

        oss << (block.second.first ? "occup" : "avail") << " " << block.second.second;
    }

    return oss.str();
}

size_t allocator_buddies_system::get_available_memory() const
{
    size_t available = 0;

    void **free_lists = get_free_lists_start();
    size_t min_power = get_min_block_power();
    size_t max_power = get_max_block_power();

    for (size_t p = min_power; p <= max_power; ++p)
    {
        void *current = free_lists[p - min_power];
        while (current != nullptr)
        {
            available += power_of_two(p);
            current = get_next_block(current);
        }
    }

    return available;
}

void *allocator_buddies_system::get_user_block_from_block(
    void *block) const
{
    return reinterpret_cast<unsigned char *>(block) + get_block_metadata_size();
}

void *allocator_buddies_system::get_block_from_user_block(
    void *user_block) const
{
    return reinterpret_cast<unsigned char *>(user_block) - get_block_metadata_size();
}

size_t allocator_buddies_system::power_of_two(
    size_t n) const
{
    return static_cast<size_t>(1) << n;
}