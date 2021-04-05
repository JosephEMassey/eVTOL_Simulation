#ifndef T_LOCKED_QUEUE_H
#define T_LOCKED_QUEUE_H

#include <deque>
#include <mutex>
#include <optional>

/**
 * @brief 
 * 
 * @tparam T 
 * 
 */
template <typename T>
class TLockedQueue
{
public:
    TLockedQueue();
    TLockedQueue(const TLockedQueue<T> &) = delete;             // Copy constructor
    TLockedQueue& operator=(const TLockedQueue<T> &) = delete;  // Assignment
    TLockedQueue(TLockedQueue<T>&& o);

    virtual ~TLockedQueue();

    bool is_empty() const;
    size_t size();

    void enqueue(const T &item);
    std::optional<T> dequeue();

private:
    std::deque<T> q;
    std::mutex cs;
};

/**
 * @brief Default constructor.
 * 
 * @tparam T 
 */
template <typename T>
inline TLockedQueue<T>::TLockedQueue()
{

}

/**
 * @brief Move constructor.
 * 
 * @tparam T 
 */
template <typename T>
inline TLockedQueue<T>::TLockedQueue(TLockedQueue<T>&& o)
{
    std::lock_guard<std::mutex> lock(cs);
    q = std::move(o.q);
}

/**
 * @brief Default destructor.
 * 
 * @tparam T 
 */
template <typename T>
inline TLockedQueue<T>::~TLockedQueue()
{
    
}

/**
 * @brief 
 * 
 * @tparam T 
 * @return bool 
 */
template <typename T>
inline bool TLockedQueue<T>::is_empty() const
{
    std::lock_guard<std::mutex> lock(cs);
    return q.empty();
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
    std::lock_guard<std::mutex> lock(cs);
    return q.size();
}

/**
 * @brief 
 * 
 * @tparam T 
 * @param item 
 * @return true 
 * @return false 
 */
template <typename T>
inline void TLockedQueue<T>::enqueue(const T &item)
{
    std::lock_guard<std::mutex> lock(cs);
    q.push_back(item);
}

/**
 * @brief 
 * 
 * @tparam T 
 * @return optional 
 * @return T item 
 */
template <typename T>
inline std::optional<T> TLockedQueue<T>::dequeue()
{
    std::lock_guard<std::mutex> lock(cs);
    if ( q.empty() )
      return {};

    T t = q.front();
    q.pop_front();
    return t;
}

#endif