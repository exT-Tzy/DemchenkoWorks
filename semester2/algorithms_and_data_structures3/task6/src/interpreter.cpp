#include "../../../algorithms_and_data_structures4/task5/include/allocator_buddies_system.h"
#include "../include/interpreter.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

interpreter::interpreter(
    size_t max_capacity) : _max_memory_capacity(max_capacity),
                           _current_depth(0)
{
    _allocator = new allocator_buddies_system(max_capacity, nullptr, nullptr);
}

interpreter::~interpreter()
{
    for (auto var : _all_variables)
    {
        if (var->get_memory() != nullptr)
        {
            try
            {
                _allocator->deallocate(var->get_memory());
            }
            catch (...)
            {
            }
        }
        delete var;
    }

    delete _allocator;
}

void interpreter::process_file(
    std::string const &file_path)
{
    std::ifstream file(file_path);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << file_path << std::endl;
        throw std::runtime_error("Could not open file");
    }

    std::cout << "\n=== Processing file: " << file_path << " ===" << std::endl;

    std::string line;
    while (std::getline(file, line))
    {
        process_line(line);
    }

    while (_current_depth > 0)
    {
        exit_scope();
    }

    file.close();
}

void interpreter::process_line(
    std::string const &line)
{
    std::string trimmed = trim(line);

    if (trimmed.empty())
    {
        return;
    }

    if (trimmed == "{")
    {
        enter_scope();
        return;
    }

    if (trimmed == "}")
    {
        exit_scope();
        return;
    }

    if (trimmed == "ShowVar;")
    {
        handle_show_var();
        return;
    }

    size_t eq_pos = trimmed.find('=');
    if (eq_pos != std::string::npos)
    {
        std::string var_name = trim(trimmed.substr(0, eq_pos));
        std::string right_side = trim(trimmed.substr(eq_pos + 1));

        if (right_side.find("new(") == 0 && right_side.back() == ';')
        {
            right_side.pop_back();

            size_t open_paren = right_side.find('(');
            size_t close_paren = right_side.find(')');

            if (open_paren != std::string::npos && close_paren != std::string::npos)
            {
                std::string size_str = trim(right_side.substr(open_paren + 1, close_paren - open_paren - 1));
                size_t size = parse_hex_size(size_str);

                handle_new_operator(var_name, size);
            }
        }
    }
}

void interpreter::handle_new_operator(
    std::string const &var_name,
    size_t size)
{
    std::cout << "Allocating variable '" << var_name << "' with size 0x" << std::hex << size << std::dec << " (" << size << " bytes)" << std::endl;

    variable *existing_var = find_variable(var_name);
    if (existing_var != nullptr && existing_var->get_depth() == _current_depth)
    {
        std::cout << "Warning: Variable '" << var_name << "' already exists in current scope, deallocating old memory" << std::endl;

        if (existing_var->get_memory() != nullptr)
        {
            try
            {
                _allocator->deallocate(existing_var->get_memory());
            }
            catch (std::exception const &ex)
            {
                std::cerr << "Error deallocating old variable: " << ex.what() << std::endl;
            }
        }

        auto it = std::find(_all_variables.begin(), _all_variables.end(), existing_var);
        if (it != _all_variables.end())
        {
            _all_variables.erase(it);
        }
        delete existing_var;
    }

    void *memory = nullptr;

    try
    {
        memory = _allocator->allocate(1, size);
    }
    catch (std::bad_alloc const &)
    {
        std::cout << "Allocation failed, running garbage collection..." << std::endl;

        cleanup_blocked_variables();

        try
        {
            memory = _allocator->allocate(1, size);
        }
        catch (std::bad_alloc const &)
        {
            std::cerr << "Fatal error: Cannot allocate memory even after garbage collection" << std::endl;
            std::cerr << "Requested size: " << size << " bytes" << std::endl;
            std::cerr << "Terminating application..." << std::endl;
            exit(1);
        }
    }

    variable *new_var = new variable(var_name, memory, size, _current_depth);
    _all_variables.push_back(new_var);

    std::cout << "Successfully allocated variable '" << var_name << "' at address " << memory << std::endl;
}

void interpreter::handle_show_var()
{
    std::cout << "\n--- ShowVar (depth " << _current_depth << ") ---" << std::endl;

    auto visible = get_visible_variables();

    if (visible.empty())
    {
        std::cout << "No visible variables" << std::endl;
    }
    else
    {
        std::cout << "Visible variables:" << std::endl;
        for (auto var : visible)
        {
            std::cout << "  " << var->get_name()
                      << " [depth=" << var->get_depth()
                      << ", size=0x" << std::hex << var->get_size() << std::dec
                      << " (" << var->get_size() << " bytes)"
                      << ", addr=" << var->get_memory()
                      << "]" << std::endl;
        }
    }

    std::cout << "--- End ShowVar ---\n"
              << std::endl;
}

void interpreter::enter_scope()
{
    ++_current_depth;
    std::cout << "Entering scope (depth=" << _current_depth << ")" << std::endl;
}

void interpreter::exit_scope()
{
    std::cout << "Exiting scope (depth=" << _current_depth << ")" << std::endl;

    if (_current_depth > 0)
    {
        --_current_depth;
    }
}

void interpreter::cleanup_blocked_variables()
{
    auto blocked = get_blocked_variables();

    std::cout << "Garbage collection: found " << blocked.size() << " blocked variables" << std::endl;

    for (auto var : blocked)
    {
        std::cout << "  Deallocating blocked variable '" << var->get_name()
                  << "' (depth=" << var->get_depth()
                  << ", size=" << var->get_size() << " bytes)" << std::endl;

        if (var->get_memory() != nullptr)
        {
            try
            {
                _allocator->deallocate(var->get_memory());
            }
            catch (std::exception const &ex)
            {
                std::cerr << "Error during garbage collection: " << ex.what() << std::endl;
            }
        }

        auto it = std::find(_all_variables.begin(), _all_variables.end(), var);
        if (it != _all_variables.end())
        {
            _all_variables.erase(it);
        }
        delete var;
    }

    std::cout << "Garbage collection completed" << std::endl;
}

variable *interpreter::find_variable(
    std::string const &name) const
{
    for (auto it = _all_variables.rbegin(); it != _all_variables.rend(); ++it)
    {
        if ((*it)->get_name() == name && (*it)->get_depth() <= _current_depth)
        {
            return *it;
        }
    }
    return nullptr;
}

std::vector<variable *> interpreter::get_visible_variables() const
{
    std::vector<variable *> result;

    for (auto var : _all_variables)
    {
        if (var->get_depth() <= _current_depth)
        {
            result.push_back(var);
        }
    }

    return result;
}

std::vector<variable *> interpreter::get_blocked_variables() const
{
    std::vector<variable *> result;

    for (auto var : _all_variables)
    {
        if (var->get_depth() > _current_depth)
        {
            result.push_back(var);
        }
    }

    return result;
}

std::string interpreter::trim(
    std::string const &str) const
{
    size_t start = 0;
    while (start < str.length() && std::isspace(str[start]))
    {
        ++start;
    }

    size_t end = str.length();
    while (end > start && std::isspace(str[end - 1]))
    {
        --end;
    }

    return str.substr(start, end - start);
}

size_t interpreter::parse_hex_size(
    std::string const &hex_str) const
{
    std::istringstream iss(hex_str);
    size_t value = 0;
    iss >> std::hex >> value;
    return value;
}