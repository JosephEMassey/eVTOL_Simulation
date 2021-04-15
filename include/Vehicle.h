#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <sstream>
#include <memory>

#include "SimulationObject.h"
#include "TLockedQueue.h"

enum VehicleType 
{
    A = 0,
    B,
    C,
    D,
    E,
};

enum VehicleStateType
{
    INITIAL,
    CRUISING,
    NEEDS_CHARGED,
    CHARGING,
    CHARGED
};



class Vehicle : public SimulationObject, 
                public std::enable_shared_from_this<Vehicle>
{

public:

    // SimulationObject
    //virtual void Start() override;
    //virtual void Stop() override;
    
    virtual void Run()        override;
    virtual void PrintStats() override;

    // States
    void NeedsCharged();
    void Cruise();
    void ChangeState(VehicleStateType state);

    int64_t CruiseTime()   const;
    int64_t TimeToCharge() const;

    std::string Name()  const { return _name; }
    unsigned short ID() const { return _id;   }

    const std::string ToString() const;

public:

    Vehicle() = default;
    Vehicle(const std::string& n, 
            const uint16_t     bc,
            const uint16_t     cs,
            const uint16_t     pc,
            const float        eac,
            const float        pof,
            const float        ttc,
            const uint16_t     id,
            TLockedQueue<std::shared_ptr<Vehicle>>& chargingQ);

    virtual ~Vehicle() = default;

    static std::shared_ptr<Vehicle> Create(VehicleType type, 
                                           const unsigned short id,
                                           TLockedQueue<std::shared_ptr<Vehicle>>& chargingQ);

    void print(const std::stringstream& ss) const;

    std::chrono::steady_clock::time_point _vehicle_cruising_time_start;
    std::chrono::steady_clock::time_point _vehicle_cruising_time_end;
    std::chrono::seconds _vehicle_cruising_time_total;

    std::chrono::steady_clock::time_point _vehicle_charging_time_start;
    std::chrono::steady_clock::time_point _vehicle_charging_time_end;
    std::chrono::seconds _vehicle_charging_time_total;

    std::chrono::steady_clock::time_point _vehicle_in_q_time_start;
    std::chrono::steady_clock::time_point _vehicle_in_q_time_end;
    std::chrono::seconds _vehicle_in_q_time_total;

private:

    

protected:

    const std::string _name;

    const unsigned short _battery_capacity;
    const unsigned short _cruise_speed;
    const unsigned short _passenger_count; 

    const float _energy_use_at_cruise;
    const float _prob_of_fault;
    const float _time_to_charge;

    const unsigned short _id;    

    TLockedQueue<std::shared_ptr<Vehicle>>& _charging_q;

    VehicleStateType _state;
};

typedef TLockedQueue<std::shared_ptr<Vehicle>> ChargingQ;

#endif