#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>

#include "Vehicle.h"

class Simulation
{
    private:
        std::vector<std::shared_ptr<Vehicle>> _vehicles;

    public:
        Simulation() = default;
        virtual ~Simulation() = default;

        void Create(const unsigned short numVehicles,
                    const unsigned short numVehicleTypes);
        
        void Start();
        

    private:
        
        static void Run(std::shared_ptr<Vehicle> v);
        
        std::shared_ptr<Vehicle> CreateRandomVehicle();

};


#endif