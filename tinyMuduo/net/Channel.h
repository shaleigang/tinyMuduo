//
// Created by slg on 6/22/23.
//

#ifndef TINYMUDUO_CHANNEL_H
#define TINYMUDUO_CHANNEL_H

#include "../base/Timestamp.h"
#include "../base/noncopyable.h"
#include "EventLoop.h"

#include <functional>

namespace tmuduo {
namespace net {

class Channel : tmuduo::noncopyable {
public:
    typedef std::function<void()> EventCallback;
    typedef std::function<void(Timestamp)> ReadEventCallback;

    Channel(EventLoop* loop, int fd);
    ~Channel();

    void handleEvent(Timestamp receiveTime);
    void tie(const std::shared_ptr<void> &obj);
    void remove();

    void setReadCallback(ReadEventCallback cb)  { readCallback_ = std::move(cb); }
    void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }
    void setCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }
    void setErrorCallback(EventCallback cb) { errorCallback_ = std::move(cb); }

    int fd() const { return fd_; }
    EventLoop* ownerLoop() { return loop_; }
    int events() const { return events_; }
    void set_revents(int revt) { revents_ = revt; }

    bool isNoneEvent() const { return events_ == kNoneEvent; }
    void enableReading() { events_ |= kReadEvent; update(); }
    void enableWriting() { events_ |= kWriteEvent; update(); }
    void disableReading() { events_ &= ~kReadEvent; update(); }
    void disableWriting() { events_ &= ~kWriteEvent; update(); }
    void disableAll() { events_ = kNoneEvent; update(); }
    bool isWriting() const { return events_ & kWriteEvent; }
    bool isReading() const { return events_ & kReadEvent; }

    int index() { return index_; }
    void set_index(int idx) { index_ = idx; }

private:

    void update();
    void handleEventWithGuard(Timestamp receiveTime);

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop* loop_;
    const int fd_;
    int events_;
    int revents_;

    ReadEventCallback readCallback_;
    EventCallback  writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;

    int index_;
    bool logHup_;
    std::weak_ptr<void> tie_;
    bool tied_;
    bool eventHandling_;
    bool addedToLoop_;
};

} // namespace net
} // namespace tmuduo

#endif //TINYMUDUO_CHANNEL_H
