#include <iostream>

#include "../include/simulation.h"

int main(
    int argc,
    char *argv[])
{
    if (argc != 4)
    {
        std::cout << "Usage: " << argv[0] << " <N> <M> <T>" << std::endl;

        return 1;
    }

    try
    {
        int n = std::stoi(argv[1]);
        int m = std::stoi(argv[2]);
        int t = std::stoi(argv[3]);

        if (n <= 0 || m <= 0 || t <= 0)
        {
            throw std::invalid_argument("all parameters must be positive");
        }

        hospital_simulation sim(n, m, t);

        int simulation_duration = 10000;
        int min_arrival_interval = 50;
        int max_arrival_interval = 200;

        sim.run(simulation_duration, min_arrival_interval, max_arrival_interval);

        std::cout << "simulation completed" << std::endl;

        std::cout << "check log files:" << std::endl;
        std::cout << "  patients_log.txt" << std::endl;
        std::cout << "  doctors_log.txt" << std::endl;
        std::cout << "  service_log.txt" << std::endl;
        std::cout << "  queue_log.txt" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "\nerror: " << e.what() << std::endl;

        return 1;
    }

    return 0;
}