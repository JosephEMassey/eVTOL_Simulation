#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <math.h>

#include "Charger.h"

Charger::Charger(const unsigned int num_chargers, 
                 TLockedQueue<Vehicle>& vehicle_charging_q) 
{
    // Start num_chargers threads to handle

    // create a thread that simply pops the q and delays the time_to_charge amout of time, then 
    // changes the vehicle state to VehicleStateType::CHARGED

    std::vector<std::thread> charger_threads;
    for(unsigned int i = 0; i < num_chargers; ++i)
    {
        //charger_threads.push_back(std::thread(Run, std::ref(vehicle_charging_q), i));

        charger_threads.push_back(std::thread([&vehicle_charging_q, &i]() {
            while(true)
            {
                Vehicle v = vehicle_charging_q.dequeue();

                // Time to charge converted from realtime (mins) to sim time (secs)
                int64_t ttc = ceil(v.time_to_charge * 60);
                std::cout << "<Charger #" << i << "> Charging " << v.name << " for " << ttc << " mins" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(ttc));
                std::cout << "<Charger #" << i << "> Charged " << v.name << std::endl;
                v.state = VehicleStateType::CHARGED;
            }
        }));
    }

    for(auto& t : charger_threads)
        t.detach();
}

void Charger::Run(TLockedQueue<Vehicle>& chargingQ, const unsigned int& charger_num)
{
    std::cout << "Charger #" << charger_num << " running." << std::endl;

    while(true)
    {
        Vehicle v = chargingQ.dequeue();

        // Time to charge converted from realtime (mins) to sim time (secs)
        int64_t ttc = ceil(v.time_to_charge * 60);
        std::cout << "<Charger #" << charger_num << "> Charging " << v.name << " for " << ttc << " mins" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(ttc));
        std::cout << "<Charger #" << charger_num << "> Charged " << v.name << std::endl;
        v.state = VehicleStateType::CHARGED;
    }
}