//
// Created by slg on 6/26/23.
//

#ifndef TINYMUDUO_EVENTLOOPTHREADPOOL_H
#define TINYMUDUO_EVENTLOOPTHREADPOOL_H

#include "../base/noncopyable.h"
#include "EventLoopThread.h"
#include "../base/types.h"

#include <vector>

namespace tmuduo {
namespace net {

class EventLoopThreadPool {
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallback;

    EventLoopThreadPool(EventLoop* baseloop, const string& nameArg);
    ~EventLoopThreadPool();

    void setThreadNum(int numThreads) { numThreads_ = numThreads; }
    void start(const ThreadInitCallback& cb = ThreadInitCallback());
    EventLoop* getNextLoop();

    bool started() const { return started_; }
    const string& name() const { return name_; }

private:
    EventLoop* baseLoop_;
    string name_;
    bool started_;
    int numThreads_;
    int next_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;
};


} // namespace net
} // namespace tmuduo
#endif //TINYMUDUO_EVENTLOOPTHREADPOOL_H
