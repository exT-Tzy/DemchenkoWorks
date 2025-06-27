#include <iostream>

#include "logger.h"
#include "logger_builder.h"
#include "client_logger_builder.h"

int main()
{
    logger* log = client_logger_builder()
        .add_file_stream("file1.log", logger::severity::information)
        ->add_file_stream("file2.log", logger::severity::warning)
        ->add_console_stream(logger::severity::information)
        ->add_console_stream(logger::severity::critical)
        ->build();

    log
        ->trace("trace log")
        ->debug("debug log")
        ->information("information log")
        ->warning("warning log")
        ->error("error log")
        ->critical("where are labs?!")
        ->critical("smoking?");

    delete log;

    return 0;
}