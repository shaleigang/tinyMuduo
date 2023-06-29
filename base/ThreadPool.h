//
// Created by sha on 6/12/23.
// 线程池中的多个Thread实例指针存放在vector中，用unique_ptr管理指针
// 线程池构造时只是初始化name， 线程是在start()中创建的
// 同样用vector存储需要执行的task，实现方式和阻塞队列相似，相当于一个有界阻塞队列
// 所有线程执行的函数就是一个while循环，不断从队列中取任务，然后执行
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
    // 存放线程指针的vector
    std::vector<std::unique_ptr<Thread>> threads_;
    // 任务队列
    std::deque<Task> queue_;
    size_t maxQueueSize_;
    bool running_;
};

} // namespace tmuduo
#endif //TINYMUDUO_THREADPOOL_H
