//
// Created by slg on 6/22/23.
//

#include <tinyMuduo/net/EventLoop.h>
#include <tinyMuduo/net/Channel.h>

#include <cassert>
#include <poll.h>

using namespace tmuduo;
using namespace tmuduo::net;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(tmuduo::net::EventLoop *loop, int fd)
  : loop_(loop),
    fd_(fd),
    events_(0),
    revents_(0),
    index_(-1),
    logHup_(true),
    tied_(false),
    eventHandling_(false),
    addedToLoop_(false){

}

Channel::~Channel() {
    assert(!eventHandling_);
    assert(!addedToLoop_);
    if(loop_->isInLoopThread()){
        assert(!loop_->hasChannel(this));
    }
}

void Channel::tie(const std::shared_ptr<void> &obj) {
    tie_ = obj;
    tied_ = true;
}

void Channel::update() {
    addedToLoop_ = true;
    loop_->updateChannel(this);
}

void Channel::remove() {
    assert(isNoneEvent());
    addedToLoop_ = false;
    loop_->removeChannel(this);;
}

void Channel::handleEvent(tmuduo::Timestamp receiveTime) {
    std::shared_ptr<void> guard;
    if(tied_){
        guard = tie_.lock();
        if(guard){
            handleEventWithGuard(receiveTime);
        }
    }
    else{
        handleEventWithGuard(receiveTime);
    }
}

void Channel::handleEventWithGuard(tmuduo::Timestamp receiveTime) {
    eventHandling_ = true;
    // LOG
    if((revents_ & POLLHUP) && !(revents_ & POLLIN)){
        if(logHup_){
            // LOG
        }
        if(closeCallback_) closeCallback_();
    }

    if(revents_ & POLLNVAL){
        // LOG
    }

    if (revents_ & (POLLERR | POLLNVAL)){
        if (errorCallback_) errorCallback_();
    }
    if (revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
        if (readCallback_) readCallback_(receiveTime);
    }
    if (revents_ & POLLOUT){
        if (writeCallback_) writeCallback_();
    }
    eventHandling_ = false;
}

