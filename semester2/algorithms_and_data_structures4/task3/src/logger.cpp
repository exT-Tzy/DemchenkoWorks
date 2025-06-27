#include "logger.h"

#include <iomanip>
#include <sstream>
#include <format>
#include <chrono>

logger const* logger::trace(
    std::string const& message) const noexcept
{
    return log(message, logger::severity::trace);
}

logger const* logger::debug(
    std::string const& message) const noexcept
{
    return log(message, logger::severity::debug);
}

logger const* logger::information(
    std::string const& message) const noexcept
{
    return log(message, logger::severity::information);
}

logger const* logger::warning(
    std::string const& message) const noexcept
{
    return log(message, logger::severity::warning);
}

logger const* logger::error(
    std::string const& message) const noexcept
{
    return log(message, logger::severity::error);
}

logger const* logger::critical(
    std::string const& message) const noexcept
{
    return log(message, logger::severity::critical);
}

std::string logger::severity_to_string(
    logger::severity severity)
{
    switch (severity)
    {
    case logger::severity::trace:
        return "TRACE";
    case logger::severity::debug:
        return "DEBUG";
    case logger::severity::information:
        return "INFORMATION";
    case logger::severity::warning:
        return "WARNING";
    case logger::severity::error:
        return "ERROR";
    case logger::severity::critical:
        return "CRITICAL";
    }

    throw std::out_of_range("Invalid severity value");
}

std::string logger::get_date(
    const std::chrono::system_clock::time_point& time_point)
{
    return std::format("{:%Y-%m-%d}", std::chrono::zoned_time(std::chrono::current_zone(), time_point));
}

std::string logger::get_time(
    const std::chrono::system_clock::time_point& time_point)
{
    return std::format("{:%H:%M:%S}", std::chrono::zoned_time(std::chrono::current_zone(), time_point));
}