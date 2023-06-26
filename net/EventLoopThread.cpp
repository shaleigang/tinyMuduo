//
// Created by slg on 6/26/23.
//

#include "EventLoopThread.h"

using namespace tmuduo;
using namespace tmuduo::net;

EventLoopThread::EventLoopThread(const ThreadInitCallback &cb, const string &name)
  : loop_(nullptr),
    existing_(false),
    thread_(std::bind(&EventLoopThread::threadFunc, this), name),
    callback_(cb){

}

EventLoopThread::~EventLoopThread() {
    existing_ = true;
    if(loop_ != nullptr){
        loop_->quit();
        thread_.join();
    }
}

EventLoop* EventLoopThread::startLoop() {
    thread_.start();

    EventLoop* loop = nullptr;
    {
        std::unique_lock<mutex> lock(mutex_);
        while(loop_ == nullptr){
            cond_.wait(lock);
        }
        loop = loop_;
    }
    return loop;
}

void EventLoopThread::threadFunc() {
    EventLoop loop;
    if(callback_){
        callback_(&loop);
    }

    {
        std::unique_lock<mutex> lock(mutex_);
        loop_ = &loop;
        cond_.notify_one();
    }

    loop.loop();
    std::unique_lock<mutex> lock(mutex_);
    loop_ = nullptr;

}