//
// Created by slg on 6/22/23.
//

#include "EventLoop.h"

using namespace tmuduo;
using namespace tmuduo::net;

__thread EventLoop* t_loopInThisThread = 0;
const int kPollTimeMs = 10000;

