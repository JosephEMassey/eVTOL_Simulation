#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>

/**
 * @brief Simple stopwatch to calculate differences in timepoints.
 * 
 */
class StopWatch
{
public:

    /**
     * @brief Default Constructor.
     * 
     */
    StopWatch() = default;

    /**
     * @brief Default Copy Constructor (disabled).
     * 
     */
    StopWatch(const StopWatch &) = delete;

    /**
     * @brief Assignment operator (disabled).
     * 
     * @return SimulationThread& 
     */
    StopWatch &operator=(const StopWatch &) = delete;

    /**
     * @brief Destroy the StopWatch object
     * 
     */
    virtual ~StopWatch() = default;

    /**
     * @brief Reset StopWatch.
     * 
     */
    void Reset();

    /**
     * @brief Start Stopwatch.
     * 
     */
    void Tik();

    /**
     * @brief Stop Stopwatch.
     * 
     */
    void Tok();
    
    /**
     * @brief Calculatess total duration of StopWatch.
     * 
     * @return constexpr int64_t Total duration of StopWatch in seconds.
     */
    constexpr int64_t Total();

private:

    /**
     * @brief Start timepoint.
     * 
     */
    std::chrono::steady_clock::time_point _start;

    /**
     * @brief Stop timepoint.
     * 
     */
    std::chrono::steady_clock::time_point _stop;

    /**
     * @brief Durations of start and stop timepoints.
     * 
     */
    std::chrono::seconds _total;
};

/**
 * @brief Reset StopWatch.
 * 
 */
inline void StopWatch::Reset()
{
    _start = {};
    _stop  = {};
    _total = {};
}

/**
 * @brief Start StopWatch.
 * 
 */
inline void StopWatch::Tik()
{
    _start = std::chrono::steady_clock::now();
}

/**
 * @brief Stop StopWatch.
 * 
 */
inline void StopWatch::Tok()
{
    _stop   = std::chrono::steady_clock::now();
    _total += std::chrono::duration_cast<std::chrono::seconds>(_stop - _start); 
}

/**
 * @brief Calculates total duration of StopWatch in seconds.
 * 
 * @return constexpr int64_t Total duration of StopWatch in seconds.
 */
inline constexpr int64_t StopWatch::Total()
{
    return _total.count();
}

#endif