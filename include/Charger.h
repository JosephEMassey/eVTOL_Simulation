#ifndef CHARGER_H
#define CHARGER_H

#include <memory>

#include "Vehicle.h"
#include "SimulationObject.h"

/**
 * @brief Simulates a charger (consumer) running in a thread.
 * 
 */
class Charger : public SimulationObject
{
public:

    //
    // Charger
    //
    
    /**
     * @brief Construct a new Charger object
     * 
     * @param id Charger identification.
     * @param charging_q Vehicle charging queue.
     */
    Charger(uint16_t   id, 
            ChargingQ& charging_q);

    /**
     * @brief Default Constructor (disabled).
     * 
     */
    Charger() = delete;

    /**
     * @brief Default Copy Constructor (disabled).
     * 
     */
    Charger(const Charger &) = delete;

    /**
     * @brief Assignment operator (disabled).
     * 
     * @return Charger& 
     */
    Charger &operator=(const Charger &) = delete; // Disable Assignment
    
    /**
     * @brief Destroy the Charger object
     * 
     */
    virtual ~Charger() = default;

    //
    // SimulationObject overrides
    //

    /**
     * @brief Prints charger statistics to console.
     * 
     */
    virtual void PrintStats() override;
    
    //
    // SimulationThread overrides
    //

    /**
     * @brief Consumes shared vehicle queue (thread-safe) of vehicles and charges them.
     * 
     */
    virtual void Run() override;

protected:

    //
    // SimulationObject overrides
    //

    /**
     * @brief String used to uniquely identify this object.
     * 
     * @return const std::string Header used to uniquely identify charger.
     */
    virtual const std::string Header() override;
    
    /**
     * @brief Header of this object.
     * 
     */
    const std::string _header;

    /**
     * @brief Id of this object.
     * 
     */
    const uint16_t _id;

    /**
     * @brief Vehicle charging queue.
     * 
     */
    ChargingQ& _charging_q;
};

#endif