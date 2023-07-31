//
// Created by slg on 7/30/23.
//

#include <tinyMuduo/net/Timer.h>

using namespace tmuduo::net;

std::atomic_int64_t Timer::s_numCreated_;

void Timer::restart(tmuduo::Timestamp now) {
    if (repeat_){
        expiration_ = addTime(now, interval_);
    }
    else{
        expiration_ = Timestamp::invalid();
    }
}
