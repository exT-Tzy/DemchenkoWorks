#include "client_logger.h"

std::map<std::string, std::pair<std::ostream*, size_t>> client_logger::_all_streams = std::map<std::string, std::pair<std::ostream*, size_t>>();

std::string client_logger::get_absolute_path(
    std::string const& relative_path)
{
    fs::path p = relative_path;

    if (!fs::exists(p))
    {
        throw std::invalid_argument("file doesnt exist!");
    }

    fs::path absolute_path = fs::absolute(p);

    return absolute_path.string();
}

client_logger::client_logger(
    std::map<logger::severity, std::vector<std::string>> const& streams)
{
    for (auto& streams_pair : streams)
    {
        _streams[streams_pair.first];

        for (auto& relative_path : streams_pair.second)
        {
            std::string absolute_path;

            try
            {
                absolute_path = get_absolute_path(relative_path);
            }
            catch (std::invalid_argument const& e)
            {
                // TODO

                continue;
            }

            auto all_streams_pair_iterator = _all_streams.find(absolute_path);

            if (all_streams_pair_iterator != _all_streams.end())
            {
                _streams[streams_pair.first].push_back((*all_streams_pair_iterator).second.first);

                ++(*all_streams_pair_iterator).second.second;
            }
            else
            {
                std::ofstream output_file_stream(absolute_path);

                _streams[streams_pair.first].push_back(&output_file_stream);

                _all_streams.emplace(absolute_path, std::pair<std::ostream*, size_t>(&output_file_stream, 1));
            }
        }
    }
}