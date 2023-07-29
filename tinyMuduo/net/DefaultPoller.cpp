//
// Created by slg on 6/27/23.
//

#include <tinyMuduo/net/EPollPoller.h>
#include <tinyMuduo/net/Poller.h>

using namespace tmuduo::net;

Poller *Poller::newDefaultPoller(EventLoop *loop) {
    return new EPollPoller(loop);
}