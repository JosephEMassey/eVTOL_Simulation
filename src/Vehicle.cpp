#include <random>

#include "Vehicle.h"

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
Vehicle::Vehicle(const std::string& n,
                 const uint16_t     bc,
                 const uint16_t     cs,
                 const uint16_t     pc,
                 const float        eac,
                 const float        pof,
                 const float        ttc,
                 const uint16_t     id,
                 ChargingQ& chargingQ) : _battery_capacity    (bc),
                                         _cruise_speed        (cs),
                                         _energy_use_at_cruise(eac),
                                         _header("<Vehicle " + n + std::to_string(id) + "> "),
                                         _id                  (id),
                                         _name                (n),
                                         _passenger_count     (pc),
                                         _prob_of_fault       (pof),
                                         _time_to_charge      (ttc),
                                         _charging_q { chargingQ },
                                         _state(INITIAL)
{ 

}

/**
 * @brief Creates an instance of the requested Vehicle using Factory of vehicles.
 * 
 * @param type 
 * @param id 
 * @param chargingQ 
 * @return std::shared_ptr<Vehicle> 
 */
std::shared_ptr<Vehicle> Vehicle::Create(VehicleType type, 
                                         const uint16_t id,
                                         ChargingQ& chargingQ)
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
 * @brief Changes the current state of this vehicle.
 * 
 * @param state State to switch to.
 */
void Vehicle::ChangeState(VehicleStateType state)
{
    _state = state;
}

/**
 * @brief Simulates a vehicle cruising by blocking thread for CruiseTime().
 * 
 */
void Vehicle::CruiseAction()
{
    int64_t cruise_time = CruiseTime();

    std::stringstream ss;
    ss << "Cruising for " << cruise_time << " mins";
    PrintToConsole(ss);

    CruisingTime.Tik();

    // Blocks for desired seconds OR thread exits
    WaitFor(std::chrono::seconds(cruise_time));

    CruisingTime.Tok();
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
int64_t Vehicle::ChargeTime() const
{
    return ceil(_time_to_charge * 60);
}

/**
 * @brief Pushes vehicle to charging queue.
 * 
 */
void Vehicle::NeedsChargedAction()
{
    // Save vehicle queueing start time
    QingTime.Tik();

    // Add this vehicle to the charging queue
    _charging_q.enqueue(shared_from_this());
}

/**
 * @brief Formatted string describing this vehicle.
 * 
 * @return const std::string Formatted string describing this vehicle.
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

/**
 * @brief Total distance traveled.
 * 
 * @return float 
 */
float Vehicle::TotalDistance()
{
    return PassengerCount() * CruiseSpeed() * float(CruisingTime.Total()) / 60;
}

//
// Simulation overrides
//

/**
 * @brief String used to uniquely identify this object.
 * 
 * @return const std::string Header used to uniquely identify charger.
 */
const std::string Vehicle::Header()
{
    return _header;
}

/**
 * @brief Prints vehicle statistics to console.
 * 
 */
void Vehicle::PrintStats()
{
    std::stringstream output;
    output << "\n-----------------------------------------------------------------------------------------------------------------" << std::endl;
    output << "|  Total Flight Time (mins)  |  Total Charge Time (mins)  |  Total Qing Time (mins)  |  Total Distance (miles)  |" << std::endl;
    output << "-----------------------------------------------------------------------------------------------------------------" << std::endl;
    
    output << std::setprecision(2) << std::fixed;
    output << "|"    << std::right << std::setw(25) << std::setfill(' ') << CruisingTime.Total();
    output << "   |" << std::setw(25) << ChargingTime.Total();
    output << "   |" << std::setw(23) << QingTime.Total();
    output << "   |" << std::setw(23) << TotalDistance();
    output << "   |" << std::endl;
    output << "-----------------------------------------------------------------------------------------------------------------" << std::endl;
    
    // output << "Total Cruising Time: "  << CruisingTime.Total() << " mins\t"
    //        << "Total Charging Time: "  << ChargingTime.Total() << " mins\t"
    //        << "Total Q Time: "         << QingTime.Total()     << " mins\t"
    //        << "Total Distance: "       << TotalDistance()      << " miles\t";
    //        //<< "Probability of Fault: " << prob_of_fault * (180 / 60);

    PrintToConsole(output);
}

//
// SimulationThread overrides
// 

/**
 * @brief Thread of execution to run.  Simulates a vehicle 
 *        and all its states.  Is a producer of the shared 
 *        vehicle queue (thread-safe).
 * 
 */
void Vehicle::Run()
{   
    std::stringstream ss;
    ss << "Running...";
    PrintToConsole(ss);
    
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
                NeedsChargedAction();
                ChangeState(CHARGING);
                break;

            case CHARGED:
                ChangeState(CRUISING);
                break;

            case CRUISING:
                CruiseAction();
                ChangeState(NEEDS_CHARGED);
                break;
            
            case CHARGING:
            default:
                break; 
        }
    }
}