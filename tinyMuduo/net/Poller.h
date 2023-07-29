//
// Created by slg on 6/22/23.
//

#ifndef TINYMUDUO_POLLER_H
#define TINYMUDUO_POLLER_H

#include <tinyMuduo//base/noncopyable.h>
#include <tinyMuduo//base/Timestamp.h>
#include <tinyMuduo/net/Channel.h>
#include <tinyMuduo/net/EventLoop.h>

#include <vector>
#include <map>

namespace tmuduo {
namespace net {

class Poller : noncopyable {
public:
    typedef std::vector<Channel*> ChannelList;

    Poller(EventLoop* loop) : ownerLoop_(loop) {}
    virtual ~Poller() = default;

    virtual tmuduo::Timestamp poll(int timeoutMs, ChannelList* activeChannels) = 0;

    virtual void updateChannel(Channel* channel) = 0;

    virtual void removeChannel(Channel* channel) = 0;

    virtual bool hasChannel(Channel* channel) const{
        assertInLoopThread();
        auto it = channels_.find(channel->fd());
        return it != channels_.end() && it->second == channel;
    }

    static Poller* newDefaultPoller(EventLoop* loop);

    void assertInLoopThread() const{
        ownerLoop_->assertInLoopThread();
    }

protected:
    typedef std::map<int, Channel*> ChannelMap;
    ChannelMap channels_;

private:
    EventLoop* ownerLoop_;

};

} // namespace net
} // namespace tmuduo

#endif //TINYMUDUO_POLLER_H
