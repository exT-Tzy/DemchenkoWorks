#include "../include/variable.h"

variable::variable(
    std::string const& name,
    void* mem_ptr,
    size_t size,
    size_t depth) :
    _name(name),
    _memory(mem_ptr),
    _size(size),
    _depth(depth)
{
}

std::string const& variable::get_name() const
{
    return _name;
}

void* variable::get_memory() const
{
    return _memory;
}

size_t variable::get_size() const
{
    return _size;
}

size_t variable::get_depth() const
{
    return _depth;
}