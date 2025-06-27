#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <chrono>

class logger
{

public:

    enum class severity
    {
        trace,
        debug,
        information,
        warning,
        error,
        critical
    };

public:

    virtual ~logger() noexcept = default;

public:

    virtual logger const* log(
        std::string const& message,
        logger::severity severity) const noexcept = 0;

public:

    logger const* trace(
        std::string const& message) const noexcept;

    logger const* debug(
        std::string const& message) const noexcept;

    logger const* information(
        std::string const& message) const noexcept;

    logger const* warning(
        std::string const& message) const noexcept;

    logger const* error(
        std::string const& message) const noexcept;

    logger const* critical(
        std::string const& message) const noexcept;

protected:

    static std::string severity_to_string(
        logger::severity severity);

    static std::string get_time(
        const std::chrono::system_clock::time_point& time_point);

    static std::string get_date(
        const std::chrono::system_clock::time_point& time_point);
};

#endif //LOGGER_H