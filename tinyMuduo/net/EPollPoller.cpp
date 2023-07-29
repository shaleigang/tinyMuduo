//
// Created by slg on 6/22/23.
//

#include <tinyMuduo/net/EPollPoller.h>
#include <tinyMuduo/net/Channel.h>
#include <tinyMuduo/net/Poller.h>
#include <tinyMuduo/base/Logger.h>

#include <cassert>
#include <cerrno>
#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>

using namespace tmuduo::net;

namespace
{
const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;
}

EPollPoller::EPollPoller(EventLoop *loop)
  : Poller(loop),
    epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
    events_(kInitEventListSize){
    if(epollfd_ < 0)
        LOG_ERROR("epoll create error");
}

EPollPoller::~EPollPoller() {
    ::close(epollfd_);
}

tmuduo::Timestamp EPollPoller::poll(int timeoutMs, Poller::ChannelList *activeChannels) {
//    LOG_DEBUG("epoll polling");
    int numEvents = ::epoll_wait(epollfd_,
                                 &*events_.begin(),
                                 static_cast<int>(events_.size()),
                                 timeoutMs);
    int savedErrno = errno;
    tmuduo::Timestamp now(tmuduo::Timestamp::now());
    if(numEvents > 0){
//        LOG_DEBUG("epoll have active event");
        fillActiveChannels(numEvents, activeChannels);
        if(static_cast<size_t>(numEvents) == events_.size()){
            events_.resize(events_.size() * 2);
        }
    }
    else if(numEvents == 0){
        // LOG
    }
    else{
        if(savedErrno != EINTR){
             errno = savedErrno;
            LOG_ERROR("epoll wait error");
        }
    }
    return now;
}

void EPollPoller::fillActiveChannels(int numEvents,
                                     ChannelList* activeChannels) const
{
    assert(static_cast<size_t>(numEvents) <= events_.size());
    for (int i = 0; i < numEvents; ++i)
    {
        Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
        channel->set_revents(events_[i].events);
        activeChannels->push_back(channel);
    }
}

void EPollPoller::updateChannel(Channel* channel)
{
    Poller::assertInLoopThread();
    const int index = channel->index();
//    LOG_DEBUG("updating channel %d", channel->fd());
    if (index == kNew || index == kDeleted)
    {
        int fd = channel->fd();
        if (index == kNew)
        {
            assert(channels_.find(fd) == channels_.end());
            channels_[fd] = channel;
        }
        else // index == kDeleted
        {
            assert(channels_.find(fd) != channels_.end());
            assert(channels_[fd] == channel);
        }

        channel->set_index(kAdded);
        update(EPOLL_CTL_ADD, channel);
//        LOG_DEBUG("fd: %d added to Poller", channel->fd());
    }
    else
    {
        int fd = channel->fd();
        (void)fd;
        assert(channels_.find(fd) != channels_.end());
        assert(channels_[fd] == channel);
        assert(index == kAdded);
        if (channel->isNoneEvent())
        {
            update(EPOLL_CTL_DEL, channel);
            channel->set_index(kDeleted);
        }
        else
        {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}

void EPollPoller::removeChannel(Channel* channel)
{
    Poller::assertInLoopThread();
    int fd = channel->fd();
    // LOG
    assert(channels_.find(fd) != channels_.end());
    assert(channels_[fd] == channel);
    assert(channel->isNoneEvent());
    int index = channel->index();
    assert(index == kAdded || index == kDeleted);
    size_t n = channels_.erase(fd);
    (void)n;
    assert(n == 1);

    if (index == kAdded)
    {
        update(EPOLL_CTL_DEL, channel);
    }
    channel->set_index(kNew);
}

void EPollPoller::update(int operation, Channel* channel)
{
    struct epoll_event event;
    memset(&event, 0, sizeof event);
    event.events = channel->events();
    event.data.ptr = channel;
    int fd = channel->fd();
//     LOG
    if (::epoll_ctl(epollfd_, operation, fd, &event) < 0)
    {
        if (operation == EPOLL_CTL_DEL)
        {
            // LOG
        }
        else
        {
            // LOG
        }
    }
}