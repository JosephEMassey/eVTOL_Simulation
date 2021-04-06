#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <sstream>
#include <memory>

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
    CRUISING,
    NEEDS_CHARGED,
    CHARGING,
    CHARGED,
    EXIT
};

class Vehicle
{
    public:
        std::string name;
        unsigned int cruise_speed;
        unsigned int battery_capacity;
        unsigned short passenger_count;
        float time_to_charge;
        float energy_use_at_cruise;
        float prob_of_fault;

        VehicleStateType state;

    public:
        Vehicle() = default;
        Vehicle( const std::string& n, 
                 const unsigned int cs,
                 const unsigned int bc,
                 const unsigned short pc,
                 const float ttc,
                 const float eac,
                 const float pof );
        virtual ~Vehicle() = default;

        int64_t CruiseTime();

        const std::string to_string();

        static std::shared_ptr<Vehicle> Create(VehicleType type);
};

#endif