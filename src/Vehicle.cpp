#include <future>
#include <iomanip>
#include <iostream>
#include <random>

#include "Vehicle.h"

Vehicle::Vehicle(const std::string& n,
                 const uint16_t     bc,
                 const uint16_t     cs,
                 const uint16_t     pc,
                 const float        eac,
                 const float        pof,
                 const float        ttc,
                 const uint16_t     id,
                 TLockedQueue<std::shared_ptr<Vehicle>>& chargingQ)  
                                            : _name                (n),
                                              _battery_capacity    (bc),
                                              _cruise_speed        (cs),
                                              _passenger_count     (pc),
                                              _energy_use_at_cruise(eac),
                                              _prob_of_fault       (pof),
                                              _time_to_charge      (ttc),
                                              _id                  (id),
                                              _charging_q { chargingQ },
                                              _state(INITIAL)
{ 

}

/**
 * @brief 
 * 
 * @return const std::string 
 */
const std::string Vehicle::ToString() const
{   
    std::stringstream ss;
    ss << "Name:                 " << _name             << std::endl;
    ss << "Cruise Speed:         " << _cruise_speed     << std::endl;
    ss << "Battery Capacity:     " << _battery_capacity << std::endl;
    ss << "Time to Charge:       " << _time_to_charge   << std::endl;
    ss << "Passenger Count:      " << _passenger_count  << std::endl;
    ss << "Probability of Fault: " << _prob_of_fault    << std::endl;
    return ss.str();
}

std::shared_ptr<Vehicle> Vehicle::Create(VehicleType type, 
                                         const unsigned short id,
                                         TLockedQueue<std::shared_ptr<Vehicle>>& chargingQ)
{
    switch(type)
    {
        case VehicleType::A:
            return std::make_shared<Vehicle>("A", 320, 120, 4, 1.6, 0.25, 0.60, id, chargingQ);
        case VehicleType::B:
            return std::make_shared<Vehicle>("B", 100, 100, 5, 1.5, 0.10, 0.20, id, chargingQ);
        case VehicleType::C:
            return std::make_shared<Vehicle>("C", 220, 160, 3, 2.2, 0.05, 0.80, id, chargingQ);
        case VehicleType::D:
            return std::make_shared<Vehicle>("D", 120,  90, 2, 0.8, 0.22, 0.62, id, chargingQ);
        case VehicleType::E:
            return std::make_shared<Vehicle>("E", 150,  30, 2, 5.8, 0.61, 0.30, id, chargingQ);
        default:
            return nullptr;
    }
}

/**
 * @brief Calculates the cruise time in seconds converted to simulation time.
 * 
 * @return int64_t Cruise time in seconds.
 */
int64_t Vehicle::CruiseTime() const
{


    return floor(_battery_capacity / _energy_use_at_cruise / _cruise_speed * 60);
}

/**
 * @brief Calculates the time to charge in seconds converted to simulation time.
 * 
 * @return int64_t Time to Charge in seconds.
 */
int64_t Vehicle::TimeToCharge() const
{
    return ceil(_time_to_charge * 60);
}

/**
 * @brief 
 * 
 */
void Vehicle::Run()
{   
    std::stringstream ss;
    ss << "Running...";
    print(ss);
    
    while(_thread_state != ThreadState::EXIT)
    {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        switch(_state)
        {
            case INITIAL:
                ChangeState(CRUISING);
                break;

            case NEEDS_CHARGED:
                NeedsCharged();
                ChangeState(CHARGING);
                break;

            case CHARGED:
                ChangeState(CRUISING);
                break;

            case CRUISING:
                Cruise();
                ChangeState(NEEDS_CHARGED);
                break;
            
            case CHARGING:
            default:
                break; 
        }
    }
}

/**
 * @brief 
 * 
 * @param ss 
 */
void Vehicle::print(const std::stringstream& ss) const
{
    tm localTime;
    std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
    time_t now = std::chrono::system_clock::to_time_t(t);
    localtime_r(&now, &localTime);

    const std::chrono::duration<double> tse = t.time_since_epoch();
    std::chrono::seconds::rep milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(tse).count() % 1000;

    std::stringstream msg;
    msg << "["
        << std::setfill('0') << std::setw(2) << localTime.tm_hour << ':'
        << std::setfill('0') << std::setw(2) << localTime.tm_min  << ':'
        << std::setfill('0') << std::setw(2) << localTime.tm_sec  << '.'
        << std::setfill('0') << std::setw(3) << milliseconds
        << "] "
        << "<Vehicle " << _name << _id << "> " << ss.str() << std::endl;

    // Print to console
    std::cout << msg.str();
}

void Vehicle::ChangeState(VehicleStateType state)
{
    _state = state;
}

void Vehicle::Cruise()
{
    int64_t cruise_time = CruiseTime();

    std::stringstream ss;
    ss << "Cruising for " << cruise_time << " mins";
    print(ss);

    _vehicle_cruising_time_start = std::chrono::steady_clock::now();

    // Blocks for desired seconds OR thread exits
    WaitFor(std::chrono::seconds(cruise_time));

    _vehicle_cruising_time_end    = std::chrono::steady_clock::now();
    _vehicle_cruising_time_total += std::chrono::duration_cast<std::chrono::seconds>(_vehicle_cruising_time_end - _vehicle_cruising_time_start);
    
}

void Vehicle::NeedsCharged()
{
    _vehicle_in_q_time_start = std::chrono::steady_clock::now();

    // Add this vehicle to the charging queue
    _charging_q.enqueue(shared_from_this());
}

void Vehicle::PrintStats()
{
    // Print stats
    /* average time in flight                 = Total Cruising Time / Simulation Time
       average time charging                  = Total Charging Time / Simulation Time
       average time waiting in line to charge = Total Time In Q     / Simulation Time 
       max number of faults                   = Probability of fault/hr * 60 * Simulation Time
       total distance traveled by passengers  = Passenger Count * Total Cruising Time * Cruise Speed

                                                4 * 120 mph * (60 m / 60) = 480    miles
                                                5 * 100 mph * (74 m / 60) = 616.67 miles

    */

    std::stringstream output;
    output << "Total Cruising Time: "  << _vehicle_cruising_time_total.count() << " mins\t"
           << "Total Charging Time: "  << _vehicle_charging_time_total.count() << " mins\t"
           << "Total Q Time: "         << _vehicle_in_q_time_total.count()     << " mins\t"
           << "Total Distance: "       << _passenger_count * _cruise_speed * float(_vehicle_cruising_time_total.count()) / 60 << " miles\t";
           //<< "Probability of Fault: " << prob_of_fault * (180 / 60);

    print(output);
}
