#include "Charger.h"

/**
 * @brief Construct a new Charger object
 * 
 * @param id Charger identification.
 * @param charging_q Vehicle charging queue.
 */
Charger::Charger(uint16_t   id, 
                 ChargingQ& charging_q) : _charging_q {charging_q},
                                          _header("<Charger " + std::to_string(_id) + "> "),
                                          _id(id)                                                 
{ }

/**
 * @brief String used to uniquely identify this object.
 * 
 * @return const std::string Header used to uniquely identify charger.
 */
const std::string Charger::Header()
{
    return _header;
}

/**
 * @brief Prints charger statistics to console.
 * 
 */
void Charger::PrintStats()
{
    // Charger does not have any stats (yet)
}

/**
 * @brief Consumes shared vehicle queue (thread-safe) of vehicles and charges them.
 * 
 */
void Charger::Run()
{
    std::shared_ptr<Vehicle> v;

    std::stringstream ss;
    ss << "Running...";
    PrintToConsole(ss);

    // Charge vehicles
    while(_thread_state != ThreadState::EXIT)
    {
        if(_charging_q.try_dequeue(v))
        {
            // Vehicle is charging
            v->ChangeState(CHARGING);
            
            // Save vehicle queue start time
            v->QingTime.Tok();

            int64_t ttc = v->ChargeTime();

            ss.str("");
            ss << "Charging Vehicle " << v->Name() << " for " << ttc << " mins";
            PrintToConsole(ss);

            // Save vehicle charge start time
            v->ChargingTime.Tik();

            // Blocks for desired seconds OR thread exits
            WaitFor(std::chrono::seconds(ttc));

            // Save vehicle charge stop time
            v->ChargingTime.Tok();

            ss.str("");
            ss << "Charged " << v->Name();
            PrintToConsole(ss);

            // Vehicle is charged
            v->ChangeState(CHARGED);
        }
    }

    // Need to handle vehicle queue time for vehicles that are currently in 
    // the charging queue when the simulation ends
    while(_charging_q.try_dequeue(v))
        v->QingTime.Tok();
}