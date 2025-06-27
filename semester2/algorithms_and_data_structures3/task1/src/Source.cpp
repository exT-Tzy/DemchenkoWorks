#include <iostream>

#include "elevator_system.h"
#include "utils.h"

int main(
    int argc, 
    char* argv[]) 
{
    if (argc < 3)
    {
        std::cout << "Usage: " << argv[0] << " <config_file> <passenger_file1> [passenger_file2] ..." << std::endl;
        
        return 1;
    }

    std::vector<int> config = parse_config(argv[1]);
    
    if (config.size() < 2) 
    {
        std::cout << "Invalid configuration file format" << std::endl;
        
        return 1;
    }

    int floors = config[0];
    int numElevators = config[1];
    std::vector<int> elevatorCapacities;
    
    for (int i = 2; i < config.size() && i < 2 + numElevators; i++) 
    {
        elevatorCapacities.push_back(config[i]);
    }

    elevator_system system(floors, elevatorCapacities);

    for (int i = 2; i < argc; i++) 
    {
        std::vector<passenger*> passengers = parse_passengers(argv[i]);
        
        for (passenger* passenger : passengers)
        {
            system.add_passenger(passenger);
        }
    }

    system.simulate(86400);

    system.generate_reports("passenger_report.txt", "elevator_report.txt");

    std::cout << "Simulation completed. Reports generated." << std::endl;

    return 0;
}