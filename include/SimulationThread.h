#ifndef SIMULATION_THREAD_H
#define SIMULATION_THREAD_H

#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

/**
 * @brief State of thread.
 * 
 */
enum ThreadState
{
    RUNNING,
    WAIT,
    EXIT
};

/**
 * @brief Wrapper for std::thread for convenience.
 * 
 */
class SimulationThread
{
public:

    /**
     * @brief Default Constructor.
     * 
     */
    SimulationThread() = default;

    /**
     * @brief Default Copy Constructor (disabled).
     * 
     */
    SimulationThread(const SimulationThread &) = delete;

    /**
     * @brief Assignment operator (disabled).
     * 
     * @return SimulationThread& 
     */
    SimulationThread &operator=(const SimulationThread &) = delete;

    /**
     * @brief Destroy the Simulation Thread object.
     * 
     */
    virtual ~SimulationThread() = default;

    /**
     * @brief Joins the current thread and will return once the thread execution has completed.
     * 
     */
    virtual void Join();

    /**
     * @brief Thread of execution to run.
     * 
     */
    virtual void Run();

    /**
     * @brief Starts the thread.
     * 
     */
    virtual void Start();
    
    /**
     * @brief Stops the thread.
     * 
     */
    virtual void Stop();

    /**
     * @brief Blocks thread for duration OR signaled to exit.
     * 
     * @tparam Duration 
     * @param duration Maximum time span during which the thread
     *                 will block waiting to be notified.
     * @return true  Thread was signaled to exit.
     * @return false Thread completed the duration. 
     */
    template<class Duration> bool WaitFor(Duration duration);

protected:

    /**
     * @brief A single thread of execution.
     * 
     */
    std::unique_ptr<std::thread> _thread;

    /**
     * @brief Current state of thread.
     * 
     */
    ThreadState _thread_state;
    
    /**
     * @brief Locks access and used to exit thread.
     * 
     */
    std::mutex _cs;

    /**
     * @brief Signals threads to exit.
     * 
     */
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