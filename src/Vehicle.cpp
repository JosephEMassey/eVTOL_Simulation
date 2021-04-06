#include <random>

#include "Vehicle.h"

Vehicle::Vehicle( const std::string&   n, 
                  const unsigned int   cs,
                  const unsigned int   bc,
                  const unsigned short pc,
                  const float          ttc,
                  const float          eac,
                  const float          pof)  
                                            : name                (n),
                                              cruise_speed        (cs),
                                              battery_capacity    (bc),
                                              passenger_count     (pc),
                                              time_to_charge      (ttc),
                                              energy_use_at_cruise(eac),
                                              prob_of_fault       (pof),
                                              state(VehicleStateType::CRUISING)
{ }

const std::string Vehicle::to_string()
{   
    std::stringstream ss;
    ss << "Name:                 " << name             << std::endl;
    ss << "Cruise Speed:         " << cruise_speed     << std::endl;
    ss << "Battery Capacity:     " << battery_capacity << std::endl;
    ss << "Time to Charge:       " << time_to_charge   << std::endl;
    ss << "Passenger Count:      " << passenger_count  << std::endl;
    ss << "Probability of Fault: " << prob_of_fault    << std::endl;
    return ss.str();
}

std::shared_ptr<Vehicle> Vehicle::Create(VehicleType type)
{
    switch(type)
    {
        case VehicleType::A:
            return std::make_unique<Vehicle>("A", 120, 320, 4, 0.60, 1.6, 0.25);
        case VehicleType::B:
            return std::make_unique<Vehicle>("B", 100, 100, 5, 0.20, 1.5, 0.10);
        case VehicleType::C:
            return std::make_unique<Vehicle>("C", 160, 220, 3, 0.80, 2.2, 0.05);
        case VehicleType::D:
            return std::make_unique<Vehicle>("D",  90, 120, 2, 0.62, 0.8, 0.22);
        case VehicleType::E:
            return std::make_unique<Vehicle>("E",  30, 150, 2, 0.30, 5.8, 0.61);
        default:
            return nullptr;
    }
}

/**
 * @brief Calculates the cruise time in seconds converted to simulation time.
 * 
 * @return Cruise time in seconds.
 */
int64_t Vehicle::CruiseTime()
{
    // Cruise Speed:         120 mph
    // Battery Capacity:     320 kWh
    // Energy use at Cruise: 1.6 kWh/mile
    // 320 kwh / 1.6 kWh/mile = 200 miles
    // 200 miles / 120 mph = 1.666667 hrs
    // 1.666667 hrs == 100 mins
    // Vehicle A can fly for 100 mins (100s sim time) and then needs charged.

    return floor(battery_capacity / energy_use_at_cruise / cruise_speed * 60);
}