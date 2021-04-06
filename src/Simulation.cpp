#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <thread>
#include <sstream>

#include "Simulation.h"
#include "Vehicle.h"

Simulation::Simulation()
{

}

void Simulation::Create(const unsigned short numVehicles,
                        const unsigned short numVehicleTypes,
                        const unsigned short numChargers)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, numVehicleTypes-1);

    // Create random vehicles
    for(int i = 0; i < numVehicles; i++)
        _vehicles.push_back(Vehicle::Create(static_cast<VehicleType>(distr(gen))));

    // Create chargers
    _charger = std::make_shared<Charger>(numChargers, _vehicle_charging_q);
}

void Simulation::Start()
{
    // Vehicles
    std::vector<std::thread> vehicle_threads;
    for(auto& v : _vehicles)
        vehicle_threads.push_back(std::thread(Run, std::cref(v), std::ref(_vehicle_charging_q)));

    for(auto& v : vehicle_threads)
        v.join();
}

void Simulation::Run(const std::shared_ptr<Vehicle>& v,
                     TLockedQueue<std::shared_ptr<Vehicle>>& chargingQ)
{
    while(true)
    {
        switch(v->state)
        {
            case VehicleStateType::NEEDS_CHARGED:
                v->state = VehicleStateType::CHARGING;
                chargingQ.enqueue(v);
                break;

            case VehicleStateType::CHARGED:
                v->state = VehicleStateType::CRUISING;
                break;

            case VehicleStateType::CHARGING:
                break;

            case VehicleStateType::CRUISING:
            {
                int64_t cruise_time = v->CruiseTime();
                std::stringstream msg;
                msg << "<Vehicle " << v->name << ">" << " cruising for " << cruise_time << " mins" << std::endl;
                std::cout << msg.str();
                std::this_thread::sleep_for(std::chrono::seconds(cruise_time));
            }
                v->state = VehicleStateType::NEEDS_CHARGED;
                break;

            case VehicleStateType::EXIT:
                return;

            default:
                break;
        }
    }

    std::stringstream msg;
    msg << v->name << " EXITING " << std::endl;
    std::cout << msg.str();
}