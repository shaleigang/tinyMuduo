//
// Created by sha on 6/11/23.
//

#include "Thread.h"
#include "CurrentThread.h"
#include <semaphore.h>

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
        thread_ = std::shared_ptr<std::thread>(new std::thread([&](){
            tid_ = CurrentThread::tid();
            sem_post(&sem);
            func();
        }));
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