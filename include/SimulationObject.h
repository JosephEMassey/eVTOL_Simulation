#ifndef SIMULATION_OBJECT_H
#define SIMULATION_OBJECT_H

#include <iomanip>
#include <iostream>

#include "SimulationThread.h"

class SimulationObject : public SimulationThread
{
public:

    /**
     * @brief Default Constructor
     * 
     */
    SimulationObject() = default;

    /**
     * @brief Default Copy Constructor (disabled)
     * 
     */
    SimulationObject(const SimulationObject &) = delete;

    /**
     * @brief Assignment operator (disabled)
     * 
     * @return SimulationObject& 
     */
    SimulationObject &operator=(const SimulationObject &) = delete;

    /**
     * @brief Destroy the Simulation Object object
     * 
     */
    virtual ~SimulationObject() = default;

    /**
     * @brief Prints the stats of this object.
     * 
     */
    virtual void PrintStats() = 0;
    
protected:

    /**
     * @brief Header identifier used to identify this object.
     * 
     * @return const std::string 
     */
    virtual const std::string Header() = 0;

    /**
     * @brief Prints stream to console prefixed with timestamp.
     * 
     * @param ss Stream to print to console.
     */
    void PrintToConsole(const std::stringstream& ss)
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
            << Header() << ss.str() << std::endl;

        // Print to console
        std::cout << msg.str();
    }
};

#endif