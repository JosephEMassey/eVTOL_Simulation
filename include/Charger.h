#ifndef CHARGER_H
#define CHARGER_H

#include "Vehicle.h"
#include "TLockedQueue.h"

class Charger
{
    private:
        static void Run(TLockedQueue<Vehicle>& chargingQ, const unsigned int& charger_num);

    public:
        Charger( const unsigned int num_chargers, 
                 TLockedQueue<Vehicle>& vehicle_charging_q);
                 
        virtual ~Charger() = default;
};

#endif