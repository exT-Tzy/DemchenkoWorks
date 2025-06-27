#include "utils.h"

#include <sstream>
#include <fstream>

std::vector<std::string> split(
    const std::string& str, 
    char delimiter) 
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) 
    {
        tokens.push_back(token);
    }
   
    return tokens;
}

int parse_time(
    const std::string& timeStr) 
{
    std::vector<std::string> parts = split(timeStr, ':');
    
    if (parts.size() == 2)
    {
        int minutes = std::stoi(parts[0]);
        int seconds = std::stoi(parts[1]);
       
        return minutes * 60 + seconds;
    }
    
    return 0;
}

std::vector<int> parse_config(
    const std::string& filename) 
{
    std::ifstream file(filename);
    std::vector<int> config;

    if (file.is_open()) 
    {
        std::string line;
       
        while (std::getline(file, line)) 
        {
            if (!line.empty()) 
            {
                config.push_back(std::stoi(line));
            }
        }
       
        file.close();
    }

    return config;
}

std::vector<passenger*> parse_passengers(
    const std::string& filename) 
{
    std::vector<passenger*> passengers;
    std::ifstream file(filename);

    if (file.is_open()) 
    {
        std::string line;
       
        while (std::getline(file, line)) 
        {
            if (!line.empty())
            {
                std::istringstream iss(line);
                int id, sourceFloor, targetFloor;
                double weight;
                
                std::string timeStr;

                if (iss >> id >> weight >> sourceFloor >> timeStr >> targetFloor)
                {
                    int appearanceTime = parse_time(timeStr);
                    passengers.push_back(new passenger(id, weight, sourceFloor, targetFloor, appearanceTime));
                }
            }
        }
        
        file.close();
    }

    return passengers;
}