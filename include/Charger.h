#ifndef CHARGER_H
#define CHARGER_H

#include <memory>

#include "Vehicle.h"
#include "SimulationObject.h"

class Charger : public SimulationObject
{
public:
    Charger()                           = delete; // Disable Default CTor
    Charger(const Charger &)            = delete; // Disable Copy
    Charger &operator=(const Charger &) = delete; // Disable Assignment
    
    Charger(uint16_t   id, 
            ChargingQ& charging_q);

    virtual ~Charger() = default;

    // SimulationObject
    virtual void PrintStats() override;
    virtual const std::string Header() override;

    // SimulationThread
    virtual void Run() override;

protected:
    const uint16_t    _id;
    const std::string _header;

    ChargingQ& _charging_q;
};

#endif