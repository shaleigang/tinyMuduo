//
// Created by slg on 6/27/23.
//

#include "Poller.h"
#include "EPollPoller.h"

using namespace tmuduo::net;

Poller *Poller::newDefaultPoller(EventLoop *loop) {
    return new EPollPoller(loop);
}