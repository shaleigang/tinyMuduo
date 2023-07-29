//
// Created by slg on 6/26/23.
//

#ifndef TINYMUDUO_EVENTLOOPTHREAD_H
#define TINYMUDUO_EVENTLOOPTHREAD_H

#include <tinyMuduo/base/noncopyable.h>
#include <tinyMuduo/base/types.h>
#include <tinyMuduo/base/Thread.h>
#include <tinyMuduo/net/EventLoop.h>

namespace tmuduo {
namespace net {

class EventLoopThread : noncopyable {
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallback;

    explicit EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(),
                    const string& name = string());
    ~EventLoopThread();
    EventLoop* startLoop();

private:
    void threadFunc();

    EventLoop* loop_;
    bool existing_;
    mutex mutex_;
    Thread thread_;
    condition_variable cond_;
    ThreadInitCallback callback_;
};

} // namespace net
} // namespace tmuduo
#endif //TINYMUDUO_EVENTLOOPTHREAD_H
