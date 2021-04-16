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

   /**
    * @brief Default Constructor.
    * 
    */
   TLockedQueue() = default;

   /**
    * @brief Default Copy Constructor (disabled).
    * 
    */
   TLockedQueue(const TLockedQueue<T> &) = delete;

   /**
    * @brief Assignment operator (disabled).
    * 
    * @return TLockedQueue& 
    */
   TLockedQueue &operator=(const TLockedQueue<T> &) = delete;

   /**
    * @brief Destroy the TLockedQueue object
    * 
    */
   virtual ~TLockedQueue() = default;

   /**
    * @brief Pop item from queue.  Blocks on empty queue.
    * 
    * @return T First item. 
    */
   virtual T dequeue();

   /**
    * @brief Pop item from queue.  Blocks on empty queue.
    * 
    * @tparam T 
    * @param item First item.
    */
   virtual void dequeue(T &item);

   /**
    * @brief Push item into queue.
    * 
    * @tparam T 
    * @param item Item to push.
    */
   virtual void enqueue(T &&item);

   /**
    * @brief Push item into queue.
    * 
    * @tparam T 
    * @param item Item to push.
    */
   virtual void enqueue(const T &item);

   /**
    * @brief Pop item from queue.  Non-blocking.
    * 
    * @tparam T 
    * @param item 
    */
   virtual bool try_dequeue(T &item);

   /**
    * @brief Checks to see if queue is empty.
    * 
    * @tparam T 
    * @return true Queue is empty.
    * @return false Queue is not empty.
    */
   bool Empty() const;

   /**
    * @brief Size of queue.
    * 
    * @tparam T 
    * @return size_t Size of queue.
    */
   size_t Size();

private:

   /**
    * @brief Locks access to shared resources.
    * 
    */
   std::mutex _cs;

   /**
    * @brief Signals thread that either and item has been pushed.
    * 
    */
   std::condition_variable _cv;

   /**
    * @brief Queue container buffer.
    * 
    */
   std::queue<T> _q;
   
};

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
 * @brief Push item into queue.
 * 
 * @tparam T 
 * @param item Item to push.
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
 * @param item Item to push.
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
 * @brief Checks to see if queue is empty.
 * 
 * @tparam T 
 * @return true Queue is empty.
 * @return false Queue is not empty.
 */
template <typename T>
inline bool TLockedQueue<T>::Empty() const
{
   std::unique_lock<std::mutex> lock(_cs);
   return _q.empty();
}

/**
 * @brief Size of queue.
 * 
 * @tparam T 
 * @return size_t Size of queue.
 */
template <typename T>
inline size_t TLockedQueue<T>::Size()
{
   std::unique_lock<std::mutex> lock(_cs);
   return _q.size();
}

#endif