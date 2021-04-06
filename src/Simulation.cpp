#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <thread>

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

    for(auto& a : _vehicles)
        std::cout << a->to_string() << std::endl;
}

void Simulation::Start()
{
    _charger = std::make_shared<Charger>(3, _vehicle_charging_q);

    // Vehicles
    std::vector<std::thread> vehicle_threads;
    for(auto& v : _vehicles)
        vehicle_threads.push_back(std::thread(Run, std::cref(v), std::ref(_vehicle_charging_q)));

    for(auto& v : vehicle_threads)
        v.join();
}

void Simulation::Run(const std::shared_ptr<Vehicle>& v,
                     TLockedQueue<Vehicle>&          chargingQ)
{
    while(true)
    {
        switch(v->state)
        {
            case VehicleStateType::NEEDS_CHARGED:
                v->state = VehicleStateType::CHARGING;
                chargingQ.enqueue(*v);
                break;

            case VehicleStateType::CHARGED:
                v->state = VehicleStateType::CRUISING;
                break;

            case VehicleStateType::CHARGING:
                break;

            case VehicleStateType::CRUISING:
            {
                int64_t cruise_time = v->CruiseTime();
                std::cout << v->name << " cruising for  " << cruise_time << " seconds " << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(cruise_time));
            }
                v->state = VehicleStateType::CHARGING;
                break;

            case VehicleStateType::EXIT:
                return;

            default:
                break;
        }
        // auto start = std::chrono::system_clock::now();
        // auto end = std::chrono::system_clock::now();
        // std::chrono::duration<double> elapsed_seconds = end-start;
        // std::cout << v->name << " elapsed time: " << elapsed_seconds.count() << std::endl;
    }

    std::cout << v->name << " EXITING " << std::endl;
}