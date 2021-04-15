#ifndef SIMULATION_OBJECT_H
#define SIMULATION_OBJECT_H

#include "SimulationThread.h"

class SimulationObject : public SimulationThread
{
public:

    SimulationObject()                                    = default; // Default CTor
    SimulationObject(const SimulationObject &)            = delete;  // Disable Copy
    SimulationObject &operator=(const SimulationObject &) = delete;  // Disable Assignment

    virtual ~SimulationObject() = default; // Default DTor

    virtual void PrintStats() = 0;
};

#endif