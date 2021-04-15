#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>

class StopWatch
{
public:

    void Tik();
    void Tok();
    void Reset();
    constexpr int64_t Total();

private:
    std::chrono::steady_clock::time_point _start;
    std::chrono::steady_clock::time_point _stop;
    std::chrono::seconds _total;
};

/**
 * @brief Starts stopwatch.
 * 
 */
inline void StopWatch::Tik()
{
    _start = std::chrono::steady_clock::now();
}

/**
 * @brief Stops stopwatch.
 * 
 */
inline void StopWatch::Tok()
{
    _stop   = std::chrono::steady_clock::now();
    _total += std::chrono::duration_cast<std::chrono::seconds>(_stop - _start); 
}

/**
 * @brief Reset stopwatch.
 * 
 */
inline void StopWatch::Reset()
{
    _start = {};
    _stop  = {};
    _total = {};
}

/**
 * @brief Total duration.
 * 
 * @return constexpr int64_t 
 */
inline constexpr int64_t StopWatch::Total()
{
    return _total.count();
}

#endif