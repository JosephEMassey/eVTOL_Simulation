#include <iostream>
#include <memory>
#include <string>

#include "Simulation.h"

// ./eVTOL_Simulation -v 20 -c 3 -s 180

int main(int argc, char** argv)
{   
    // Default values
    uint16_t num_vehicles     = 20;
    uint16_t num_vehicleTypes = 5;
    uint16_t num_chargers     = 3;
    uint64_t secs             = 10;

    // Simple way to parse command line args
    for(int i = 0; i < argc; ++i)
    {
        std::string s(argv[i]);
        
        // Number of vehicles
        if(s == "-v")
        {
            std::istringstream(argv[i+1]) >> num_vehicles;
            i++;
        }

        // Number of chargers
        else if(s == "-c")
        {
            std::istringstream(argv[i+1]) >> num_chargers;
            i++;
        }

        // Simulation time in seconds
        else if (s == "-s")
        {
            std::istringstream(argv[i+1]) >> secs;
            i++;
        }

    }

    auto sim = std::make_shared<Simulation>(num_vehicles, num_vehicleTypes, num_chargers);
    sim->Create();
    sim->Run(secs);

    return 0;
}