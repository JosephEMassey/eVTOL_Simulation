#ifndef SIMULATION_THREAD_H
#define SIMULATION_THREAD_H

#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

enum ThreadState
{
    RUNNING,
    WAIT,
    EXIT
};

class SimulationThread
{
public:

    SimulationThread()                                    = default; // Default CTor
    SimulationThread(const SimulationThread &)            = delete;  // Disable Copy
    SimulationThread &operator=(const SimulationThread &) = delete;  // Disable Assignment

    virtual ~SimulationThread() = default; // Default DTor

    virtual void Join();
    virtual void Run();
    virtual void Start();
    virtual void Stop();

    template<class Duration> bool WaitFor(Duration duration);

protected:

    ThreadState _thread_state;
    std::unique_ptr<std::thread> _thread;

    std::mutex              _cs;
    std::condition_variable _cv;

private:

    static void run(SimulationThread* so);
};

/**
 * @brief Blocks thread for duration OR signaled to exit.
 * 
 * @tparam Duration 
 * @param duration Maximum time span during which the thread
 *                 will block waiting to be notified.
 * @return true  Thread was signaled to exit.
 * @return false Thread completed the duration. 
 */
template<class Duration>
bool SimulationThread::WaitFor(Duration duration)
{
    std::unique_lock<std::mutex> lock(_cs);
    return !_cv.wait_for(lock, duration, [this](){
        return _thread_state == ThreadState::EXIT;
    });
}

#endif