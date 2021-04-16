#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>

#include "Charger.h"
#include "TLockedQueue.h"
#include "Vehicle.h"

/**
 * @brief Main application that runs the simulation.  The simulation consists
 *        of running n number of Vehicles with m number of chargers for a requested
 *        duration.  Results will be printed to console once complete.
 * 
 */
class Simulation
{
public:

    /**
     * @brief Construct a new Simulation object.
     * 
     * @param num_vehicles Number of vehicles to run in simulation.
     * @param num_vehicle_types Number of vehicle types.
     * @param num_chargers Number of chargers to run in simulation.
     */
    Simulation(const unsigned short num_vehicles,
               const unsigned short num_vehicle_types,
               const unsigned short num_chargers);
 
    /**
     * @brief Default Constructor (disabled).
     * 
     */
    Simulation() = delete;

    /**
     * @brief Default Copy Constructor (disabled).
     * 
     */
    Simulation(const Simulation &) = delete;

    /**
     * @brief Assignment operator (disabled).
     * 
     * @return Simulation& 
     */
    Simulation &operator=(const Simulation &) = delete;

    /**
     * @brief Destroy the Simulation object.
     * 
     */
    virtual ~Simulation() = default;

    /**
     * @brief Creates random vehicles and chargers simulation objects. 
     * 
     */
    size_t Create();

    /**
     * @brief Prints the stats for each simulation object (Vehicle, Charger) to the console.
     * 
     */
    void PrintStatsForEachSimObject() const;

    /**
     * @brief Calculates and prints stats for each vehicle type (VehicleA, VehicleB, ...).
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
    void PrintStatsForEachVehicleType(const int64_t sim_time_secs) const;

    /**
     * @brief Runs the simulation for sim_time_secs. Each second that passes in 
     *        realtime is equivalent to one minute of simulation time, i.e. 180s
     *        of realtime is 3 hours for simulation time.  Prints stats of each
     *        vehicle type.
     * 
     * @param sim_time_secs Duration (seconds) to run simulation.
     */
    void Run(const int64_t sim_time_secs) const;

private:

    /**
     * @brief Number of chargers to run in simulation.
     * 
     */
    const unsigned short _num_chargers;

    /**
     * @brief Number of vehicles to run in simulation.
     * 
     */
    const unsigned short _num_vehicles;

    /**
     * @brief Number of vehicle types.
     * 
     */
    const unsigned short _num_vehicle_types;

    /**
     * @brief Simulation objects that will run in simulation.
     * 
     */
    std::vector<std::shared_ptr<SimulationObject>> _sim_objs;

    /**
     * @brief Vehicle charging queue.
     * 
     */
    ChargingQ _vehicle_charging_q;
};

#endif