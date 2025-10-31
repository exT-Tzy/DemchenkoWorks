#include <iostream>

#include "../include/logger.h"
#include "../include/client_logger.h"
#include "../include/client_logger_builder.h"

int main()
{
    logger *log = client_logger_builder()
                      .add_console_stream(logger::severity::information)
                      ->build();

    log->information("Hello");

    return 0;
}