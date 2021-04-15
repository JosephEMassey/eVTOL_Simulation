#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>

#include "Vehicle.h"
#include "TLockedQueue.h"
#include "Charger.h"

class Simulation
{
public:
    Simulation()                              = delete; // Disable Default CTor
    Simulation(const Simulation &)            = delete; // Disable Copy
    Simulation &operator=(const Simulation &) = delete; // Disable Assignment
    Simulation(const unsigned short num_vehicles,
               const unsigned short num_vehicle_types,
               const unsigned short num_chargers);
 
    virtual ~Simulation() = default;

    void Create();        
    void Run(const int64_t secs);

private:
    std::vector<std::shared_ptr<SimulationObject>> _sim_objs;
    TLockedQueue<std::shared_ptr<Vehicle>>         _vehicle_charging_q;

    const unsigned short _num_vehicles;
    const unsigned short _num_vehicle_types;
    const unsigned short _num_chargers;
};


#endif