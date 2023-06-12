//
// Created by sha on 6/12/23.
//

#include "ThreadPool.h"
#include <cassert>
#include <iostream>

using namespace tmuduo;

ThreadPool::ThreadPool(const std::string &nameArg)
  : name_(nameArg),
    running_(false){

}

void ThreadPool::runInThread() {
    if(threadInitCallback_){
        threadInitCallback_();
    }
    while(running_){
        Task task(take());
        if(task){
            task();
        }
    }
}

void ThreadPool::start(int numThreads) {
    assert(threads_.empty());
    running_ = true;
    threads_.reserve(numThreads);
    for(int i = 0; i < numThreads; ++i){
        char id[32];
        snprintf(id, sizeof(id), "%d", i + 1);
        threads_.emplace_back(new Thread(
                std::bind(&ThreadPool::runInThread, this), name_+id
                ));
        threads_[i]->start();
    }
}

void ThreadPool::stop(){
    {
        std::unique_lock<mutex> lock(mutex_);
        running_ = false;
        notEmpty_.notify_all();
        notFull_.notify_all();
    }
    for(auto& thr : threads_){
        thr->join();
    }
}

ThreadPool::~ThreadPool() {
    if(running_){
        stop();
    }
}

void ThreadPool::run(Task task) {
    if(threads_.empty()){
        task();
    }
    else{
        std::unique_lock<mutex> lock(mutex_);
        while(maxQueueSize_ > 0 && queue_.size() >= maxQueueSize_ && running_) {
            notFull_.wait(lock);
        }
        if(!running_) return;
        queue_.push_back(std::move(task));
        notEmpty_.notify_one();
    }
}

ThreadPool::Task ThreadPool::take() {
    std::unique_lock<mutex> lock(mutex_);
    while(queue_.empty() && running_) {
        notEmpty_.wait(lock);
    }
    Task task;
    if(!queue_.empty()) {
        task = queue_.front();
        queue_.pop_front();
        if (maxQueueSize_ > 0) {
            notFull_.notify_one();
        }
    }
    return task;
}

size_t ThreadPool::queueSize() const {
    std::unique_lock<mutex> lock(mutex_);
    return queue_.size();
}

bool ThreadPool::isFull() const {
    std::unique_lock<mutex> lock(mutex_);
    return maxQueueSize_ > 0 && queue_.size() >= maxQueueSize_;
}
