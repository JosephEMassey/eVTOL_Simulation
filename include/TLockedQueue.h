#ifndef T_LOCKED_QUEUE_H
#define T_LOCKED_QUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

/**
 * @brief Thread-safe queue
 * 
 * @tparam T 
 * 
 */
template <typename T>
class TLockedQueue
{
public:
    TLockedQueue() = default;
    TLockedQueue(const TLockedQueue<T> &) = delete;             // Disable Copy
    TLockedQueue& operator=(const TLockedQueue<T> &) = delete;  // Disable Assignment
    virtual ~TLockedQueue() = default;

    void enqueue(T&& item);
    void enqueue(const T& item);
    void dequeue(T& item);
    T    dequeue();

private:
    std::queue<T> _q;
    std::mutex _cs;
    std::condition_variable _cv;
};

/**
 * @brief 
 * 
 * @tparam T 
 * @param item 
 * @return true 
 * @return false 
 */
template <typename T>
inline void TLockedQueue<T>::enqueue(const T& item)
{
    std::unique_lock<std::mutex> lock(_cs);
    _q.push(item);
    lock.unlock();
    _cv.notify_one();
}

/**
 * @brief 
 * 
 * @tparam T 
 * @param item 
 */
template <typename T>
inline void TLockedQueue<T>::enqueue(T&& item)
{
    std::unique_lock<std::mutex> lock(_cs);
    _q.push(std::move(item));
    lock.unlock();
    _cv.notify_one();
}

/**
 * @brief Pops front item from queue and will block if queue is empty.
 * 
 * @tparam T 
 * @return optional 
 * @return T item 
 */
template <typename T>
inline T TLockedQueue<T>::dequeue()
{
    std::unique_lock<std::mutex> lock(_cs);
    while (_q.empty())
    {
      _cv.wait(lock);
    }
    auto item = _q.front();
    _q.pop();
    return item;
}

/**
 * @brief 
 * 
 * @tparam T 
 * @param item 
 */
template <typename T>
inline void TLockedQueue<T>::dequeue(T& item)
  {
    std::unique_lock<std::mutex> lock(_cs);
    while (_q.empty())
    {
      _cv.wait(lock);
    }
    item = _q.front();
    _q.pop();
  }

#endif