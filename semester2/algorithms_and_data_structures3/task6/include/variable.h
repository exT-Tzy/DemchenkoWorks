#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>

class variable final
{
private:
    std::string _name;
    void *_memory;
    size_t _size;
    size_t _depth;

public:
    variable(
        std::string const &name,
        void *mem_ptr,
        size_t size,
        size_t depth);

    ~variable() = default;

    std::string const &get_name() const;

    void *get_memory() const;

    size_t get_size() const;

    size_t get_depth() const;
};

#endif // VARIABLE_H