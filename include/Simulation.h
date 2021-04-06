#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>

#include "Vehicle.h"
#include "TLockedQueue.h"
#include "Charger.h"

class Simulation
{
    private:
        std::vector<std::shared_ptr<Vehicle>> _vehicles;
        TLockedQueue<std::shared_ptr<Vehicle>> _vehicle_charging_q;
        std::shared_ptr<Charger> _charger;

    public:
        Simulation();
        virtual ~Simulation() = default;

        void Create(const unsigned short numVehicles,
                    const unsigned short numVehicleTypes,
                    const unsigned short numChargers);
        
        void Start();
        

    private:
        
        static void Run(const std::shared_ptr<Vehicle>& v, 
                        TLockedQueue<std::shared_ptr<Vehicle>>& chargingQ);
        
        std::shared_ptr<Vehicle> CreateRandomVehicle();



};


#endif