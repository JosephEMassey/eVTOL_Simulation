#ifndef T_LOCKED_QUEUE_H
#define T_LOCKED_QUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

/**
 * @brief Thread-safe multi-producer / multi-consumer queue.
 * 
 * @tparam T 
 * 
 */
template <typename T>
class TLockedQueue
{
public:
   TLockedQueue() = default;
   TLockedQueue(const TLockedQueue<T> &) = delete;            // Disable Copy
   TLockedQueue &operator=(const TLockedQueue<T> &) = delete; // Disable Assignment
   virtual ~TLockedQueue() = default;

   virtual void enqueue(T &&item);
   virtual void enqueue(const T &item);
   virtual void dequeue(T &item);
   virtual T dequeue();

   virtual bool try_dequeue(T &item);

   size_t size();
   bool empty() const;

private:
   std::queue<T> _q;
   std::mutex _cs;
   std::condition_variable _cv;
};

/**
 * @brief Push item into queue.
 * 
 * @tparam T 
 * @param item 
 */
template <typename T>
inline void TLockedQueue<T>::enqueue(const T &item)
{
   std::unique_lock<std::mutex> lock(_cs);

   _q.push(item);
   lock.unlock();
   _cv.notify_one();
}

/**
 * @brief Push (move) item into queue.
 * 
 * @tparam T 
 * @param item 
 */
template <typename T>
inline void TLockedQueue<T>::enqueue(T &&item)
{
   std::unique_lock<std::mutex> lock(_cs);

   _q.push(std::move(item));
   lock.unlock();
   _cv.notify_one();
}

/**
 * @brief Pop item from queue.  Blocks on empty queue.
 * 
 * @return T First item. 
 */
template <typename T>
inline T TLockedQueue<T>::dequeue()
{
   std::unique_lock<std::mutex> lock(_cs);
   
   _cv.wait(lock, [this]{ return !_q.empty(); });

   auto item = _q.front();
   _q.pop();
   return item;
}

/**
 * @brief Pop item from queue.  Blocks on empty queue.
 * 
 * @tparam T 
 * @param item First item.
 */
template <typename T>
inline void TLockedQueue<T>::dequeue(T &item)
{
   std::unique_lock<std::mutex> lock(_cs);
   
   _cv.wait(lock, [this]{ return !_q.empty(); } );

   item = _q.front();
   _q.pop();
}

/**
 * @brief Pop item from queue.  Non-blocking.
 * 
 * @tparam T 
 * @param item 
 */
template <typename T>
inline bool TLockedQueue<T>::try_dequeue(T &item)
{
   std::unique_lock<std::mutex> lock(_cs);
   if(_q.empty())
      return false;

   item = _q.front();
   _q.pop();
   return true;
}

/**
 * @brief 
 * 
 * @tparam T 
 * @return size_t 
 */
template <typename T>
inline size_t TLockedQueue<T>::size()
{
   std::unique_lock<std::mutex> lock(_cs);
   return _q.size();
}

/**
 * @brief 
 * 
 * @tparam T 
 * @return bool 
 */
template <typename T>
inline bool TLockedQueue<T>::empty() const
{
   std::unique_lock<std::mutex> lock(_cs);
   return _q.empty();
}

#endif