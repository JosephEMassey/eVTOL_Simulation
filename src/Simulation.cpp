#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <thread>

#include "Simulation.h"
#include "Vehicle.h"

void Simulation::Create(const unsigned short numVehicles,
                        const unsigned short numVehicleTypes)
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
    std::vector<std::thread> vehicle_threads;
    for(auto& v : _vehicles)
        vehicle_threads.push_back(std::thread(Run, v));

    for(auto& v : vehicle_threads)
        v.join();
}

void Simulation::Run(std::shared_ptr<Vehicle> v)
{
    std::cout << "Thread " << std::this_thread::get_id() << " running " << std::endl;
    std::cout << v->to_string() << std::endl;

    while(true)
    {
        // Fly

        // 1 sec of real time == 1 min of sim time
        int64_t cruise_time = v->CruiseTime();

        std::this_thread::sleep_for (std::chrono::seconds(1));

        // Cruise Speed:         120 mph
        // Battery Capacity:     320 kWh
        // Energy use at Cruise: 1.6 kWh/mile

        // 320 kwh / 1.6 kWh/mile = 200 miles
        // 200 miles / 120 mph = 1.666667 hrs
        // 1.666667 hrs == 100 mins
        // 100 mins == 6000 secs

        // Vehicle A can fly for 100 mins and then needs charged.
        
    }
}