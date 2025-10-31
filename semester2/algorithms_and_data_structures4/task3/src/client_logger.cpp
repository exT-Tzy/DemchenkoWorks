#include "../include/client_logger.h"

#include <fstream>
#include <set>
#include <chrono>
#include <filesystem>

std::map<std::string, std::pair<std::ostream*, size_t>> client_logger::_all_streams = std::map<std::string, std::pair<std::ostream*, size_t>>();

client_logger::client_logger(
    std::map<logger::severity, std::pair<std::set<std::string>, std::string>> const& streams,
    std::string log_format) :
    _log_format(std::move(log_format))
{
    std::set<std::string> registered_paths;

    for (auto const& severity_path : streams)
    {
        _streams[severity_path.first] = std::vector<std::pair<std::ostream*, std::string>>(severity_path.second.first.size());
        int i = 0;

        for (auto const& path : severity_path.second.first)
        {
            auto it = _all_streams.find(path);

            if (it == _all_streams.cend())
            {
                _all_streams[path] = std::make_pair(path.empty()
                    ? &std::cout
                    : new std::ofstream(path), 1);

                it = _all_streams.find(path);

                registered_paths.insert(path);
            }
            else if (!registered_paths.contains(it->first))
            {
                ++(it->second.second);

                registered_paths.insert(path);
            }

            _streams[severity_path.first][i++] = std::make_pair(it->second.first, path);
        }
    }
}

client_logger::~client_logger()
{
    cleanup_streams();
}

client_logger::client_logger(
    client_logger const& other): _streams(other._streams), _log_format(other._log_format)
{
    increment_counts_of_stream_references();
}

client_logger& client_logger::operator=(
    client_logger const& other)
{
    if (this != &other)
    {
        cleanup_streams();

        _streams = other._streams;
        _log_format = other._log_format;

        increment_counts_of_stream_references();
    }

    return *this;
}

client_logger::client_logger(
    client_logger&& other) noexcept
{
    _streams = std::move(other._streams);
    _log_format = std::move(other._log_format);
}

client_logger& client_logger::operator=(
    client_logger&& other) noexcept
{
    if (this != &other)
    {
        cleanup_streams();

        _streams = std::move(other._streams);
        _log_format = std::move(other._log_format);
    }

    return *this;
}

logger const* client_logger::log(
    std::string const& message,
    logger::severity severity) const noexcept
{
    std::chrono::system_clock::time_point log_time = std::chrono::system_clock::now();
    
    auto it = _streams.find(severity);

    if (it == _streams.cend())
    {
        return this;
    }

    auto formatted_message = format_log(message, severity, log_time);

    for (auto const& stream_path : it->second)
    {
        *stream_path.first << formatted_message << std::endl;
    }

    return this;
}

void client_logger::cleanup_streams()
{
    std::set<std::string> unregistered_paths;

    for (auto const& severity_stream_path : _streams)
    {
        for (auto const& stream_path : severity_stream_path.second)
        {
            if (!unregistered_paths.contains(stream_path.second))
            {
                unregistered_paths.insert(stream_path.second);

                auto it = _all_streams.find(stream_path.second);

                if (--(it->second.second) == 0)
                {
                    if (it->second.first != &std::cout)
                    {
                        it->second.first->flush();
                        delete it->second.first;
                    }

                    _all_streams.erase(it);
                }
            }
        }
    }
}

void client_logger::increment_counts_of_stream_references()
{
    for (auto& severity_streams : _streams)
    {
        for (auto& stream_pair : severity_streams.second)
        {
            auto& stream_info = _all_streams[stream_pair.second];
            ++stream_info.second;
        }
    }
}

std::string client_logger::format_log(
    std::string const& message,
    logger::severity severity,
    std::chrono::system_clock::time_point const & time_point) const
{
    std::string formatted_log;
    bool previous_symbol_was_percent_flag = false;

    for (auto const& symbol : _log_format)
    {
        if (previous_symbol_was_percent_flag)
        {
            switch (symbol)
            {
            case 'd':

                formatted_log += get_date(time_point);

                break;

            case 't':

                formatted_log += get_time(time_point);

                break;

            case 's':

                formatted_log += severity_to_string(severity);

                break;

            case 'm':

                formatted_log += message;

                break;
            }

            previous_symbol_was_percent_flag = false;
        }
        else
        {
            if (symbol == '%')
            {
                previous_symbol_was_percent_flag = true;
            }
            else
            {
                formatted_log += symbol;
            }
        }
    }

    return formatted_log;
}