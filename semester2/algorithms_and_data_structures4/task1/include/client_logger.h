#ifndef COURSE1_CPP_RGU_CLIENT_LOGGER_H
#define COURSE1_CPP_RGU_CLIENT_LOGGER_H

#include <map>
#include <ostream>
#include <vector>
#include <fstream>

#include <boost/filesystem.hpp>

#include "logger.h"

namespace fs = boost::filesystem;

class client_logger final :
    public logger
{

private:

    static std::map<std::string, std::pair<std::ostream*, size_t>> _all_streams;

private:

    std::map<logger::severity, std::vector<std::ostream*>> _streams;

    friend class client_logger_builder;

private:

    static std::string get_absolute_path(
        std::string const& relative_path);

private:

    client_logger(
        std::map<logger::severity, std::vector<std::string>> const& streams);


};

#endif //COURSE1_CPP_RGU_CLIENT_LOGGER_H