//
// Created by slg on 6/22/23.
//

#ifndef TINYMUDUO_EPOLLPOLLER_H
#define TINYMUDUO_EPOLLPOLLER_H

#include <tinyMuduo/net/Poller.h>
#include <tinyMuduo/base/Timestamp.h>

#include <vector>

struct epoll_event;

namespace tmuduo {
namespace net {

class EPollPoller : public Poller {
public:
    explicit EPollPoller(EventLoop* loop);
    ~EPollPoller() override;

    tmuduo::Timestamp poll(int timeoutMs, ChannelList* activeChannels) override;
    void updateChannel(Channel* channel) override;
    void removeChannel(Channel* channel) override;

private:
    static const int kInitEventListSize = 16;

    void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;
    void update(int operation, Channel* channel);

    typedef std::vector<struct epoll_event> EventList;
    int epollfd_;
    EventList events_;

};

} // namespace net
} // namespace tmuduo

#endif //TINYMUDUO_EPOLLPOLLER_H
