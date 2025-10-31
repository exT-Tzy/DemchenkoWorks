#include "allocator_global_heap.h"

#include <sstream>
#include <iomanip>

allocator_global_heap::~allocator_global_heap()
{
    if (_logger != nullptr)
    {
        _logger->trace("allocator_global_heap::~allocator_global_heap() started");
        _logger->debug("allocator_global_heap::~allocator_global_heap() called");
        _logger->debug("allocator_global_heap::~allocator_global_heap() finished");
        _logger->trace("allocator_global_heap::~allocator_global_heap() finished");
    }
}

allocator_global_heap::allocator_global_heap(
    allocator_global_heap &&other) noexcept : _logger(other._logger)
{
    if (_logger != nullptr)
    {
        _logger->trace("allocator_global_heap::allocator_global_heap(move) started");
        _logger->debug("allocator_global_heap::allocator_global_heap(move) called");
    }

    other._logger = nullptr;

    if (_logger != nullptr)
    {
        _logger->debug("allocator_global_heap::allocator_global_heap(move) finished");
        _logger->trace("allocator_global_heap::allocator_global_heap(move) finished");
    }
}

allocator_global_heap &allocator_global_heap::operator=(
    allocator_global_heap &&other) noexcept
{
    trace_with_guard("allocator_global_heap::operator=(move) started");
    debug_with_guard("allocator_global_heap::operator=(move) called");

    if (this != &other)
    {
        _logger = other._logger;
        other._logger = nullptr;
    }

    debug_with_guard("allocator_global_heap::operator=(move) finished");
    trace_with_guard("allocator_global_heap::operator=(move) finished");

    return *this;
}

allocator_global_heap::allocator_global_heap(
    logger *logger) : _logger(logger)
{
    trace_with_guard("allocator_global_heap::allocator_global_heap(logger*) started");
    debug_with_guard("allocator_global_heap::allocator_global_heap(logger*) called");
    debug_with_guard("allocator_global_heap::allocator_global_heap(logger*) finished");
    trace_with_guard("allocator_global_heap::allocator_global_heap(logger*) finished");
}

[[nodiscard]] void *allocator_global_heap::allocate(
    size_t value_size,
    size_t values_count)
{
    trace_with_guard("allocator_global_heap::allocate() started");
    debug_with_guard("allocator_global_heap::allocate() called");

    size_t requested_size = value_size * values_count;
    size_t total_size = requested_size + get_block_metadata_size();

    std::ostringstream oss;
    oss << "Allocating " << requested_size << " bytes (total with metadata: " << total_size << " bytes)";
    trace_with_guard(oss.str());

    void *allocated_block = nullptr;

    try
    {
        allocated_block = ::operator new(total_size);
    }
    catch (std::bad_alloc const &ex)
    {
        std::ostringstream error_oss;
        error_oss << "Failed to allocate " << total_size << " bytes: " << ex.what();
        error_with_guard(error_oss.str());

        debug_with_guard("allocator_global_heap::allocate() finished with exception");
        trace_with_guard("allocator_global_heap::allocate() finished with exception");

        throw;
    }

    get_allocator_from_block(allocated_block) = this;
    get_size_from_block(allocated_block) = requested_size;

    void *user_block = reinterpret_cast<unsigned char *>(allocated_block) + get_block_metadata_size();

    std::ostringstream success_oss;
    success_oss << "Successfully allocated block at " << user_block << " (internal: " << allocated_block << ")";
    trace_with_guard(success_oss.str());

    debug_with_guard("allocator_global_heap::allocate() finished");
    trace_with_guard("allocator_global_heap::allocate() finished");

    return user_block;
}

void allocator_global_heap::deallocate(
    void *at)
{
    trace_with_guard("allocator_global_heap::deallocate() started");
    debug_with_guard("allocator_global_heap::deallocate() called");

    if (at == nullptr)
    {
        error_with_guard("Attempted to deallocate nullptr");

        debug_with_guard("allocator_global_heap::deallocate() finished with exception");
        trace_with_guard("allocator_global_heap::deallocate() finished with exception");

        throw std::logic_error("Cannot deallocate nullptr");
    }

    void *internal_block = reinterpret_cast<unsigned char *>(at) - get_block_metadata_size();

    allocator *block_allocator = get_allocator_from_block(internal_block);

    if (block_allocator != this)
    {
        std::ostringstream error_oss;
        error_oss << "Block at " << at << " does not belong to this allocator instance";
        error_with_guard(error_oss.str());

        debug_with_guard("allocator_global_heap::deallocate() finished with exception");
        trace_with_guard("allocator_global_heap::deallocate() finished with exception");

        throw std::logic_error("Block does not belong to this allocator instance");
    }

    size_t block_size = get_size_from_block(internal_block);

    if (block_size > 0 && block_size <= 4096)
    {
        std::string dump = get_block_dump(at, block_size);
        std::ostringstream dump_oss;
        dump_oss << "Block state before deallocation (size: " << block_size << " bytes): " << dump;
        debug_with_guard(dump_oss.str());
    }

    std::ostringstream dealloc_oss;
    dealloc_oss << "Deallocating block at " << at << " (internal: " << internal_block << ")";
    trace_with_guard(dealloc_oss.str());

    ::operator delete(internal_block);

    trace_with_guard("Block successfully deallocated");

    debug_with_guard("allocator_global_heap::deallocate() finished");
    trace_with_guard("allocator_global_heap::deallocate() finished");
}

inline allocator *allocator_global_heap::get_allocator() const
{
    return nullptr;
}

inline logger *allocator_global_heap::get_logger() const
{
    return _logger;
}

inline constexpr size_t allocator_global_heap::get_block_metadata_size() noexcept
{
    return sizeof(allocator *) + sizeof(size_t);
}

inline allocator *&allocator_global_heap::get_allocator_from_block(
    void *block) const
{
    return *reinterpret_cast<allocator **>(block);
}

inline size_t &allocator_global_heap::get_size_from_block(
    void *block) const
{
    return *reinterpret_cast<size_t *>(reinterpret_cast<unsigned char *>(block) + sizeof(allocator *));
}

std::string allocator_global_heap::get_block_dump(
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