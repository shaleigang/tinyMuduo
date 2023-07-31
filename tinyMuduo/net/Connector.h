//
// Created by slg on 7/30/23.
//

#ifndef TINYMUDUO_CONNECTOR_H
#define TINYMUDUO_CONNECTOR_H

#include <tinyMuduo/base/noncopyable.h>
#include <tinyMuduo/net/EventLoop.h>
#include <tinyMuduo/net/InetAddress.h>
#include <tinyMuduo/net/Channel.h>

#include <functional>
#include <memory>

namespace tmuduo {
namespace net {

class Connector : public noncopyable, public std::enable_shared_from_this<Connector>{
public:
    typedef std::function<void (int sockfd)> NewConnectionCallback;

    Connector(EventLoop* loop, const InetAddress& serverAddr);
    ~Connector();

    void setConnectionCallback(const NewConnectionCallback& cb) { newConnectionCallback_ = cb; }

    void start();
    void restart();
    void stop();

    const InetAddress& serverAddress() const { return serverAddr_; }

private:
    enum States { kDisconnected, kConnecting, kConnected };
    static const int kMaxRetryDelayMs = 30*1000;
    static const int kInitRetryDelayMs = 500;

    void setState(States s) { state_ = s; }
    void startInLoop();
    void stopInLoop();
    void connect();
    void connecting(int sockfd);
    void handleWrite();
    void handleError();
    void retry(int sockfd);
    int removeAndResetChannel();
    void resetChannel();

    EventLoop* loop_;
    InetAddress serverAddr_;
    bool connect_;
    States state_;
    std::unique_ptr<Channel> channel_;
    NewConnectionCallback newConnectionCallback_;
    int retryDelayMs_;
};


}
}


#endif //TINYMUDUO_CONNECTOR_H
