#include <iomanip>
#include <map>
#include <memory>
#include <random>
#include <vector>

#include "Simulation.h"
#include "Vehicle.h"

using namespace std::chrono;

/**
 * @brief Construct a new Simulation:: Simulation object
 * 
 * @param num_vehicles Number of vehicles to run in simulation.
 * @param num_vehicle_types Number of vehicle types to run in simulation.
 * @param num_chargers Number of chargers to run in simulation.
 */
Simulation::Simulation(const unsigned short num_vehicles,
                       const unsigned short num_vehicle_types,
                       const unsigned short num_chargers) : _num_vehicles     (num_vehicles),
                                                            _num_vehicle_types(num_vehicle_types),
                                                            _num_chargers     (num_chargers)
{

}

/**
 * @brief Creates random vehicles and chargers simulation objects.  
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
        _sim_objs.push_back(std::make_shared<Charger>(i, _vehicle_charging_q));
}

/**
 * @brief Runs the simulation for sim_time_secs. Each second that passes in 
 *        realtime is equivalent to one minute of simulation time, i.e. 180s
 *        of realtime is 3 hours for simulation time.  Prints stats of each
 *        vehicle type.
 * 
 * @param secs Duration (seconds) to run simulation.
 */
void Simulation::Run(const int64_t sim_time_secs) const
{
    std::cout << "Starting simulation ... \n";

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    // Start simulation
    for(auto const& so : _sim_objs)
        so->Start();

    // Run simulation for secs
    std::this_thread::sleep_for(std::chrono::seconds(sim_time_secs));

    std::cout << "Stopping simulation ...\n";

    // Stop simulation
    for(auto const& so : _sim_objs)
        so->Stop();

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Simulation ran for " << time_span.count() << " seconds.\n";

    std::cout << "Calculating statistics ...\n";

    // Stats for each vehicle
    PrintStatsForEachSimObject();

    // Stats for each vehicle type (VehicleA, VehicleB, ...)
    PrintStatsForEachVehicleType(sim_time_secs);
}

/**
 * @brief Prints the stats for each simulation object (Vehicle, Charger).
 * 
 */
void Simulation::PrintStatsForEachSimObject() const
{
    for(auto const& so : _sim_objs)
        so->PrintStats();
}

/**
 * @brief Prints the stats for each vehicle type (VehicleA, VehicleB, ...).
 *          * Avg Flight Time (mins)
 *          * Flight Time (%)
 *          * Avg Charge Time (mins)
 *          * Charge Time (%)
 *          * Avg Qing Time (mins)
 *          * Qing Time (%)
 *          * Max Faults
 * 
 * @param sim_time_secs Duration (seconds) to run simulation.
 */
void Simulation::PrintStatsForEachVehicleType(const int64_t sim_time_secs) const
{
    // Ideally I'd save the results to a file in a known format (i.e. csv) 
    // and have some external tool plot/graph/display results.

    // Find all Vehicle objects (downcast)
    std::map<std::string, std::vector<std::shared_ptr<Vehicle>>> vehicle_stats;
    for(auto const& so : _sim_objs)
    {
        std::shared_ptr<Vehicle> v = std::dynamic_pointer_cast<Vehicle>(so);
        if(v)
            vehicle_stats[v->Name()].push_back(v);
    }

    std::cout << "\n\nTotal Simulation Time: " << sim_time_secs << " mins" << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "|  Vehicle  |  Num Vehicles  |  Avg Flight Time (mins)  |  Flight Time (%)  |  Avg Charge Time (mins)  |  Charge Time (%)  |  Avg Qing Time (mins)  |  Qing Time (%)  |  Max Faults  |" << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

    // Calculate all vehicle stats and prints to console
    for(auto const& [key, val] : vehicle_stats)
    {
        int64_t total_cruise = 0;
        int64_t total_charge = 0;
        int64_t total_q      = 0;

        for(auto const& vehicle : val)
        {
            total_cruise += vehicle->CruisingTime.Total();
            total_charge += vehicle->ChargingTime.Total();
            total_q      += vehicle->QingTime.Total();
        }

        float probability_fault = val[0]->ProbabilityOfFault();

        int64_t num_vehicles_in_sim = val.size();

        int64_t avg_cruise_mins = total_cruise / num_vehicles_in_sim;
        int64_t avg_charge_mins = total_charge / num_vehicles_in_sim;
        int64_t avg_q_mins      = total_q      / num_vehicles_in_sim;

        float max_num_faults    = float(sim_time_secs / 60.0f) * probability_fault * num_vehicles_in_sim;

        float cruise_percentage = float(total_cruise) / float(sim_time_secs * num_vehicles_in_sim) * 100.0f;
        float charge_percentage = float(total_charge) / float(sim_time_secs * num_vehicles_in_sim) * 100.0f;
        float q_percentage      = float(total_q)      / float(sim_time_secs * num_vehicles_in_sim) * 100.0f;

        std::cout << std::setprecision(2) << std::fixed;
        std::cout << "|"   << std::right << std::setw(9) << std::setfill(' ') << key;
        std::cout << "  |" << std::setw(14) << num_vehicles_in_sim;
        std::cout << "  |" << std::setw(24) << avg_cruise_mins;
        std::cout << "  |" << std::setw(17) << cruise_percentage;
        std::cout << "  |" << std::setw(24) << avg_charge_mins;
        std::cout << "  |" << std::setw(17) << charge_percentage;
        std::cout << "  |" << std::setw(22) << avg_q_mins;
        std::cout << "  |" << std::setw(15) << q_percentage;
        std::cout << "  |" << std::setw(12) << max_num_faults;
        std::cout << "  |" << std::endl;
        std::cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
    }
}