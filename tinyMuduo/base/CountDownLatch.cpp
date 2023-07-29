//
// Created by sha on 6/11/23.
//

#include <tinyMuduo/base/CountDownLatch.h>

using namespace tmuduo;

CountDownLatch::CountDownLatch(int count) : count_(count) {}

int CountDownLatch::getCount() const {
    std::unique_lock<mutex> lock(mutex_);
    return count_;
}

void CountDownLatch::countDown() {
    std::unique_lock<mutex> lock(mutex_);
    --count_;
    if(0 == count_){
        cond_.notify_all();
    }
}

void CountDownLatch::wait() {
    std::unique_lock<mutex> lock(mutex_);
//    cond_.wait(lock, [this](){ return 0 == count_; });
    cond_.wait(lock);
}
