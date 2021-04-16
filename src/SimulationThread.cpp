#include "SimulationThread.h"

/**
 * @brief Joins the current thread and will return once the thread execution has completed.
 * 
 */
void SimulationThread::Join()
{
    if(_thread != nullptr)
    {
        _thread->join();
        _thread = nullptr;
    }
}

/**
 * @brief Thread of execution to run.
 * 
 */
void SimulationThread::Run()
{ }

/**
 * @brief Starts the thread.
 * 
 */
void SimulationThread::Start()
{
    _thread.reset(new std::thread(SimulationThread::run, this));
    _thread_state = ThreadState::RUNNING;
}

/**
 * @brief Stops the thread.
 * 
 */
void SimulationThread::Stop()
{
    _thread_state = ThreadState::EXIT;
    _cv.notify_all();
    Join();
}

/**
 * @brief Thread of execution to run.
 * 
 * @param st 
 */
void SimulationThread::run(SimulationThread* st)
{
    st->Run();
}