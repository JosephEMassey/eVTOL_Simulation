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
    
    Charger( const unsigned int id, 
             ChargingQ& charging_q);

    virtual ~Charger() = default;

    // SimulationObject
    virtual void PrintStats() override;

    // SimulationThread
    virtual void Run() override;
    
private:
    const unsigned short _id;
    ChargingQ& _charging_q;

    void print(const std::stringstream& ss);
};

#endif