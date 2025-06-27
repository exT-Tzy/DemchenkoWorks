#ifndef CLIENT_LOGGER_BUILDER_H
#define CLIENT_LOGGER_BUILDER_H

#include <map>
#include <set>
#include <vector>

#include "logger_builder.h"

class client_logger_builder final :
    public logger_builder
{

private:

    std::map<logger::severity, std::pair<std::set<std::string>, std::string>> _streams_info;
    std::string _log_format;

public:

    client_logger_builder();

public:

    logger_builder* set_log_format(
        std::string const& format);

public:

    logger_builder* add_file_stream(
        std::string const& stream_file_path,
        logger::severity severity) override;

    logger_builder* add_console_stream(
        logger::severity severity) override;

    logger_builder* transform_with_configuration(
        std::string const& configuration_file_path,
        std::string const& configuration_path) override;

    logger_builder* clear() override;

    logger* build() const override;

private:

    static std::string get_absolute_path(
        std::string const& relative_path);

};

#endif //CLIENT_LOGGER_BUILDER_H