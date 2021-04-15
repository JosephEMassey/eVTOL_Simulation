#include <iostream>
#include <iomanip>

#include "Charger.h"

Charger::Charger(const unsigned int id, 
                 ChargingQ& charging_q) : _id(id),
                                          _charging_q {charging_q}                                                           
{ }

void Charger::Run()
{
    std::shared_ptr<Vehicle> v;

    std::stringstream ss;
    ss << "Running...";
    print(ss);

    while(_thread_state != ThreadState::EXIT)
    {
        if(_charging_q.try_dequeue(v))
        {
            v->ChangeState(CHARGING);

            v->_vehicle_in_q_time_end    = std::chrono::steady_clock::now();
            v->_vehicle_in_q_time_total += std::chrono::duration_cast<std::chrono::seconds>(v->_vehicle_in_q_time_end - 
                                                                                            v->_vehicle_in_q_time_start);
            int64_t ttc = v->TimeToCharge();

            ss.str("");
            ss << "Charging Vehicle " << v->Name() << " for " << ttc << " mins";
            print(ss);

            v->_vehicle_charging_time_start = std::chrono::steady_clock::now();

            // Blocks for desired seconds OR thread exits
            WaitFor(std::chrono::seconds(ttc));

            v->_vehicle_charging_time_end    = std::chrono::steady_clock::now();
            v->_vehicle_charging_time_total += std::chrono::duration_cast<std::chrono::seconds>(v->_vehicle_charging_time_end - v->_vehicle_charging_time_start);

            ss.str("");
            ss << "Charged " << v->Name();
            print(ss);

            v->ChangeState(CHARGED);
        }
    }

    // Need to handle vehicle queue time for vehicles that are currently in 
    // the charging queue when the simulation ends
    while(_charging_q.try_dequeue(v))
    {
        v->_vehicle_in_q_time_end    = std::chrono::steady_clock::now();
        v->_vehicle_in_q_time_total += std::chrono::duration_cast<std::chrono::seconds>(v->_vehicle_in_q_time_end - 
                                                                                        v->_vehicle_in_q_time_start);
    }
}   

/**
 * @brief 
 * 
 * @param ss 
 */
void Charger::print(const std::stringstream& ss)
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
        << "<Charger " << _id << "> " << ss.str() << std::endl;

    // Print to console
    std::cout << msg.str();
}

void Charger::PrintStats()
{
    // Charger does not have any stats (yet)
}