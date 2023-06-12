//
// Created by sha on 6/12/23.
//

#ifndef TINYMUDUO_THREADPOOL_H
#define TINYMUDUO_THREADPOOL_H

#include "noncopyable.h"
#include "types.h"
#include "Thread.h"
#include <deque>
#include <vector>

namespace tmuduo {

class ThreadPool : noncopyable{
public:
    typedef std::function<void ()> Task;

    explicit ThreadPool(const string& nameArg = string("ThreadPool"));
    ~ThreadPool();

    void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }
    void setThreadInitCallback(const Task& cb){
        threadInitCallback_ = cb;
    }

    void start(int numThreads);
    void stop();

    const string& name() const { return name_; }
    size_t queueSize() const;

    void run(Task task);

private:
    bool isFull() const;
    void runInThread();
    Task take();

    mutable mutex mutex_;
    condition_variable notEmpty_;
    condition_variable notFull_;
    string name_;
    Task threadInitCallback_;
    std::vector<std::unique_ptr<Thread>> threads_;
    std::deque<Task> queue_;
    size_t maxQueueSize_;
    bool running_;
};

} // namespace tmuduo
#endif //TINYMUDUO_THREADPOOL_H
