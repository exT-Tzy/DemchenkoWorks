#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>
#include <string>
#include <memory>

#include "variable.h"
#include "allocator_buddies_system.h"

class interpreter final
{
private:
    size_t _max_memory_capacity;
    allocator_buddies_system *_allocator;
    std::vector<variable *> _all_variables;
    size_t _current_depth;

public:
    interpreter(
        size_t max_capacity);

    ~interpreter();

    void process_file(
        std::string const &file_path);

private:
    void process_line(
        std::string const &line);

    void handle_new_operator(
        std::string const &var_name,
        size_t size);

    void handle_show_var();

    void enter_scope();

    void exit_scope();

    void cleanup_blocked_variables();

    variable *find_variable(
        std::string const &name) const;

    std::vector<variable *> get_visible_variables() const;

    std::vector<variable *> get_blocked_variables() const;

    std::string trim(
        std::string const &str) const;

    size_t parse_hex_size(
        std::string const &hex_str) const;
};

#endif // INTERPRETER_H