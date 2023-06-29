//
// Created by sha on 6/11/23.
//

#include "Thread.h"
#include "CurrentThread.h"
#include <semaphore.h>

#include <memory>

using namespace tmuduo;

Thread::Thread(ThreadFunc &&f, const string &name)
    :   started_(false),
        joined_(false),
        func(f),
        tid_(),
        name_(name){
    setDefaultName();
}

void Thread::start() {
    if(!started_) {
        started_ = true;
        sem_t sem;
        sem_init(&sem, false, 0);
        // 传入一个lambda表达式，线程开始执行
        // 线程首先获取tid，然后再执行func，所以要在tid获取结束之后，线程才算真正开始运行
        // 所以这里采用一个信号量进行同步操作，因为tid()执行的线程和start()运行的线程不同，需要同步
        thread_ = std::make_shared<std::thread>([&](){
            tid_ = CurrentThread::tid();
            sem_post(&sem);
            func();
        });
        sem_wait(&sem);
    }
}

Thread::~Thread() {
    if(started_ && !joined_){
        thread_->detach();
    }
}

void Thread::join(){
    if(!joined_){
        thread_->join();
        joined_ = true;
    }
}

void Thread::detach(){
    if(!joined_){
        thread_->detach();
        joined_ = true;
    }
}

void Thread::setDefaultName() {
    int num = ++numCreated_;
    if(name_.empty()){
        char buf[32];
        snprintf(buf, sizeof(buf), "Thread%d", num);
        name_ = buf;
    }
}

atomic_int32_t Thread::numCreated_(0);