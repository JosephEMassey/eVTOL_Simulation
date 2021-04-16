#ifndef VEHICLE_H
#define VEHICLE_H

#include <memory>
#include <string>
#include <sstream>

#include "SimulationObject.h"
#include "StopWatch.h"
#include "TLockedQueue.h"

/**
 * @brief Vehicle types.
 * 
 */
enum VehicleType 
{
    A = 0,
    B,
    C,
    D,
    E,
};

/**
 * @brief Vehicle states.
 * 
 */
enum VehicleStateType
{
    INITIAL,
    CRUISING,
    NEEDS_CHARGED,
    CHARGING,
    CHARGED
};

/**
 * @brief Simulates a vehicle (producer) running in a thread.
 * 
 */
class Vehicle : public SimulationObject, 
                public std::enable_shared_from_this<Vehicle>
{

public:

    /**
     * @brief Construct a new Vehicle object.
     * 
     * @param n Name of vehicle.
     * @param bc Battery capacity (kWh).
     * @param cs Cruise speed (mph)
     * @param pc Passenger count.
     * @param eac Energy use at cruise (KWh/mile).
     * @param pof Probability of fault (fault/hr).
     * @param ttc Time to charge (hr)
     * @param id Identification of this vehicle
     * @param chargingQ Vehicle charging queue.
     */
    Vehicle(const std::string& n, 
            const uint16_t     bc,
            const uint16_t     cs,
            const uint16_t     pc,
            const float        eac,
            const float        pof,
            const float        ttc,
            const uint16_t     id,
            TLockedQueue<std::shared_ptr<Vehicle>>& chargingQ);

    /**
     * @brief Default Constructor (disabled).
     * 
     */
    Vehicle() = delete;

    /**
     * @brief Default Copy Constructor (disabled).
     * 
     */
    Vehicle(const Vehicle &) = delete;

    /**
     * @brief Assignment operator (disabled).
     * 
     * @return Vehicle& 
     */
    Vehicle &operator=(const Vehicle &) = delete;

    /**
     * @brief Destroy the Vehicle object.
     * 
     */
    virtual ~Vehicle() = default;

    /**
     * @brief Creates an instance of the requested Vehicle using Factory of vehicles.
     * 
     * @param type 
     * @param id 
     * @param chargingQ 
     * @return std::shared_ptr<Vehicle> 
     */
    static std::shared_ptr<Vehicle> Create(VehicleType type, 
                                           const uint16_t id,
                                           TLockedQueue<std::shared_ptr<Vehicle>>& chargingQ);

    //
    // Properties
    //

    /**
     * @brief Battery capacity (kWh).
     * 
     * @return uint16_t Battery capacity (kWh).
     */
    uint16_t BatteryCapacity() const { return _battery_capacity; }

    /**
     * @brief Cruise speed (mph).
     * 
     * @return uint16_t Cruise speed (mph).
     */
    uint16_t CruiseSpeed() const { return _cruise_speed; }

    /**
     * @brief Energy use at cruise (kWh/mile).
     * 
     * @return float Energy use at cruise (kWh/mile).
     */
    float EnergyUseAtCruise() const { return _energy_use_at_cruise; }

    /**
     * @brief ID of vehicle.
     * 
     * @return uint16_t ID of vehicle.
     */
    uint16_t ID() const { return _id; }

    /**
     * @brief Name of vehicle.
     * 
     * @return const std::string Name of vehicle.
     */
    const std::string Name() const { return _name; }

    /**
     * @brief Passenger count.
     * 
     * @return uint16_t Passenger count.
     */
    uint16_t PassengerCount() const { return _passenger_count; }
    
    /**
     * @brief Probability of a fault occuring / hr.
     * 
     * @return float Probability of a fault occuring / hr.
     */
    float ProbabilityOfFault() const { return _prob_of_fault; }

    /**
     * @brief Time to charge (hrs).
     * 
     * @return float 
     */
    float TimeToCharge() const { return _time_to_charge; }

    //
    // Vehicle
    //

    /**
     * @brief Changes the current state of this vehicle.
     * 
     * @param state State to switch to.
     */
    void ChangeState(VehicleStateType state);

    /**
     * @brief Simulates a vehicle cruising by blocking thread for CruiseTime().
     * 
     */
    void CruiseAction();

    /**
     * @brief Calculates the cruise time in seconds converted to simulation time.
     * 
     * @return int64_t Cruise time in seconds.
     */
    int64_t CruiseTime() const;

    /**
     * @brief Calculates the time to charge in seconds converted to simulation time.
     * 
     * @return int64_t Time to Charge in seconds.
     */
    int64_t ChargeTime() const;

    /**
     * @brief Pushes vehicle to charging queue.
     * 
     */
    void NeedsChargedAction();

    /**
     * @brief Formatted string describing this vehicle.
     * 
     * @return const std::string Formatted string describing this vehicle.
     */
    const std::string ToString() const;

    /**
     * @brief Total distance traveled.
     * 
     * @return float 
     */
    float TotalDistance();

    /**
     * @brief Prints vehicle statistics to console.
     * 
     */
    virtual void PrintStats() override;
    
    //
    // SimulationThread overrides
    // 

    /**
     * @brief Thread of execution to run.  Simulates a vehicle 
     *        and all it's states.  Is a producer of the shared 
     *        vehicle queue (thread-safe).
     * 
     */
    virtual void Run() override;

    //
    // Properties
    //

    /**
     * @brief Cruising duration converted to simulation time (seconds).
     * 
     */
    StopWatch CruisingTime;

    /**
     * @brief Charging duration converted to simulation time (seconds).
     * 
     */
    StopWatch ChargingTime;

    /**
     * @brief Queueing duration converted to simulation time (seconds).
     * 
     */
    StopWatch QingTime;

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

    //
    // Properties
    //

    const uint16_t _battery_capacity;
    const uint16_t _cruise_speed;
    const float _energy_use_at_cruise;
    const std::string _header;
    const uint16_t _id; 
    const std::string _name;
    const uint16_t _passenger_count; 
    const float _prob_of_fault;
    const float _time_to_charge;

    /**
     * @brief Vehicle charging queue.
     * 
     */
    TLockedQueue<std::shared_ptr<Vehicle>>& _charging_q;

    /**
     * @brief Current state of vehicle.
     * 
     */
    VehicleStateType _state;
};

typedef TLockedQueue<std::shared_ptr<Vehicle>> ChargingQ;

#endif