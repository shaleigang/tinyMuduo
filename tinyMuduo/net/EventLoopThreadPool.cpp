//
// Created by slg on 6/26/23.
//

#include <tinyMuduo/net/EventLoopThreadPool.h>

#include <cstdio>

using namespace tmuduo;
using namespace tmuduo::net;

EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseloop, const string &nameArg)
  : baseLoop_(baseloop),
    name_(nameArg),
    started_(false),
    numThreads_(0),
    next_(0){

}

EventLoopThreadPool::~EventLoopThreadPool() {

}

void EventLoopThreadPool::start(const EventLoopThreadPool::ThreadInitCallback &cb) {
    baseLoop_->assertInLoopThread();
    started_ = true;
    for(int i = 0; i < numThreads_; ++i) {
        char buf[name_.size() + 32];
        snprintf(buf, sizeof buf, "%s%d", name_.c_str(), i);
        EventLoopThread* t = new EventLoopThread(cb, buf);
        threads_.push_back(std::unique_ptr<EventLoopThread>(t));
        loops_.push_back(t->startLoop());
    }
    if(numThreads_ == 0 && cb){
        cb(baseLoop_);
    }
}

EventLoop* EventLoopThreadPool::getNextLoop() {
    baseLoop_->assertInLoopThread();
    EventLoop* loop = baseLoop_;

    if (!loops_.empty())
    {
        loop = loops_[next_];
        ++next_;
        if (static_cast<size_t>(next_) >= loops_.size())
        {
            next_ = 0;
        }
    }
    return loop;
}