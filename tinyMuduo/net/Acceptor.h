//
// Created by slg on 6/26/23.
//

#ifndef TINYMUDUO_ACCEPTOR_H
#define TINYMUDUO_ACCEPTOR_H

#include <tinyMuduo/net/EventLoop.h>
#include <tinyMuduo/net/Socket.h>
#include <tinyMuduo/net/Channel.h>
#include <tinyMuduo/net/InetAddress.h>

#include <functional>

namespace tmuduo {
namespace net {

class Acceptor {
public:
    typedef std::function<void(int sockfd, const InetAddress&)> NewConnectionCallback;

    Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);
    ~Acceptor();

    void setNewConnectionCallback(const NewConnectionCallback& cb){
        newConnectionCallback_ = cb;
    }

    void listen();
    bool listening() const { return listening_; }

private:
    void handleRead();

    EventLoop* loop_;
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewConnectionCallback newConnectionCallback_;
    bool listening_;
    int idleFd_;
};

} // namespace net
} // namespace tmuduo
#endif //TINYMUDUO_ACCEPTOR_H
