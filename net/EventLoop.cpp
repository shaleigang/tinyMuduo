//
// Created by slg on 6/22/23.
//

#include "EventLoop.h"
#include "../base/Logger.h"
#include "../base/CurrentThread.h"
#include "Poller.h"

#include <sys/eventfd.h>
#include <unistd.h>
#include <cassert>
#include <algorithm>

using namespace tmuduo;
using namespace tmuduo::net;

namespace {

__thread EventLoop *t_loopInThisThread = nullptr;
const int kPollTimeMs = 10000;

int createEventfd() {
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0) {
        LOG_ERROR("Failed in eventfd");
        abort();
    }
    return evtfd;
}

}

EventLoop* EventLoop::getEventLoopOfCurrentThread()
{
    return t_loopInThisThread;
}

EventLoop::EventLoop()
  : looping_(false),
    quit_(false),
    eventHandling_(false),
    callingPendingFunctors_(false),
    threadId_(CurrentThread::tid()),
    poller_(Poller::newDefaultPoller(this)),
    wakeupFd_(createEventfd()),
    wakeupChannel_(new Channel(this, wakeupFd_)),
    currentActiveChannel_(nullptr){

    LOG_DEBUG("EventLoop created in thread %d", threadId_);
//    LOG_DEBUG("wakeup fd: %d", wakeupFd_);
    if(t_loopInThisThread){
        LOG_FATAL("Another EventLoop exists in this thread %d", threadId_);
    }
    else{
        t_loopInThisThread = this;
    }
    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
    wakeupChannel_->enableReading();
}

EventLoop::~EventLoop() {
    LOG_DEBUG("EventLoop of thread %d destructs in thread %d", threadId_, CurrentThread::tid());
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    ::close(wakeupFd_);
    t_loopInThisThread = nullptr;
}

void EventLoop::loop() {
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    quit_ = false;
    LOG_DEBUG("EventLoop start looping");
    while(!quit_){
        activeChannels_.clear();
        pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
        eventHandling_ = true;
        for(auto channel : activeChannels_){
            currentActiveChannel_ = channel;
            currentActiveChannel_->handleEvent(pollReturnTime_);
        }
        currentActiveChannel_ = nullptr;
        eventHandling_ = false;
        doPendingFunctors();
    }
    LOG_DEBUG("EventLoop stop looping");
    looping_ = false;
}

void EventLoop::quit() {
    quit_ = true;
    if(!isInLoopThread()) {
        wakeup();
    }
}

void EventLoop::runInLoop(tmuduo::net::EventLoop::Functor cb) {
    if(isInLoopThread()){
        cb();
    }
    else{
        queueInLoop(std::move(cb));
    }
}

void EventLoop::queueInLoop(tmuduo::net::EventLoop::Functor cb) {
    {
        std::unique_lock<mutex> lock(mutex_);
        pendingFunctors_.push_back(std::move(cb));
    }
    if(!isInLoopThread() || callingPendingFunctors_){
        wakeup();
    }
}

void EventLoop::updateChannel(tmuduo::net::Channel *channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(tmuduo::net::Channel *channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    if(eventHandling_){
        assert(currentActiveChannel_ == channel ||
               std::find(activeChannels_.begin(), activeChannels_.end(), channel) == activeChannels_.end());
    }
    poller_->removeChannel(channel);
}

bool EventLoop::hasChannel(tmuduo::net::Channel *channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    return poller_->hasChannel(channel);
}

void EventLoop::abortNotInLoopThread()
{
    LOG_FATAL("EventLoop::abortNotInLoopThread - EventLoop was created in threadId_ = %d, current thread id = %d", threadId_, CurrentThread::tid());
}

void EventLoop::wakeup() {
    uint64_t one = 1;
    ssize_t n = ::write(wakeupFd_, &one, sizeof(one));
    if(n != sizeof one){
        LOG_ERROR("EventLoop::wakeup() writes %d bytes instead of 8", n);
    }
}

void EventLoop::handleRead() {
    uint64_t one = 1;
    ssize_t n = ::read(wakeupFd_, &one, sizeof(one));
    if(n != sizeof one){
        LOG_ERROR("EventLoop::handleRead() reads %d bytes instead of 8", n);
    }
}

void EventLoop::doPendingFunctors() {
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;

    {
        std::unique_lock<mutex> lock(mutex_);
        functors.swap(pendingFunctors_);
    }

    for(const Functor& functor : functors){
        functor();
    }
    callingPendingFunctors_ = false;
}