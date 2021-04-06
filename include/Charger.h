#ifndef CHARGER_H
#define CHARGER_H

#include <memory>

#include "Vehicle.h"
#include "TLockedQueue.h"

class Charger
{
    private:
        static void Run(TLockedQueue<std::shared_ptr<Vehicle>>& chargingQ, const unsigned int& charger_num);
        
    public:
        explicit Charger( const unsigned int num_chargers, 
                          TLockedQueue<std::shared_ptr<Vehicle>>& vehicle_charging_q);

        void Start(); 
        virtual ~Charger() = default;
};

#endif