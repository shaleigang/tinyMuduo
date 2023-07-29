//
// Created by slg on 6/26/23.
//

#include <tinyMuduo/net/EventLoopThread.h>

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

    // 和之前Thread类型的方法
    // 通过线程同步，startLoop()等待线程执行threadFunc()函数创建完成EventLoop并获取
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