#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <random>
#include <ratio>
#include <sstream>
#include <thread>
#include <vector>

#include "Simulation.h"
#include "Vehicle.h"

using namespace std::chrono;

/**
 * @brief Construct a new Simulation:: Simulation object
 * 
 * @param num_vehicles 
 * @param num_vehicle_types 
 * @param num_chargers 
 */
Simulation::Simulation(const unsigned short num_vehicles,
                       const unsigned short num_vehicle_types,
                       const unsigned short num_chargers) : _num_vehicles     (num_vehicles),
                                                            _num_vehicle_types(num_vehicle_types),
                                                            _num_chargers     (num_chargers)
{

}

/**
 * @brief Creates vehicles and chargers simulation objects.  
 * 
 */
void Simulation::Create()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, _num_vehicle_types-1);

    // Create N random vehicles from M types
    for(int i = 0; i < _num_vehicles; ++i)
        _sim_objs.push_back(Vehicle::Create(static_cast<VehicleType>(distr(gen)), i, _vehicle_charging_q));

    // Create chargers
    for(int i = 0; i < _num_chargers; ++i)
        _sim_objs.push_back(std::make_unique<Charger>(i, _vehicle_charging_q));
}

void Simulation::Run(const int64_t secs)
{
    std::cout << "Starting simulation ... \n";

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    // Start simulation
    for(auto& so : _sim_objs)
        so->Start();

    // Run simulation for secs
    std::this_thread::sleep_for(std::chrono::seconds(secs));

    std::cout << "Stopping simulation ...\n";

    // Stop simulation
    for(auto& so : _sim_objs)
        so->Stop();

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Simulation ran for " << time_span.count() << " seconds.\n";

    std::cout << "Calculating statistics ...\n";

    for(auto& so : _sim_objs)
        so->PrintStats();
}