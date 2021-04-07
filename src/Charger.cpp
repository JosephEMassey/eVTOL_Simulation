#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <math.h>
#include <sstream>

#include "Charger.h"

Charger::Charger(const unsigned int num_chargers, 
                 TLockedQueue<std::shared_ptr<Vehicle>>& vehicle_charging_q)
                                                                               
{
    std::vector<std::thread> charger_threads;
    for(unsigned int i = 0; i < num_chargers; ++i)
        charger_threads.push_back(std::thread(Run, std::ref(vehicle_charging_q), i));

    for(auto& t : charger_threads)
        t.detach();
}

void Charger::Run(TLockedQueue<std::shared_ptr<Vehicle>>& chargingQ, 
                  const unsigned int& charger_num)
{
    std::stringstream msg;
    msg << "Charger #" << charger_num << " running." << std::endl;
    std::cout << msg.str();

    while(true)
    {
        std::shared_ptr<Vehicle> v = chargingQ.dequeue();

        // Time to charge converted from realtime (mins) to sim time (secs)
        int64_t ttc = ceil(v->time_to_charge * 60);
        std::stringstream msg;
        msg << "<Charger #" << charger_num << "> Charging " << v->name << " for " << ttc << " mins" << std::endl;
        std::cout << msg.str();
        std::this_thread::sleep_for(std::chrono::seconds(ttc));
        msg.str("");
        msg << "<Charger #" << charger_num << "> Charged " << v->name << std::endl;
        std::cout << msg.str();
        v->state = VehicleStateType::CHARGED;
    }
}