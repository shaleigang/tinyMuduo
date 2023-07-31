//
// Created by slg on 7/30/23.
//

#ifndef TINYMUDUO_TIMERQUEUE_H
#define TINYMUDUO_TIMERQUEUE_H

#include <set>

#include <tinyMuduo/base/types.h>
#include <tinyMuduo/base/Timestamp.h>
#include <tinyMuduo/net/Callback.h>
#include <tinyMuduo/net/Timer.h>
#include <tinyMuduo/net/TimerId.h>
#include <tinyMuduo/net/EventLoop.h>
#include <tinyMuduo/net/Channel.h>

namespace tmuduo {
namespace net {

class TimerQueue : noncopyable {
public:
    explicit TimerQueue(EventLoop* loop);
    ~TimerQueue();

    TimerId addTimer(TimerCallback cb, Timestamp when, double intervel);
    void cancel(TimerId timerId);

private:
    typedef std::pair<Timestamp, Timer*> Entry;
    typedef std::set<Entry> TimerList;
    typedef std::pair<Timer*, int64_t> ActiveTimer;
    typedef std::set<ActiveTimer> ActiveTimerSet;

    void addTimerInLoop(Timer* timer);
    void cancelInLoop(TimerId timerId);
    void handleRead();
    std::vector<Entry> getExpired(Timestamp now);
    void reset(const std::vector<Entry>& expired, Timestamp now);

    bool insert(Timer* timer);

    EventLoop* loop_;
    const int timerfd_;
    Channel timerfdChannel_;
    TimerList timers_;

    ActiveTimerSet activeTimers_;
    bool callingExpiredTimers_;
    ActiveTimerSet cancelingTimers_;
};

}
}


#endif //TINYMUDUO_TIMERQUEUE_H
