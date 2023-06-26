//
// Created by slg on 6/22/23.
//

#ifndef TINYMUDUO_EVENTLOOP_H
#define TINYMUDUO_EVENTLOOP_H

#include "../base/noncopyable.h"
#include "../base/Timestamp.h"
#include "../base/CurrentThread.h"

#include <functional>
#include <vector>
#include <atomic>

namespace tmuduo {
namespace net {

class Channel;
class Poller;

class EventLoop : noncopyable {
public:
    typedef std::function<void()> Functor;

    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    void runInLoop(Functor cb);
    void queueInLoop(Functor cb);

    void wakeup();
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
    bool hasChannel(Channel* channel);

    void assertInLoopThread(){
        if(!isInLoopThread()){
            abortNotInLoopThread();
        }
    }

    bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
    bool eventHandling() const { return eventHandling_; }

    static EventLoop* getEventLoopOfCurrentThread();

private:
    typedef std::vector<Channel*> ChannelList;

    void abortNotInLoopThread();
    void handleRead();
    void doPendingFunctors();

    std::atomic<bool> looping_;
    std::atomic<bool> quit_;
    std::atomic<bool> eventHandling_;
    std::atomic<bool> callingPendingFunctors_;

    const pid_t threadId_;
    Timestamp pollReturnTime_;

    std::unique_ptr<Poller> poller_;
    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;

    ChannelList activeChannels_;
    Channel* currentActiveChannel_;

    mutable std::mutex mutex_;
    std::vector<Functor> pendingFunctors_;
};

} // namespace net
} // namespace tmuduo


#endif //TINYMUDUO_EVENTLOOP_H
