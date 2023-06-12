//
// Created by sha on 6/11/23.
//

#ifndef TINYMUDUO_BLOCKINGQUEUE_H
#define TINYMUDUO_BLOCKINGQUEUE_H

#include <mutex>
#include <condition_variable>
#include <deque>

namespace tmuduo {

template<typename T>
class BlockingQueue {
public:
    typedef std::deque<T> queueType;

    BlockingQueue() = default;

    void put(const T& x);

    void put(T&& x);

    T take();

    queueType drain();

    size_t size() const;

private:
    mutable std::mutex mutex_;
    std::condition_variable notEmpty_;
    queueType queue_;
};

template<typename T>
void BlockingQueue<T>::put(const T& x) {
    std::unique_lock<mutex> lock(mutex_);
    queue_.push_back(x);
    notEmpty_.notify_one();
}

template<typename T>
void BlockingQueue<T>::put(T&& x) {
    std::unique_lock<mutex> lock(mutex_);
    queue_.push_back(std::move(x));
    notEmpty_.notify_all();
}

template<typename T>
T BlockingQueue<T>::take() {
    std::unique_lock<mutex> lock(mutex_);
    notEmpty_.wait(lock);
    T front(std::move(queue_.front()));
    queue_.pop_front();
    return front;
}

template<typename T>
typename BlockingQueue<T>::queueType BlockingQueue<T>::drain() {
    typename BlockingQueue<T>::queueType res;
    {
        std::unique_lock<mutex> lock(mutex_);
        res = std::move(queue_);
        assert(queue_.empty());
    }
    return res;
}

template<typename T>
size_t BlockingQueue<T>::size() const {
    std::unique_lock<mutex> lock(mutex_);
    return queue_.size();
}

} //namespace tmuduo

#endif //TINYMUDUO_BLOCKINGQUEUE_H
